# Quick Sensor Guide

## 🔴 Universal Input Pins

### 1. On GPIO 13 (Digital Input)
*Connect sensors that provide an "ON/OFF" (Binary) state here.*
* Buttons, Tilt Switches, Reed Switches, Vibration Sensors.
* IR Obstacle, Photo Interrupter, PIR Motion Sensor.
* Sound Sensors (Microphone D0).

### 2. On GPIO 14 (Analog Input)
*Connect sensors that provide a variable value (Intensity) here.*
* Joystick (X or Y), Potentiometers.
* LDR (Light), Water Level, Soil Moisture.
* Analog Temperature (Thermistor), Linear Hall.

---

## 🟢 Special Modules

For common modules, use the following dedicated pins:

### 1. Temperature & Humidity (DHT11)
* **Pin:** Connect Signal to **GPIO 16**.
* **Usage:** Values appear at the bottom of the TFT screen and in the "Env" section of the Web Interface.

### 2. Buzzer (Active or Passive)
* **Pin:** Connect Signal (I/O) to **GPIO 17**.
* **Usage:** Use the **"TEST BUZZER"** button on the Web Interface to trigger a sound pattern.

### 3. RGB LED (NeoPixel)
* **Pin:** No wiring needed! Uses the **Onboard LED (GPIO 48)**.
* **Usage:** Use the **"TEST RGB LED"** button on the Web Interface to cycle through colors.