#pragma once
#include <Arduino.h>
#include <functional>

class SerialManager {
public:
    using FrameCallback = std::function<void(const uint8_t*, size_t)>;

    static SerialManager& instance();

    // Inizializza il manager: fornisci la porta HardwareSerial e il baud
    void begin(HardwareSerial& port, unsigned long baud);

    // Deve essere chiamato frequentemente nel loop() per processare i byte
    void loop();

    // Imposta callback che riceve frame completi (puntatore al buffer, lunghezza)
    void onFrame(FrameCallback cb) { callback = cb; }

    // Scrive sulla seriale
    void write(const uint8_t* data, size_t len);

private:
    SerialManager() = default;
    HardwareSerial* serial = nullptr;
    FrameCallback callback; // default-constructed -> empty callable
};
