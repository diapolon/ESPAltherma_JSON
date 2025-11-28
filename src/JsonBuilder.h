#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>

class JsonBuilder {
public:
    JsonBuilder() = default;

    // data = buffer raw dal Daikin
    bool buildFromDaikin(const uint8_t* data, size_t len, String& outJson);
};
