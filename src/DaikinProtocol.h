#pragma once
#include <Arduino.h>

class SerialManager;

class DaikinProtocol {
public:
    explicit DaikinProtocol(class SerialManager& sm);
    void begin();
    void loop();

private:
    SerialManager& serialMgr;
    void parseFrame(const uint8_t* data, size_t len);

    static uint8_t computeCRC(const uint8_t* data, size_t len);
};
