# Universal Test Bench 🛠️

**Version:** 1.2.0  
**Board:** ESP32-S3 DevKitC-1 (N16R8)  
**Framework:** Arduino (via PlatformIO)

## 📖 Overview
The **Universal Test Bench** is a diagnostic tool designed for the "45-in-1 Sensor Kit". It allows you to instantly test digital and analog sensors, as well as specific modules like DHT and Buzzers, without rewriting code. It visualizes data in real-time on a TFT screen and a responsive Web Interface.

## ✨ Features (v1.2.0)
* **Universal Testing:** Dedicated pins for Analog and Digital sensors.
* **Special Support:** Built-in drivers for DHT11/22, Buzzer, and the Onboard NeoPixel.
* **Real-time Display:**
    * **TFT Screen (ST7789):** Shows raw values, voltage, logic states, and environment data (Temp/Hum).
    * **Web Dashboard:** Responsive interface with action buttons to trigger output tests.
    * **Serial Monitor:** Complete network information display on startup (IP, SSID, RSSI, DNS).
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
