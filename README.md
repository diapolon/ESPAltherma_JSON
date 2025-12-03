# ESPAltherma_JSON — rewrite


This is a full rewrite of the ESPAltherma project, structured for PlatformIO and ESP32.
It support only Esp32-S3 elecrow


ToDo:
- Show on display readed values

Key features:
- modular code (SerialManager, SerialManager, WebServerManager)
- ArduinoJson for low-memory JSON
- AsyncWebServer endpoint `/getData` returning the latest JSON snapshot
- Elegant OTA for simply update


## How to use
1. Open the folder in PlatformIO (VSCode)
2. Customize `src/Globals.h` for WiFi provisioning and other settings
3. Build and upload to an ESP32
4. Call `http://<device_ip>/` for homepage
5. Call `http://<device_ip>/getData` to get JSON
6. Call `http://<device_ip>/update` for Elegant OTA
7. Call `http://<device_ip>/reboot` for rebooting
