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

void handleRoot() {
    String html = "<html><head><title>Daikin Altherma ESP32</title></head><body>";
    html += "<h1>Daikin Altherma ESP32</h1>";
    html += "<p>JSON available at <a href='/getData'>/json</a></p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleJson() {    
    getValues();        
    server.send(200, "application/json", outJson);    
}

void initWebServer() {
    server.on("/", handleRoot);
    server.on("/getData", handleJson);
    server.begin();
}

void loopWebServer() {
    server.handleClient();
}

#endif // Display_h