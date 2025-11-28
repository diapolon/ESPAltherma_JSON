#pragma once
#include <ArduinoJson.h>
#include <Arduino.h>
#include <map>
#include <vector>

class JsonBuilder {
public:
    JsonBuilder() = default;

    // Costruisce JSON da mappa di registri Daikin
    bool buildFromDaikinMap(const std::map<uint8_t,std::vector<uint8_t>>& regData, String& outJson);
};
