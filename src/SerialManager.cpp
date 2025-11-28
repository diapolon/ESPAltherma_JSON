#include "SerialManager.h"

SerialManager& SerialManager::instance() {
    static SerialManager inst;
    return inst;
}

void SerialManager::begin(HardwareSerial& port, unsigned long baud) {
    serial = &port;
    // MODE: 8E1 come richiesto dal protocollo Daikin (se serve diverso, cambia)
    serial->begin(baud, SERIAL_8E1);
}

void SerialManager::write(const uint8_t* data, size_t len) {
    if (serial) serial->write(data, len);
}

void SerialManager::loop() {
    if (!serial) return;

    // buffer locale per ricostruire frame
    static uint8_t buffer[256];
    static size_t idx = 0;
    static uint32_t lastByteMillis = 0;

    while (serial->available()) {
        uint8_t b = (uint8_t)serial->read();
        lastByteMillis = millis();

        // Se siamo all'inizio del pacchetto, accettiamo solo header validi (0x02 o 0x03)
        if (idx == 0) {
            if (b != 0x02 && b != 0x03) {
                // ignora byte spurio
                continue;
            }
        }
        buffer[idx++] = b;

        // protezione overflow
        if (idx >= sizeof(buffer)) {
            idx = 0;
            // se vuoi loggare: Serial.println("Buffer overflow, resetted");
            return;
        }

        // Se protocollo I: il terzo byte (index 2) contiene la lunghezza utile
        if (idx >= 3 && (buffer[0] == 0x03 || buffer[0] == 0x02)) {
            uint8_t expectedLen = (uint8_t)buffer[2] + 2; // come da doc
            if (idx == expectedLen) {
                // frame completo: chiamiamo callback
                if (callback) callback(buffer, idx);
                idx = 0;
                return;
            }
        }
        // Per protocollo S potresti voler controllare un terminatore specifico (non implementato qui)
    }

    // Timeout: se abbiamo bytes parziali e non arrivano nuovi byte per troppo tempo, reset
    if (idx > 0 && (millis() - lastByteMillis) > 80) {
        idx = 0;
    }
}
