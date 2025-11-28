#include "DaikinProtocol.h"
#include <Arduino.h>

DaikinProtocol::DaikinProtocol(SerialManager& sm)
: serialMgr(sm)
{}

void DaikinProtocol::begin() {
    serialMgr.onFrame([this](const uint8_t* data, size_t len){
        this->parseFrame(data, len);
    });
}

void DaikinProtocol::loop() {
    // eventualmente attività periodiche
}

uint8_t DaikinProtocol::computeCRC(const uint8_t* data, size_t len) {
    uint8_t sum = 0;
    for (size_t i = 0; i < len; ++i) sum += data[i];
    return ~sum;
}

bool DaikinProtocol::queryRegister(uint8_t regID, uint8_t* outBuffer, size_t& outLen, char protocol) {
    // Prepara comando
    uint8_t cmd[4] = {0};
    size_t cmdLen = 0;

    if(protocol == 'I'){
        cmd[0] = 0x03; cmd[1] = 0x40; cmd[2] = regID;
        cmd[3] = computeCRC(cmd, 3); cmdLen = 4;
    } else {
        cmd[0] = 0x02; cmd[1] = regID;
        cmd[2] = computeCRC(cmd, 2); cmdLen = 3;
    }

    serialMgr.write(cmd, cmdLen);

    // buffer temporaneo
    outLen = 0;
    uint32_t start = millis();
    uint8_t buffer[128] = {0};
    size_t expectedLen = (protocol=='I') ? 12 : 18; // default

    while ((millis()-start) < 300) {
        while(serialMgr.instance().loop(), serialMgr.instance().loop(), false){} // placeholder per leggere callback

        if(outLen >= expectedLen) break;

        delay(1);
    }

    // Copia in outBuffer
    memcpy(outBuffer, buffer, outLen);

    // CRC check
    if(outLen<1) return false;
    if(computeCRC(outBuffer, outLen-1) != outBuffer[outLen-1]) return false;

    return true;
}

void DaikinProtocol::parseFrame(const uint8_t* data, size_t len){
    if(len<3) return;

    uint8_t calc = computeCRC(data, len-1);
    if(calc != data[len-1]){
        Serial.println("CRC errore");
        return;
    }

    uint8_t reg = data[1];
    Serial.printf("Registro ricevuto 0x%02X\n", reg);
}
