#pragma once

#define VERSION     "v1.00.1"

// ===== Parametri WiFi =====
#define WIFI_SSID     "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// ===== Parametri seriale =====
#define SERIAL_PORT   1          // UART1
#define SERIAL_BAUD   9600       // Baudrate
#define SERIAL_RX_PIN 16         // GPIO per RX
#define SERIAL_TX_PIN 17         // GPIO per TX

// ===== Parametri Daikin =====
#define DAIKIN_QUERY_INTERVAL 5000 // Interrogazione in ms