#ifndef debugSerial_h
    #define debugSerial_h
    #include "Stream.h"
    #ifdef ARDUINO_M5Stick_C_Plus
        #include <M5StickCPlus.h>
    #elif ARDUINO_M5Stick_C
        #include <M5StickC.h>
    #endif
    class DebugSerial: public Stream {
    
        public:
            inline size_t write(uint8_t) { return 0; };
            size_t write(const uint8_t *buffer, size_t size);
            inline int available(void) { return true;};
            inline int availableForWrite(void) { return 0; };
            inline int peek(void) { return 0; };
            inline int read(void) { return 0; };
            inline void flush(void) { };
            inline size_t write(const char * s) {
                return write((uint8_t*) s, strlen(s));
            }
            inline size_t write(unsigned long n) {
                return write((uint8_t) n);
            }
            inline size_t write(long n) {
                return write((uint8_t) n);
            }
            inline size_t write(unsigned int n) {
                return write((uint8_t) n);
            }
            inline size_t write(int n) {
                return write((uint8_t) n);
            }

        DebugSerial();
        ~DebugSerial();
    };

    DebugSerial debugSerial;

    DebugSerial::DebugSerial() { }

    size_t DebugSerial::write(const uint8_t *buffer, size_t size) {
        #ifdef ARDUINO_M5Stick_C
            if (M5.Lcd.getCursorY()+13>M5.Lcd.height()) {
                M5.Lcd.fillScreen(TFT_BLACK);
                M5.Lcd.setCursor(0,0);
            }
            M5.Lcd.print((const char*) buffer);
        #endif
        #ifndef DISABLE_LOG_MESSAGES    
            Serial.write(buffer,size);
        #endif
        return size;
    }

    DebugSerial::~DebugSerial() { }
#endif
