#include "SerialManager.h"
#include "config.h"


HardwareSerial hwSerial(DAIGIN_UART_NUM);


SerialManager::SerialManager() {}


void SerialManager::begin() {
hwSerial.begin(DAIGIN_BAUD, SERIAL_8E1, DAIGIN_RX_PIN, DAIGIN_TX_PIN);
}


size_t SerialManager::available() {
return hwSerial.available();
}


int SerialManager::read(uint8_t* buf, size_t len) {
size_t r = 0;
unsigned long start = millis();
while (r < len && (millis() - start) < SERIAL_READ_TIMEOUT_MS) {
if (hwSerial.available()) {
buf[r++] = hwSerial.read();
}
}
return r;
}


int SerialManager::readByte() {
unsigned long start = millis();
while ((millis() - start) < SERIAL_READ_TIMEOUT_MS) {
if (hwSerial.available()) return hwSerial.read();
}
return -1;
}


void SerialManager::write(const uint8_t* buf, size_t len) {
hwSerial.write(buf, len);
}


void SerialManager::flushTx() {
hwSerial.flush();
}