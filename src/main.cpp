/**
 * MAIN.CPP - Universal Test Bench (Rewritten for reliability)
 * Version définie dans platformio.ini
 */
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

// Objets globaux minimaux pour robustesse
WiFiMulti wifiMulti;
AsyncWebServer server(80);
DHT dht(PIN_DHT, DHT_TYPE);
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Etats capteurs
int valAnalog = 0;
int valDigital = 0;
float temp = NAN;
float hum = NAN;
String ipAddress = "OFFLINE";
static bool staIpPrinted=false;

// Gestion des événements WiFi pour logs uniquement sur (re)connexion/déconnexion
static void onWifiEvent(WiFiEvent_t event){
  switch(event){
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println("[WiFi] STA associée au SSID");
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      ipAddress = WiFi.localIP().toString();
      Serial.printf("[WiFi] IP: %s | SSID: %s | RSSI: %d dBm\n", WiFi.localIP().toString().c_str(), WiFi.SSID().c_str(), WiFi.RSSI());
      staIpPrinted=true;
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("[WiFi] Déconnexion STA");
      staIpPrinted=false;
      break;
    default:
      break;
  }
}

// Page HTML (simple et robuste)
const char INDEX_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>%PROJECT_NAME% v%VERSION%</title>
<style>body{font-family:Segoe UI,Arial;margin:0;background:#111;color:#eee}header{background:#222;padding:12px}h1{margin:0;font-size:18px}
.wrap{display:flex;flex-wrap:wrap;gap:16px;padding:12px;justify-content:center}
.card{background:#1c1c1c;border:1px solid #333;border-radius:10px;padding:14px;min-width:240px}
.value{font-size:32px;font-weight:bold;color:#0df}.label{color:#aaa;text-transform:uppercase;font-size:12px}
.btn{background:#0a84ff;color:#fff;border:none;padding:10px 14px;border-radius:6px;cursor:pointer}
.btn.danger{background:#ff3b30}
</style>
<script>
async function refresh(){
  try{
    const d=await fetch('/data').then(r=>r.json());
    const e=await fetch('/env').then(r=>r.json());
    document.getElementById('an').textContent=d.analog;
    document.getElementById('volt').textContent=(d.analog*3.3/4095).toFixed(2)+' V';
    document.getElementById('dg').textContent=(d.digital? 'HIGH':'LOW');
    document.getElementById('tp').textContent=(isNaN(e.temp)?'--':e.temp+' °C');
    document.getElementById('hm').textContent=(isNaN(e.hum)?'--':e.hum+' %');
  }catch(err){}
}
setInterval(refresh,500);
function act(t){fetch('/action?type='+t).catch(()=>{});}
</script></head>
<body><header><h1>%PROJECT_NAME% — v%VERSION% — IP: %IP%</h1></header>
<div class="wrap">
  <div class="card"><div class="label">Analog (GPIO %PIN_A%)</div><div id="an" class="value">0</div><div id="volt" class="label">0.00 V</div></div>
  <div class="card"><div class="label">Digital (GPIO %PIN_D%)</div><div id="dg" class="value">LOW</div></div>
  <div class="card"><div class="label">Environment (GPIO %PIN_DHT%)</div><div id="tp" class="value">--</div><div id="hm" class="label">--</div></div>
  <div class="card"><div class="label">Actions</div>
    <button class="btn" onclick="act('buzzer')">Buzzer</button>
    <button class="btn danger" onclick="act('led')">RGB LED</button>
  </div>
</div></body></html>
)HTML";

// Helpers
static void blinkGreen(uint8_t times=3, uint16_t ms=150){
  for(uint8_t i=0;i<times;i++){ pixels.setPixelColor(0,pixels.Color(0,150,0)); pixels.show(); delay(ms); pixels.clear(); pixels.show(); delay(ms);} }

void printNetInfo(){
  Serial.println("\n=== Réseau ===");
  Serial.print("Mode: "); Serial.println(WiFi.getMode()==WIFI_STA?"STA":"AP");
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("IP: "); Serial.println(WiFi.localIP());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: "); Serial.println(WiFi.dnsIP());
  Serial.print("RSSI: "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
  Serial.println("==============\n");
}

void startServer(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* req){
    String html=INDEX_HTML;
    html.replace("%PROJECT_NAME%", PROJECT_NAME);
    html.replace("%VERSION%", PROJECT_VERSION);
    html.replace("%IP%", ipAddress);
    html.replace("%PIN_A%", String(PIN_TEST_ANALOG));
    html.replace("%PIN_D%", String(PIN_TEST_DIGITAL));
    html.replace("%PIN_DHT%", String(PIN_DHT));
    req->send(200,"text/html; charset=utf-8",html);
  });
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest* req){
    String json="{\"analog\":"+String(valAnalog)+",\"digital\":"+String(valDigital==HIGH?1:0)+"}";
    req->send(200,"application/json; charset=utf-8",json);
  });
  server.on("/env", HTTP_GET, [](AsyncWebServerRequest* req){
    // JSON ne supporte pas NaN : utiliser null quand valeur inconnue
    String json = "{\"temp\":";
    if (isnan(temp)) json += "null"; else json += String(temp);
    json += ",\"hum\":";
    if (isnan(hum)) json += "null"; else json += String(hum);
    json += "}";
    req->send(200,"application/json; charset=utf-8",json);
  });
  server.on("/action", HTTP_GET, [](AsyncWebServerRequest* req){
    if(req->hasParam("type")){
      String t=req->getParam("type")->value();
      if(t=="buzzer"){ digitalWrite(PIN_BUZZER,HIGH); delay(120); digitalWrite(PIN_BUZZER,LOW); }
      else if(t=="led"){ pixels.setPixelColor(0,pixels.Color(255,0,0)); pixels.show(); delay(200); pixels.setPixelColor(0,pixels.Color(0,255,0)); pixels.show(); delay(200); pixels.setPixelColor(0,pixels.Color(0,0,255)); pixels.show(); delay(200); pixels.clear(); pixels.show(); }
    }
    req->send(200,"text/plain","OK");
  });
  server.begin();
}

void setup(){
  Serial.begin(115200); delay(120);
  Serial.println("\n[BOOT] Universal Test Bench");
  Serial.printf("[VERSION] %s\n", PROJECT_VERSION);
  Serial.printf("[CPU] Freq: %d MHz\n", getCpuFrequencyMhz());
  // PSRAM & Flash
  bool psram = psramFound();
  Serial.printf("[PSRAM] Present: %s | Size: %u bytes\n", psram?"YES":"NO", ESP.getPsramSize());
  Serial.printf("[Flash] Size: %u bytes | Speed: %u Hz\n", ESP.getFlashChipSize(), ESP.getFlashChipSpeed());

  // Pins & périphériques
  pinMode(PIN_TEST_ANALOG, INPUT);
  pinMode(PIN_TEST_DIGITAL, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT); digitalWrite(PIN_BUZZER, LOW);
  pixels.begin(); pixels.setBrightness(NEO_BRIGHTNESS); pixels.clear(); pixels.show();
  dht.begin();

  // WiFi STA
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(onWifiEvent);
  wifiMulti.addAP(ssid_1, password_1);
  wifiMulti.addAP(ssid_2, password_2);

  Serial.printf("[WiFi] Mode: STA | MAC: %s\n", WiFi.macAddress().c_str());
  Serial.println("[WiFi] Connexion en cours (WiFiMulti)...");
  int attempts=0; while(wifiMulti.run()!=WL_CONNECTED && attempts<40){ attempts++; if(attempts%5==1) Serial.printf("[WiFi] Tentative %d...\n", attempts); delay(300);} 

  if(WiFi.status()==WL_CONNECTED){
    ipAddress=WiFi.localIP().toString();
    Serial.print("[WiFi] Connecté - IP: "); Serial.println(ipAddress);
    printNetInfo();
    Serial.printf("[WiFi] SSID: %s | RSSI: %d dBm\n", WiFi.SSID().c_str(), WiFi.RSSI());
    blinkGreen(3,120);
    startServer();
    Serial.print("[HTTP] Dashboard: http://"); Serial.println(ipAddress);
  } else {
    Serial.println("[WiFi] Echec STA -> AP de secours");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("UTB_AP","utb12345"); delay(150);
    ipAddress=WiFi.softAPIP().toString();
    Serial.print("[AP] SSID: UTB_AP  Pass: utb12345  IP: "); Serial.println(ipAddress);
    Serial.println("[AP] Accès Dashboard via IP ci-dessus.");
    startServer();
    Serial.print("[HTTP] Dashboard: http://"); Serial.println(ipAddress);
  }
}

void loop(){
  // Lecture capteurs
  long sum=0; for(int i=0;i<8;i++){ sum+=analogRead(PIN_TEST_ANALOG); delay(1);} valAnalog=sum/8;
  valDigital=digitalRead(PIN_TEST_DIGITAL);

  static unsigned long lastEnv=0; if(millis()-lastEnv>2000){ float t=dht.readTemperature(); float h=dht.readHumidity(); if(!isnan(t)) temp=t; if(!isnan(h)) hum=h; lastEnv=millis(); }

  // Journal connexion unique: n'afficher l'IP qu'une seule fois en STA
  static bool staIpPrinted=false;
  if(!staIpPrinted && WiFi.getMode()==WIFI_STA && WiFi.status()==WL_CONNECTED){
    Serial.printf("[WiFi] IP: %s | SSID: %s | RSSI: %d dBm\n", WiFi.localIP().toString().c_str(), WiFi.SSID().c_str(), WiFi.RSSI());
    staIpPrinted=true;
  }
  // En mode AP, on peut afficher ponctuellement le nombre de clients (optionnel)
  // Désactivé pour éviter le spam; décommentez si nécessaire.
  // static unsigned long lastApLog=0; if(WiFi.getMode()==WIFI_AP && millis()-lastApLog>15000){
  //   Serial.printf("[AP] Clients: %d\n", WiFi.softAPgetStationNum());
  //   lastApLog=millis();
  // }
}
 