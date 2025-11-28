#pragma once


#include <Arduino.h>


// Serial port to read Daikin unit
#define DAIGIN_RX_PIN 16 // UART1 RX (change as needed)
#define DAIGIN_TX_PIN 17 // UART1 TX if needed
#define DAIGIN_UART_NUM 1
#define DAIGIN_BAUD 9600


// HTTP server
#define HTTP_PORT 80


// Timing
#define QUERY_INTERVAL_MS 5000UL // how often to poll registers
#define SERIAL_READ_TIMEOUT_MS 300


// JSON sizes (tune depending on returned payload)
#define JSON_DOC_SIZE 1024


// Debug
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 1
#endif


#if DEBUG_LEVEL > 0
#define DBG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...) (void)0
#endif