#include "DaikinProtocol.h"


uint8_t DaikinProtocol::computeCRC(const uint8_t* data, size_t len) {
uint8_t sum = 0;
for (size_t i = 0; i < len; ++i) sum += data[i];
return ~sum;
}


bool DaikinProtocol::queryRegister(uint8_t regID, uint8_t* outBuffer, size_t& outLen, char protocol) {
// Build command
uint8_t cmd[4] = {0};
uint8_t cmdLen = 0;


if (protocol == 'I') {
cmd[0] = 0x03; cmd[1] = 0x40; cmd[2] = regID; cmd[3] = computeCRC(cmd, 3); cmdLen = 4;
} else {
cmd[0] = 0x02; cmd[1] = regID; cmd[2] = computeCRC(cmd, 2); cmdLen = 3;
}


serialMgr.flushTx();
serialMgr.write(cmd, cmdLen);


// Read reply
unsigned long start = millis();
size_t idx = 0;
int expected = (protocol == 'I') ? 12 : 18; // default


while ((millis() - start) < SERIAL_READ_TIMEOUT_MS) {
int b = serialMgr.readByte();
if (b < 0) continue; // retry until timeout


outBuffer[idx++] = (uint8_t)b;


// If protocol I, after reading 3rd byte we know full length
if (protocol == 'I' && idx == 3) {
expected = outBuffer[2] + 2;
}


// Early error detection: common error code
if (idx >= 2 && outBuffer[0] == 0x15 && outBuffer[1] == 0xEA) {
DBG_PRINTF("Daikin: error response 0x15 0xEA\n");
outLen = idx;
return false;
}


if (idx >= (size_t)expected) break;
}


outLen = idx;


if (idx < (size_t)expected) {
DBG_PRINTF("Daikin: timeout reading register 0x%02X got %d/%d\n", regID, (int)idx, expected);
return false;
}


// CRC check
if (idx < 1) return false;
uint8_t calc = computeCRC(outBuffer, outLen - 1);
if (calc != outBuffer[outLen - 1]) {
DBG_PRINTF("Daikin: CRC mismatch (calc=0x%02X, recv=0x%02X)\n", calc, outBuffer[outLen - 1]);
return false;
}


return true;
}