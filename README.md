# Universal Test Bench 🛠️

**Version:** 1.3.2  
**Board:** ESP32-S3 DevKitC-1 (N16R8)  
**Framework:** Arduino (via PlatformIO)

## 📖 Overview
The **Universal Test Bench** is a diagnostic tool to quickly test common digital and analog sensors, and specific modules like DHT and Buzzers, without rewriting code. It visualizes data in real-time on a TFT screen and a responsive Web Interface.

## ✨ Features (v1.3.1)
* **WiFi Diagnostics:** MAC, event logs, periodic IP print.
* **Fallback AP:** Creates `UTB_AP` if normal connection fails.
* **mDNS:** Access via `http://utb.local` when supported.
* **Universal Testing:** Dedicated pins for Analog and Digital sensors.
* **Special Support:** Built-in drivers for DHT11/22, Buzzer, and the Onboard NeoPixel.
* **Real-time Display:**
    * **TFT Screen (ST7789):** Shows raw values, voltage, logic states, and environment data (Temp/Hum).
    * **Web Dashboard:** Responsive interface with action buttons; UTF-8 accents supported; network monitor reliably shows data.
    * **Serial Monitor:** Detailed WiFi info on startup and periodically (IP, SSID, RSSI, DNS, mode). PSRAM detection logged.
* **WiFi Multi:** Automatically connects to known networks defined in `secrets.h`.

## 🚀 Installation
1.  Clone or open this repository in **VS Code** with **PlatformIO**.
2.  Create a file named `include/secrets.h` with your WiFi credentials.
3.  Upload the code.
4.  IP address appears on TFT and in the Serial Monitor.

## ⚙️ Configuration
* **WiFi:** Edit `include/secrets.h`.
* **Project Name/Version:** Defined directly in `platformio.ini`.
* **Pinout:** Defined in `include/config.h`.
