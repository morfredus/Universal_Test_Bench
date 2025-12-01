# [1.2.2] - 2025-11-30
### Added
- Detailed WiFi diagnostics (scan results, MAC, event logging) and periodic IP logging.
- mDNS responder (`utb.local`).
- Fallback Access Point (`UTB_AP`) when STA connection fails.
### Changed
- Improved initial and reconnect Serial output for clearer IP visibility.
 
# [1.3.0] - 2025-12-01
### Changed
- Full rewrite of `src/main.cpp` to ensure reliable WiFi connection, periodic IP display in Serial Monitor, and a functional web dashboard.
### Added
- Simple, robust web UI with routes `/`, `/data`, `/env`, `/action`.
- Automatic fallback AP (`UTB_AP` / `utb12345`) when STA fails.

# [1.3.1] - 2025-12-01
### Fixed
- Web UI sends valid JSON (uses `null` instead of `NaN`) so the network monitor displays data reliably.
- Added UTF-8 meta and response headers so accents render correctly in the web UI.

# [1.3.2] - 2025-12-01
### Added
- Serial Monitor now prints detailed WiFi status (SSID, IP, RSSI, DNS, mode) on connect and periodically.
- PSRAM detection added to boot logs (presence and size), plus flash chip size/speed.

# [1.3.3] - 2025-12-01
### Changed
- Serial logging policy: now silent during normal operation; prints only on (re)connection and disconnection events. Avoids repeated IP printing.
# Changelog

All notable changes to the "Universal Test Bench" project will be documented in this file.

## [1.2.0] - 2025-11-30
### Added
- Network information display in Serial Monitor (IP, Gateway, DNS, SSID, RSSI).
# [1.2.1] - 2025-11-30
### Fixed
- More robust WiFi connection handling and clearer Serial Monitor logs (IP and Web URL shown on connect; periodic IP status).
- Documentation updated to remove kit-specific references; sensor guides made universal.


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