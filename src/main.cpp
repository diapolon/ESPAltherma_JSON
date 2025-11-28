// ===== main.cpp aggiornato con configurazione registri =====
#include <Arduino.h>
#include "SerialManager.h"
#include "DaikinProtocol.h"
#include "JsonBuilder.h"
#include "config.h"
#include "daikin_registers_config.h"
#include <WiFi.h>
#include <WebServer.h>
#include <map>
#include <vector>
#include <ArduinoJson.h>

#ifdef M5STICKC_PLUS
#include <M5StickC.h>
#endif

#ifdef ESP32_S3_TERMINAL
#include <LovyanGFX.hpp>
class LGFX_ESP32_S3_Terminal : public LGFX_Device {
public:
  LGFX_ESP32_S3_Terminal(void) {
    auto cfg = _panel_cfg;
    cfg.pin_cs   = DISPLAY_CS;
    cfg.pin_rst  = DISPLAY_RST;
    cfg.pin_dc   = DISPLAY_DC;
    cfg.panel_width  = 480;
    cfg.panel_height = 320;
    cfg.offset_x    = 0;
    cfg.offset_y    = 0;
    cfg.readable    = true;
    cfg.bus_shared  = false;
    _panel_cfg = cfg;

    auto bus_cfg = _bus_cfg;
    bus_cfg.pin_wr   = DISPLAY_WR;
    bus_cfg.pin_data = {DISPLAY_DB0, DISPLAY_DB1, DISPLAY_DB2, DISPLAY_DB3,
                        DISPLAY_DB4, DISPLAY_DB5, DISPLAY_DB6, DISPLAY_DB7,
                        DISPLAY_DB8, DISPLAY_DB9, DISPLAY_DB10, DISPLAY_DB11,
                        DISPLAY_DB12, DISPLAY_DB13, DISPLAY_DB14, DISPLAY_DB15};
    bus_cfg.freq_write = 40000000; // 40MHz
    bus_cfg.freq_read  = 16000000;
    bus_cfg.bus_width  = 16;
    _bus_cfg = bus_cfg;

    setBus(&_bus_cfg);
    setPanel(&_panel_cfg);
  }
};

LGFX_ESP32_S3_Terminal lcd;
#endif

#define DEBUG 1

HardwareSerial MySerial(SERIAL_PORT);
DaikinProtocol* daikinPtr = nullptr;
JsonBuilder jsonBuilder;
WebServer server(80);

uint32_t lastQuery = 0;
std::map<uint8_t, std::vector<uint8_t>> lastResults;

void handleRoot() {
    String html = "<html><head><title>Daikin ESP32</title></head><body>";
    html += "<h1>Daikin ESP32 Status</h1>";
    html += "<p>JSON available at <a href='/json'>/json</a></p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleJson() {
    String outJson;
    if(jsonBuilder.buildFromDaikinMap(lastResults, outJson)){
        server.send(200, "application/json", outJson);
    } else {
        server.send(500, "application/json", "{\"error\":\"No data\"}");
    }
}

void setupDisplay() {
    #ifdef M5STICKC_PLUS
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    #endif    
    #ifdef ESP32_S3_TERMINAL
    lcd.begin();
    lcd.setRotation(DISPLAY_ROTATION);
    lcd.fillScreen(TFT_BLACK);
    lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    lcd.setTextSize(2);
    #endif
}

void updateDisplay() {
    #ifdef M5STICKC_PLUS
    M5.Lcd.fillScreen(BLACK);
    int y = 0;
    if(lastResults.count(0x50)){
        float temp = ((uint16_t)lastResults[0x50][3]<<8 | lastResults[0x50][4])/10.0f;
        M5.Lcd.setCursor(0,y); M5.Lcd.printf("Temp: %.1f C", temp); y += 20;
    }
    if(lastResults.count(0x56)){
        M5.Lcd.setCursor(0,y); M5.Lcd.printf("Mode: %d", lastResults[0x56][3]); y += 20;
    }
    if(lastResults.count(0x5A)){
        M5.Lcd.setCursor(0,y); M5.Lcd.printf("Fan: %d", lastResults[0x5A][3]); y += 20;
    }
    if(lastResults.count(0x5C)){
        uint16_t power = (uint16_t)lastResults[0x5C][3]<<8 | lastResults[0x5C][4];
        M5.Lcd.setCursor(0,y); M5.Lcd.printf("Power: %d W", power); y += 20;
    }
    #endif
    #ifdef ESP32_S3_TERMINAL
    lcd.fillScreen(TFT_BLACK);
    int y_s3 = 0;
    if(lastResults.count(0x50)){
        float temp = ((uint16_t)lastResults[0x50][3]<<8 | lastResults[0x50][4])/10.0f;
        lcd.setCursor(0,y_s3); lcd.printf("Temp: %.1f C", temp); y_s3 += 20;
    }
    if(lastResults.count(0x56)){
        lcd.setCursor(0,y_s3); lcd.printf("Mode: %d", lastResults[0x56][3]); y_s3 += 20;
    }
    if(lastResults.count(0x5A)){
        lcd.setCursor(0,y_s3); lcd.printf("Fan: %d", lastResults[0x5A][3]); y_s3 += 20;
    }
    if(lastResults.count(0x5C)){
        uint16_t power = (uint16_t)lastResults[0x5C][3]<<8 | lastResults[0x5C][4];
        lcd.setCursor(0,y_s3); lcd.printf("Power: %d W", power); y_s3 += 20;
    }
    #endif
}

void setup() {
    Serial.begin(115200);
    delay(100);

    setupDisplay();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected");

    server.on("/", handleRoot);
    server.on("/json", handleJson);
    server.begin();

    auto& sm = SerialManager::instance();
    MySerial.begin(SERIAL_BAUD, SERIAL_8E1, SERIAL_RX_PIN, SERIAL_TX_PIN);
    sm.begin(MySerial, SERIAL_BAUD);

    static DaikinProtocol daikin(sm);
    daikinPtr = &daikin;
    daikin.begin();

    #if DEBUG
        Serial.println("Setup complete, starting loop...");
    #endif

}

void loop() {
    SerialManager::instance().loop();

    if(millis() - lastQuery >= DAIKIN_QUERY_INTERVAL){
        if(daikinPtr){
            std::vector<uint8_t> regs;
            for(auto& r : daikinRegisters){
                if(r.enabled) regs.push_back(r.regID);
            }
            if(!regs.empty()){
                bool success = daikinPtr->queryRegisters(regs, lastResults, 'I');
                #if DEBUG
                    if(success) Serial.println("Registers read successfully.");
                    else Serial.println("Error reading registers.");
                #endif
            }
        }
        lastQuery = millis();
        updateDisplay();
    }

    server.handleClient();
    delay(1);
}
