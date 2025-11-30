#ifndef CONFIG_H
#define CONFIG_H

#include "secrets.h"

// Le PROJECT_NAME et PROJECT_VERSION sont définis dans platformio.ini

// ================= ECRAN TFT ST7789 =================
#define TFT_WIDTH           240
#define TFT_HEIGHT          240
#define TFT_ROTATION        2    
#define TFT_MOSI            11
#define TFT_SCLK            12
#define TFT_CS              10
#define TFT_DC              6
#define TFT_RST             7
#define TFT_BL              15   

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

// ================= BANC DE TEST (PINS UNIVERSELS) =================
#define PIN_TEST_ANALOG     14  
#define PIN_TEST_DIGITAL    13  

// ================= CAPTEURS SPECIAUX =================
// DHT Sensor
#define PIN_DHT             16
#define DHT_TYPE            DHT11  

// Buzzer (Externe)
#define PIN_BUZZER          17

// NeoPixel (Integré ESP32-S3 DevKitC-1)
#define PIN_NEOPIXEL        48
#define NUM_PIXELS          1      
#define NEO_BRIGHTNESS      20     // Faible luminosité par défaut

// ================= TIMERS (ms) =================
#define INTERVAL_DISPLAY    200
#define INTERVAL_DHT        2000   

#endif