#include "WebServerManager.h"

#include <WiFi.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

static String (*dataProvider)() = nullptr;

WebServerManager::WebServerManager() {}


void WebServerManager::setDataProvider(String (*provider)()) {
dataProvider = provider;
}


void WebServerManager::begin() {
// Connect to WiFi (expect user to set SSID/PWD in config or use provisioning)
// For demo we'll use WiFi.begin() with saved credentials. Replace as needed.
WiFi.mode(WIFI_STA);
WiFi.begin();


Serial.print("Connecting WiFi...\n");
unsigned long start = millis();
while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) delay(200);


#ifdef ASYNCWEB_ENABLED
static AsyncWebServer server(HTTP_PORT);


server.on("/getData", HTTP_GET, [](AsyncWebServerRequest* req){
if (!dataProvider) {
req->send(500, "application/json", "{\"error\":\"no data provider\"}");
return;
}
String payload = dataProvider();
req->send(200, "application/json", payload);
});


server.onNotFound([](AsyncWebServerRequest* req){ req->send(404); });
server.begin();
Serial.print("HTTP server started on port %d\n", HTTP_PORT);
#else
Serial.print("Async webserver not enabled at compile time\n");
#endif
}