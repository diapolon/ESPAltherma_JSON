#include <Arduino.h>
#include <ArduinoJson.h>
#include <Globals.h>
#include <Utilities.h>
#include <Driver.h>
#include <DisplayLvgl.h>
#include <DebugSerial.h>
#include <SerialManager.h>
#include <WebServerManager.h>

void initTasks() {     
    xTaskCreate(displayTask, "displayTask", 4000, NULL, tskIDLE_PRIORITY, NULL);        
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
    unsigned long now = millis();
    loopWifi();      
    if (now - lastRead >= interval) {
        lastRead = now;
        readSerialAndBuildJson();
    }    
    ElegantOTA.loop();    
    delay(100);
}