/**
 * MAIN.CPP - Universal Test Bench
 * Version définie dans platformio.ini
 */
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

// --- Objets Globaux ---
WiFiMulti wifiMulti;
AsyncWebServer server(80);
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
DHT dht(PIN_DHT, DHT_TYPE);
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// --- Variables ---
int valAnalog = 0;
int valDigital = 0;
float temp = 0.0;
float hum = 0.0;
String ipAddress = "Connecting...";

// --- HTML Interface Web ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>%PROJECT_NAME% v%VERSION%</title>
  <style>
    body { font-family: 'Segoe UI', sans-serif; text-align: center; margin:0; padding:0; background-color: #1a1a1a; color: #eee; }
    header { background-color: #000; padding: 15px; border-bottom: 3px solid #00aa00; margin-bottom: 20px;}
    h1 { margin:0; font-size: 1.5rem; color: #fff; }
    .sub-header { color: #888; font-size: 0.8rem; }
    .container { display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; padding: 10px; }
    .card { background-color: #2a2a2a; padding: 20px; border-radius: 12px; width: 280px; border: 1px solid #444; }
    .value { font-size: 3rem; font-weight: bold; margin: 10px 0; color: #00ddff; }
    .label { font-size: 0.9rem; color: #aaa; text-transform: uppercase; }
    .sub-value { font-size: 1rem; color: #888; }
    .digital-on { color: #00ff00; } .digital-off { color: #ff3333; }
    .btn { background-color: #008CBA; border: none; color: white; padding: 15px 32px; text-align: center; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 5px; width: 100%; }
    .btn-red { background-color: #f44336; }
  </style>
  <script>
    setInterval(getData, 300); setInterval(getEnv, 2000);
    function getData() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var obj = JSON.parse(this.responseText);
          document.getElementById("valAnalog").innerHTML = obj.analog;
          document.getElementById("valVolt").innerHTML = (obj.analog * 3.3 / 4095).toFixed(2) + " V";
          var dig = document.getElementById("valDigital");
          if(obj.digital == 1) { dig.innerHTML = "HIGH"; dig.className = "value digital-on"; } 
          else { dig.innerHTML = "LOW"; dig.className = "value digital-off"; }
        }
      };
      xhttp.open("GET", "/data", true); xhttp.send();
    }
    function getEnv() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var obj = JSON.parse(this.responseText);
          document.getElementById("valTemp").innerHTML = obj.temp + " &deg;C";
          document.getElementById("valHum").innerHTML = obj.hum + " %";
        }
      };
      xhttp.open("GET", "/env", true); xhttp.send();
    }
    function triggerAction(type) { var xhr = new XMLHttpRequest(); xhr.open("GET", "/action?type=" + type, true); xhr.send(); }
  </script>
</head>
<body>
  <header>
    <h1>%PROJECT_NAME%</h1>
    <div class="sub-header">Version %VERSION%</div>
  </header>
  <div class="container">
    <div class="card">
      <div class="label">Analog (GPIO %PIN_A%)</div>
      <div id="valAnalog" class="value">0</div>
      <div id="valVolt" class="sub-value">0.00 V</div>
    </div>
    <div class="card">
      <div class="label">Digital (GPIO %PIN_D%)</div>
      <div id="valDigital" class="value">LOW</div>
      <div class="sub-value">Binary State</div>
    </div>
    <div class="card">
      <div class="label">Env (GPIO %PIN_DHT%)</div>
      <div style="display:flex; justify-content:space-around;">
        <div><div id="valTemp" class="value" style="font-size:2rem; color:#ffcc00;">--</div><div class="sub-value">Temp</div></div>
        <div><div id="valHum" class="value" style="font-size:2rem; color:#00ccff;">--</div><div class="sub-value">Hum</div></div>
      </div>
    </div>
    <div class="card">
      <div class="label">Actions</div>
      <button class="btn" onclick="triggerAction('buzzer')">🔊 BUZZER</button>
      <button class="btn btn-red" onclick="triggerAction('led')">💡 RGB LED</button>
    </div>
  </div>
</body>
</html>
)rawliteral";

// --- TFT Display ---
void drawBaseInterface() {
  tft.fillScreen(C_BLACK);
  tft.setTextSize(2); tft.setTextColor(C_ORANGE);
  tft.setCursor(10, 10); 
  // Affichage version courte sur TFT pour éviter dépassement
  tft.print("TEST BENCH v"); tft.print(PROJECT_VERSION); 
  
  tft.drawFastHLine(0, 35, TFT_WIDTH, C_DKGREY);
  tft.setTextSize(1); tft.setTextColor(C_WHITE);
  tft.setCursor(10, 45); tft.print("IP: "); tft.print(ipAddress);

  // Cadres
  tft.drawRoundRect(5, 60, 230, 70, 5, C_BLUE);
  tft.setTextColor(C_BLUE); tft.setCursor(15, 65); tft.print("ANALOG (GPIO "); tft.print(PIN_TEST_ANALOG); tft.print(")");

  tft.drawRoundRect(5, 135, 230, 60, 5, C_RED);
  tft.setTextColor(C_RED); tft.setCursor(15, 140); tft.print("DIGITAL (GPIO "); tft.print(PIN_TEST_DIGITAL); tft.print(")");

  tft.drawFastHLine(0, 205, TFT_WIDTH, C_DKGREY);
  tft.setTextColor(C_YELLOW); tft.setCursor(10, 215); tft.print("DHT11 (GPIO "); tft.print(PIN_DHT); tft.print(")");
}

void updateTFT() {
  // Analog
  tft.fillRect(15, 78, 100, 25, C_BLACK); 
  tft.setCursor(15, 78); tft.setTextSize(3); tft.setTextColor(C_GREEN); tft.print(valAnalog);
  int barWidth = map(valAnalog, 0, 4095, 0, 210);
  tft.fillRect(15, 108, 210, 8, C_DKGREY); tft.fillRect(15, 108, barWidth, 8, C_GREEN);

  // Digital
  tft.fillRect(15, 155, 200, 30, C_BLACK); tft.setCursor(20, 155); tft.setTextSize(3);
  if (valDigital == HIGH) { tft.setTextColor(C_GREEN); tft.print("HIGH"); tft.fillCircle(200, 165, 8, C_GREEN); } 
  else { tft.setTextColor(C_RED); tft.print("LOW"); tft.drawCircle(200, 165, 8, C_RED); }
}

void updateTFT_Env() {
  tft.fillRect(100, 215, 140, 20, C_BLACK);
  tft.setTextSize(1); tft.setTextColor(C_WHITE); tft.setCursor(100, 215);
  if (isnan(temp) || isnan(hum)) { tft.print("No Sensor"); } 
  else { tft.print((int)temp); tft.print(" C | "); tft.print((int)hum); tft.print(" %"); }
}

void setup() {
  Serial.begin(115200);
  delay(50); // Laisser le temps au port série de s'initialiser
  Serial.println("[BOOT] START");
  pinMode(PIN_TEST_ANALOG, INPUT);
  pinMode(PIN_TEST_DIGITAL, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT); digitalWrite(PIN_BUZZER, LOW);
  
  #ifdef NEOPIXEL_POWER
    pinMode(NEOPIXEL_POWER, OUTPUT); digitalWrite(NEOPIXEL_POWER, HIGH);
  #endif
  pixels.begin(); pixels.setBrightness(NEO_BRIGHTNESS); pixels.clear(); pixels.show();
  dht.begin();

  pinMode(TFT_BL, OUTPUT); digitalWrite(TFT_BL, HIGH);
  tft.init(TFT_WIDTH, TFT_HEIGHT); tft.setRotation(TFT_ROTATION);
  tft.fillScreen(C_BLACK);
  tft.setTextColor(C_WHITE); tft.setCursor(20, 100); tft.setTextSize(2); tft.print("CONNECTING...");

  Serial.println("[BOOT] Init WiFi STA");
  WiFi.mode(WIFI_STA);
  Serial.print("[MAC] "); Serial.println(WiFi.macAddress());
  // Ajout des SSID connus
  wifiMulti.addAP(ssid_1, password_1);
  wifiMulti.addAP(ssid_2, password_2);

  // Handler simple des évènements WiFi (réduit)
  WiFi.onEvent([](arduino_event_t *event){
    if (event->event_id == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
      Serial.print("[IP] "); Serial.println(WiFi.localIP());
    } else if (event->event_id == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
      Serial.println("[EVENT] DISCONNECTED");
    }
  });

  // Tentatives de connexion STA
  int attempts = 0;
  while (wifiMulti.run() != WL_CONNECTED && attempts < 25) {
    attempts++;
    if (attempts % 5 == 1) Serial.printf("[WiFi] Tentative %d...\n", attempts);
    delay(400);
  }
  bool connected = (wifiMulti.run() == WL_CONNECTED);
  if (connected) {
    ipAddress = WiFi.localIP().toString();
  } else {
    ipAddress = "OFFLINE";
  }

  // Journal de connexion réseau (toujours affiché au démarrage)
  Serial.println("\n===== Réseau (Phase Initiale) =====");
  if (connected) {
    Serial.println("WiFi connecté");
    Serial.print("SSID: "); Serial.println(WiFi.SSID());
    Serial.print("IP: "); Serial.println(ipAddress);
    Serial.print("Passerelle: "); Serial.println(WiFi.gatewayIP());
    Serial.print("DNS: "); Serial.println(WiFi.dnsIP());
    Serial.print("RSSI: "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
    // MDNS (retardé)
    if (MDNS.begin("utb")) Serial.println("[mDNS] utb.local prêt");

    // Clignotement NeoPixel en vert pour indiquer la connexion
    for (int i = 0; i < 3; i++) {
      pixels.setPixelColor(0, pixels.Color(0, 150, 0));
      pixels.show();
      delay(150);
      pixels.clear();
      pixels.show();
      delay(150);
    }
  } else {
    Serial.println("[WARN] WiFi non connecté -> AP secours");
    WiFi.mode(WIFI_AP);
    bool ap = WiFi.softAP("UTB_AP", "utb12345");
    delay(200);
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("[AP] Etat: "); Serial.println(ap ? "OK" : "ECHEC");
    Serial.print("[AP] SSID: UTB_AP  Pass: utb12345  IP: "); Serial.println(apIP);
    ipAddress = apIP.toString();
  }
  Serial.println("===================\n");
  
  // Affichage dans le moniteur série
  Serial.println("\n========================================");
  if (connected) {
    Serial.println("WiFi connecté !");
    Serial.print("SSID: "); Serial.println(WiFi.SSID());
    Serial.print("Adresse IP: "); Serial.println(ipAddress);
    Serial.print("Passerelle: "); Serial.println(WiFi.gatewayIP());
    Serial.print("DNS: "); Serial.println(WiFi.dnsIP());
    Serial.print("Puissance signal (RSSI): "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
    Serial.print("Dashboard: http://"); Serial.println(ipAddress);
    Serial.println("mDNS (si supporté): http://utb.local");
  } else {
    Serial.println("Échec de connexion WiFi - Mode OFFLINE");
  }
  Serial.println("========================================\n");

  // Web Server (routes)
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = index_html;
    html.replace("%PROJECT_NAME%", PROJECT_NAME); // Injection auto
    html.replace("%VERSION%", PROJECT_VERSION);   // Injection auto
    html.replace("%PIN_A%", String(PIN_TEST_ANALOG));
    html.replace("%PIN_D%", String(PIN_TEST_DIGITAL));
    html.replace("%PIN_DHT%", String(PIN_DHT));
    request->send(200, "text/html", html);
  });
  
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"analog\":" + String(valAnalog) + ",\"digital\":" + String(valDigital) + "}");
  });
  server.on("/env", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{\"temp\":" + String(temp) + ",\"hum\":" + String(hum) + "}");
  });
  server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("type")) {
      String type = request->getParam("type")->value();
      if (type == "buzzer") {
        digitalWrite(PIN_BUZZER, HIGH); delay(100); digitalWrite(PIN_BUZZER, LOW); delay(50);
        digitalWrite(PIN_BUZZER, HIGH); delay(200); digitalWrite(PIN_BUZZER, LOW);
      } else if (type == "led") {
        pixels.setPixelColor(0, pixels.Color(255, 0, 0)); pixels.show(); delay(300);
        pixels.setPixelColor(0, pixels.Color(0, 255, 0)); pixels.show(); delay(300);
        pixels.setPixelColor(0, pixels.Color(0, 0, 255)); pixels.show(); delay(300);
        pixels.clear(); pixels.show();
      }
    }
    request->send(200, "text/plain", "OK");
  });

  // Démarrer le serveur uniquement après la configuration
  server.begin();
  drawBaseInterface();
  tft.setTextColor(C_WHITE); tft.fillRect(25, 45, 200, 12, C_BLACK); tft.setCursor(25, 45); tft.print(ipAddress);
}

void loop() {
  // Surveille l'état WiFi et notifie en cas de changement
  static uint8_t lastStatus = WL_IDLE_STATUS;
  uint8_t currentStatus = wifiMulti.run();
  if (currentStatus != lastStatus) {
    if (currentStatus == WL_CONNECTED) {
      ipAddress = WiFi.localIP().toString();
      Serial.print("[WiFi] Connecté - IP: "); Serial.println(ipAddress);
      Serial.print("[HTTP] Dashboard: http://"); Serial.println(ipAddress);
      // Clignote vert une fois à la reconnexion
      pixels.setPixelColor(0, pixels.Color(0, 150, 0)); pixels.show(); delay(200);
      pixels.clear(); pixels.show();
      // Rafraîchir l'IP sur le TFT
      tft.setTextColor(C_WHITE); tft.fillRect(25, 45, 200, 12, C_BLACK); tft.setCursor(25, 45); tft.print(ipAddress);
      // Tentative mDNS après reconnexion
      MDNS.end();
      if (MDNS.begin("utb")) Serial.println("[mDNS] Reconnect OK");
    } else {
      Serial.println("[WiFi] Déconnecté");
    }
    lastStatus = currentStatus;
  }

  // Journal périodique: réaffiche l'IP toutes les 10s si connecté
  static unsigned long lastNetLog = 0;
  if (currentStatus == WL_CONNECTED && millis() - lastNetLog > 10000) {
    Serial.print("[WiFi] IP actuelle: "); Serial.println(WiFi.localIP());
    lastNetLog = millis();
  }

  long sum = 0; for(int i=0; i<10; i++){ sum += analogRead(PIN_TEST_ANALOG); delay(1); }
  valAnalog = sum / 10;
  valDigital = digitalRead(PIN_TEST_DIGITAL);

  static unsigned long lastDHT = 0;
  if (millis() - lastDHT > INTERVAL_DHT) {
    float t = dht.readTemperature(); float h = dht.readHumidity();
    if (!isnan(t)) temp = t; if (!isnan(h)) hum = h;
    updateTFT_Env(); lastDHT = millis();
  }

  static unsigned long lastDisplay = 0;
  if (millis() - lastDisplay > INTERVAL_DISPLAY) { updateTFT(); lastDisplay = millis(); }
}