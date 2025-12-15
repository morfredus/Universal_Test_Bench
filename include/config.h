#ifndef CONFIG_H
#define CONFIG_H

#include "secrets.h"

// ------------------------------------
// GPS (UART1)
// ------------------------------------
// Configuration de la communication série (UART1) avec le module GPS.
#define PIN_GPS_RXD           18  // GPS TX (vers RXD de l'ESP32). **Si 5V, Diviseur de tension OBLIGATOIRE.**
#define PIN_GPS_TXD           17  // GPS RX (vers TXD de l'ESP32). Câblage direct.
#define PIN_GPS_PPS           8   // GPS PPS, Câblage direct.

// ------------------------------------
// TFT ST7789 (SPI)
// ------------------------------------
// Configuration de l'écran couleur (TFT) utilisant le protocole SPI.
#define PIN_TFT_MOSI              11  // SPI MOSI (Master Out Slave In) : Broche de données vers l'écran. Câblage direct.
#define PIN_TFT_SCLK              12  // SPI Clock (Horloge) : Broche de synchronisation. Câblage direct.
#define PIN_TFT_CS                10  // Chip Select : Broche pour activer spécifiquement cet écran. Câblage direct.
#define PIN_TFT_DC                9   // Data/Command : Broche pour indiquer si les données sont des pixels ou une commande. Câblage direct.
#define PIN_TFT_RST               13  // Reset écran. Pull-up/down selon librairie.
#define PIN_TFT_BL                7   // Backlight (Rétroéclairage) : Souvent utilisé avec le PWM pour ajuster la luminosité.

// ------------------------------------
// I2C
// ------------------------------------
// Configuration du bus de communication I2C.
#define PIN_I2C_SDA       15  // SDA (Serial Data Line). **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.**
#define PIN_I2C_SCL       16  // SCL (Serial Clock Line). **Pull-Up 4.7kO vers 3.3V OBLIGATOIRE.**

// ------------------------------------
// DÉTAIL TECHNIQUE : Le Bus I2C (Inter-Integrated Circuit)
// ------------------------------------
// OBJECTIF : Permettre à plusieurs capteurs/périphériques de communiquer avec l'ESP32 en utilisant seulement deux broches (SDA et SCL).
// L'IMPÉRATIF PULL-UP : Le bus I2C utilise des sorties à drain ouvert (open-drain). Des résistances externes (Pull-Up, typiquement 4.7 kOhm vers 3.3V) sont nécessaires pour tirer le signal vers le haut.
// 
// ------------------------------------


// ------------------------------------
// LED RGB
// ------------------------------------
#define PIN_LED_RED 21 // Rouge. Résistance série **220O - 470O**.
#define PIN_LED_GREEN 41 // Vert. Résistance série **220O - 470O**.
#define PIN_LED_BLUE 42 // Bleu. Résistance série **220O - 470O**.
#define PIN_NEOPIXEL 48 // NeoPixel Intégrée. Data (Réservée).

// Configuration NeoPixel
#define NUM_PIXELS 1        // Nombre de pixels NeoPixel (1 pour le pixel intégré)
#define NEO_BRIGHTNESS 50   // Luminosité (0-255)

// ------------------------------------
// Boutons
// ------------------------------------
#define PIN_BUTTON_BOOT       0   // Bouton BOOT (strap). Réservé au boot.
#define PIN_BUTTON_1          38  // Bouton vers GND. Utiliser **Pull-Up interne**.
#define PIN_BUTTON_2          39  // Bouton vers GND. Utiliser **Pull-Up interne**.

// ------------------------------------
// Capteurs & Sorties
// ------------------------------------
#define PIN_PWM       20  // PWM générique. Résistance série **220O - 470O** si LED.
#define PIN_BUZZER    6   // Buzzer. Transistor + **Résistance base 1–10kO**.
#define PIN_DHT       5   // DHT. **Pull-Up 10kO vers 3.3V**.
#define DHT_TYPE      DHT22 // Type de capteur DHT (DHT11 ou DHT22)
#define PIN_MOTION_SENSOR 46 // PIR. Câblage direct. **(Potentiel JTAG - attention au boot).**
#define PIN_LIGHT_SENSOR 4  // LDR. **Diviseur ~10kO**.

// Pins de test (lecture analogique et digitale)
#define PIN_TEST_ANALOG   1  // Pin pour test analogique (GPIO1, compatible ADC)
#define PIN_TEST_DIGITAL  14 // Pin pour test digital (GPIO14, libre)

// ------------------------------------
// Capteurs de Distance
// ------------------------------------
#define PIN_DISTANCE_TRIG 2  // HC-SR04 TRIG. Câblage direct.
#define PIN_DISTANCE_ECHO 35 // HC-SR04 ECHO. **Si 5V, ajouter Diviseur de tension OBLIGATOIRE.**

// Couleurs UI
#define C_BLACK   0x0000
#define C_WHITE   0xFFFF
#define C_BLUE    0x001F
#define C_GREEN   0x07E0
#define C_RED     0xF800
#define C_YELLOW  0xFFE0
#define C_ORANGE  0xFD20
#define C_DKGREY  0x2104
#define C_CYAN    0x07FF

#endif