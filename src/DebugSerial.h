#ifndef debugSerial_h
    #define debugSerial_h
    #include "Stream.h"    
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
        if (DEBUG) Serial.write(buffer, size);                
        //String out;
        //out.reserve(size);
        //out.concat((const char*)buffer, size);
        //updateScreenMainText(out);
        return size;
    }

    DebugSerial::~DebugSerial() { }
#endif
