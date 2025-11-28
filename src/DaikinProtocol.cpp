#include "DaikinProtocol.h"
#include "SerialManager.h"
#include <Arduino.h>

DaikinProtocol::DaikinProtocol(SerialManager& sm)
: serialMgr(sm)
{}

void DaikinProtocol::begin() {
    // registriamo il callback che verrà chiamato quando arriva un frame completo
    serialMgr.onFrame([this](const uint8_t* data, size_t len){
        this->parseFrame(data, len);
    });
}

void DaikinProtocol::loop() {
    // se serve fare attività periodiche, mettile qui
}

uint8_t DaikinProtocol::computeCRC(const uint8_t* data, size_t len) {
    uint8_t sum = 0;
    for (size_t i = 0; i < len; ++i) sum += data[i];
    return ~sum;
}

void DaikinProtocol::parseFrame(const uint8_t* data, size_t len) {
    if (len < 3) return; // troppo corto

    // CRC: ultimo byte
    uint8_t calc = computeCRC(data, len - 1);
    if (calc != data[len - 1]) {
        Serial.println("CRC NON valido (DaikinProtocol)");
        return;
    }

    // Dispatch base su byte 1 o secondo byte (dipende protocol)
    uint8_t reg = data[1];
    switch (reg) {
        case 0x50:
            Serial.println("Registro 0x50 ricevuto");
            break;
        case 0x56:
            Serial.println("Registro 0x56 ricevuto");
            break;
        default:
            Serial.printf("Registro 0x%02X ricevuto\n", reg);
            break;
    }

    // debug: stampa frame
    Serial.print("Frame: ");
    for (size_t i = 0; i < len; ++i) {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();
}
