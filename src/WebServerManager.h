#ifndef WebServerManager_h
#define WebServerManager_h

#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void initWifi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    updateScreenInfos("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        updateScreenInfos(".");
    }
    updateScreenInfos(WiFi.localIP().toString());
}

void loopWifi() {
    if (WiFi.status() != WL_CONNECTED) { 
    //restart board if needed
        int i = 0;
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            if (i++ == 120) {
            updateScreenMainText("Tried connecting for 60 sec, rebooting now.");
            restartBoard();
            }
        }
    }
}

void handleRoot() {
    String html = "<html><head><title>Daikin Altherma ESP32</title></head><body>";
    html += "<h1>Daikin Altherma ESP32</h1>";
    html += "<p>JSON available at <a href='/getData'>/json</a></p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleGetData() {    
    getValues();  
    delay(100);      
    server.send(200, "application/json", outJson);    
}

void handleRebootDevice() {    
    String html = "<html><head><title>Daikin Altherma ESP32</title></head><body>";
    html += "<h1>Daikin Altherma ESP32</h1>";
    html += "<p>Reboot in 2 sec</p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
    delay(2000);
    restartBoard();
}

void initWebServer() {
    server.on("/", handleRoot);
    server.on("/getData", handleGetData);
    server.on("/rebootDevice", handleRebootDevice);
    server.begin();
}

void loopWebServer() {
    server.handleClient();
}

#endif // Display_h