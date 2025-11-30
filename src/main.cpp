#include <Arduino.h>
#include <ArduinoJson.h>
#include <Globals.h>
#include <Utilities.h>
#include <Driver.h>
#ifdef LGFX_USE_V1
    #include <DisplayLvgl.h>
#else
    #include <DisplayM5.h>
#endif
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
    loopWifi();  
    loopWebServer();
    unsigned long wait = DAIKIN_QUERY_INTERVAL - millis() + start;
    debugSerial.printf("Done. Waiting ", String(wait));
    waitLoop(wait);
}