# Changelog

All notable changes to the "Universal Test Bench" project will be documented in this file.

## [1.2.0] - 2025-11-30
### Added
- Network information display in Serial Monitor (IP, Gateway, DNS, SSID, RSSI).

### Changed
- Removed unused dependencies (UniversalTelegramBot, ArduinoJson) to eliminate deprecation warnings.
- Code optimized for latest library versions.

## [1.1.0] - 2023-11-26
### Added
- **Hardware Support:**
    - Support for DHT11/DHT22 sensors (Temp/Humidity) on GPIO 16.
    - Support for Buzzer (Active/Passive) output on GPIO 17.
    - Support for Built-in ESP32-S3 NeoPixel (RGB LED) on GPIO 48.
- **Interface:**
    - New section on TFT display for Environment data (Temp/Hum).
    - Added Action buttons on Web Interface to trigger Buzzer and LED tests.
- **System:**
    - Project Name and Version are now defined in `platformio.ini`.

## [1.0.0] - 2023-11-26
### Added
- Initial release.
- Analog/Digital universal testing.
- Web Server & TFT Display integration.