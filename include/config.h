#pragma once

#define VERSION     "v1.00.3"

// ===== Parametri WiFi =====
#define WIFI_SSID     "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// ===== Parametri Daikin =====
#define DAIKIN_QUERY_INTERVAL 5000 // Interrogazione in ms

// ===== Parametri seriale =====
#define SERIAL_PORT   1          // UART1
#define SERIAL_BAUD   9600       // Baudrate

#ifdef M5STICKC_PLUS
#define SERIAL_RX_PIN 32
#define SERIAL_TX_PIN 26
#endif


#ifdef ESP32_S3_TERMINAL
#define SERIAL_RX_PIN 43
#define SERIAL_TX_PIN 44
#define DISPLAY_ROTATION 1
#define DISPLAY_DB0 2
#define DISPLAY_DB1 3
#define DISPLAY_DB2 4
#define DISPLAY_DB3 5
#define DISPLAY_DB4 6
#define DISPLAY_DB5 7
#define DISPLAY_DB6 8
#define DISPLAY_DB7 9
#define DISPLAY_DB8 10
#define DISPLAY_DB9 11
#define DISPLAY_DB10 12
#define DISPLAY_DB11 13
#define DISPLAY_DB12 14
#define DISPLAY_DB13 15
#define DISPLAY_DB14 16
#define DISPLAY_DB15 17

#define DISPLAY_CS 21
#define DISPLAY_WR 22
#define DISPLAY_DC 23
#define DISPLAY_RST 25
#define DISPLAY_BACKLIGHT 26
#endif


#ifndef M5STICKC_PLUS
#ifndef ESP32_S3_TERMINAL
// Default ESP32 pins
#define SERIAL_RX_PIN 16
#define SERIAL_TX_PIN 17
#endif
#endif

