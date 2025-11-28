#pragma once
#include <Arduino.h>


class DaikinProtocol {
public:
DaikinProtocol();
void begin();


// Query a register (non-blocking caller). Returns true on success.
bool queryRegister(uint8_t regID, uint8_t* outBuffer, size_t& outLen, char protocol = 'I');


// Utility CRC
static uint8_t computeCRC(const uint8_t* data, size_t len);
};