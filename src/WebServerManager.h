#pragma once
#include <Arduino.h>

class WebServerManager {
public:
WebServerManager();
void begin();
void setDataProvider(String (*provider)());
private:
};