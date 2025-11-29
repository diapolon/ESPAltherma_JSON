#include <Arduino.h>
#include <ArduinoJson.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <Globals.h>
#include <Utilities.h>

#include <Display_driver.h>
#include <Display.h>
#include <DebugSerial.h>
#include <SerialManager.h>
#include <WebServerManager.h>

void initTasks() {     
    xTaskCreate(displayTask, "displayTask", 4000, NULL, tskIDLE_PRIORITY, NULL);        
}

void updateDisplay() {    
    String msg = "";
    if (msg != "") updateScreenMainText(msg);     
}

void setup() {
    Serial.begin(115200);
    delay(100);

    initSerial();
    delay(100);

    initDisplay();
    delay(100);      

    initTasks(); 
    delay(100);      

    initWifi(); 
    delay(100);      

    initRegistries();
    delay(100);

    initWebServer(); 
    delay(100);          

    
}

void loop() {  
    unsigned long start = millis();      
    loopWebServer();
    unsigned long wait = DAIKIN_QUERY_INTERVAL - millis() + start;
    debugSerial.printf("Done. Waiting ", String(wait));
    waitLoop(wait);
}