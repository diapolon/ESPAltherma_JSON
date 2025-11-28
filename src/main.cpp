// ===== main.cpp =====
#include <Arduino.h>
#include "SerialManager.h"
#include "DaikinProtocol.h"
#include "JsonBuilder.h"
#include "config.h"
#include <WiFi.h>
#include <WebServer.h>
#include <map>
#include <vector>

HardwareSerial MySerial(SERIAL_PORT);
DaikinProtocol* daikinPtr = nullptr;
JsonBuilder jsonBuilder;
WebServer server(80);

uint32_t lastQuery = 0;
std::map<uint8_t, std::vector<uint8_t>> lastResults;

void handleRoot() {
    String outJson;
    if(jsonBuilder.buildFromDaikinMap(lastResults, outJson)){
        server.send(200, "application/json", outJson);
    } else {
        server.send(500, "text/plain", "No data");
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
    server.begin();

    auto& sm = SerialManager::instance();
    MySerial.begin(SERIAL_BAUD, SERIAL_8E1, SERIAL_RX_PIN, SERIAL_TX_PIN);
    sm.begin(MySerial, SERIAL_BAUD);

    static DaikinProtocol daikin(sm);
    daikinPtr = &daikin;
    daikin.begin();
}

void loop() {
    SerialManager::instance().loop();

    if(millis() - lastQuery >= DAIKIN_QUERY_INTERVAL){
        if(daikinPtr){
            std::vector<uint8_t> regs = {0x50, 0x56}; // esempio multipli registri
            daikinPtr->queryRegisters(regs, lastResults, 'I');
        }
        lastQuery = millis();
    }

    server.handleClient();

    delay(1);
}
