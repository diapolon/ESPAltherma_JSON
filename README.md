# ESPAltherma_JSON — rewrite


This is a full rewrite of the ESPAltherma_JSON project, structured for PlatformIO and ESP32.


Key features:
- modular code (SerialManager, DaikinProtocol, JsonBuilder, WebServerManager)
- FreeRTOS task for periodic polling
- ArduinoJson (StaticJsonDocument) for low-memory JSON
- AsyncWebServer endpoint `/getData` returning the latest JSON snapshot


## How to use
1. Open the folder in PlatformIO (VSCode)
2. Customize `include/config.h` for UART pins, WiFi provisioning and other settings
3. Build and upload to an ESP32
4. Call `http://<device_ip>/getData` to get JSON