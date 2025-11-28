#include <Arduino.h>
#include "SerialManager.h"
#include "DaikinProtocol.h"

// usa UART1: modifica se necessario
HardwareSerial MySerial(1);

DaikinProtocol* daikinPtr = nullptr;

void setup() {
    Serial.begin(115200);
    delay(100);

    // inizializza SerialManager singleton
    auto& sm = SerialManager::instance();
    sm.begin(MySerial, 9600);

    // crea e inizializza protocol handler
    static DaikinProtocol daikin(sm);
    daikinPtr = &daikin;
    daikin.begin();

    Serial.println("Setup completato");
}

void loop() {
    // ciclo principale: processa la seriale
    SerialManager::instance().loop();

    // puoi chiamare eventuale loop del protocollo
    if (daikinPtr) daikinPtr->loop();

    // piccolo delay per non saturare la CPU; se hai FreeRTOS non necessario
    delay(1);
}
