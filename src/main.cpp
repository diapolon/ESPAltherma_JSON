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

void setup() {
    Serial.begin(115200);
    delay(100);

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
    }

    server.handleClient();
    delay(1);
}
