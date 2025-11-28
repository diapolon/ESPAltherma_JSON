#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>


class JsonBuilder {
public:
JsonBuilder();
// Build JSON from raw buffer returned by device
// returns true on success and writes JSON into provided String
bool buildFromDaikin(const uint8_t* data, size_t len, String& outJson);
};