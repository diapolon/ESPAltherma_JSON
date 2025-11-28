#pragma once


#include <Arduino.h>


class SerialManager {
public:
SerialManager();
void begin();
size_t available();
int read(uint8_t* buf, size_t len);
int readByte();
void write(const uint8_t* buf, size_t len);
void flushTx();
private:
};