#pragma once
#include "SerialManager.h"
#include <Arduino.h>

class DaikinProtocol {
public:
    explicit DaikinProtocol(SerialManager& sm);

    void begin();
    void loop();

    // Query singolo registro, restituisce true se riceve dati validi
    bool queryRegister(uint8_t regID, uint8_t* outBuffer, size_t& outLen, char protocol = 'I');

private:
    SerialManager& serialMgr;
    void parseFrame(const uint8_t* data, size_t len);

    static uint8_t computeCRC(const uint8_t* data, size_t len);
};
