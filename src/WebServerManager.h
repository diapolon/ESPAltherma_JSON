#ifndef WebServerManager_h
#define WebServerManager_h

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

AsyncWebServer server(80);

void loopWifi() {
    if (WiFi.status() != WL_CONNECTED) { 
    //restart board if needed
        int i = 0;
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            if (i++ == 120) {
                updateScreenMainText("Tried connecting for 60 sec, rebooting now.");
                delay(100);
                restartBoard();
            }
        }
    }
}

void initWifi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    loopWifi();
    updateScreenInfos(WiFi.localIP().toString());
}

void initWebServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<html><head><title>Daikin Altherma ESP32</title></head><body>";
        html += "<h1>Daikin Altherma ESP32 "+ String(FIRMWARE_VERSION) +"</h1>";
        html += "<p>Data available at <a href='/getData'>/json</a></p>";
        html += "<br /><p><a href='/update'>Upgrade Firmware (OTA)</a></p>";
        html += "<br /><p><a href='/reboot'>Reboot</a></p>";
        html += "</body></html>";
        request->send(200, "text/html", html);
    });     
    server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", outJson);
    });    
    server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
        String html = "<html><head><title>Daikin Altherma ESP32</title></head><body>";
        html += "<h1>Daikin Altherma ESP32</h1>";
        html += "<p>Reboot in 2 sec</p>";
        html += "</body></html>";        
        request->send(200, "text/html", html);
        delay(2000);
        restartBoard();        
    });        
    ElegantOTA.begin(&server);   
    
    server.begin();
}

#endif // Display_h