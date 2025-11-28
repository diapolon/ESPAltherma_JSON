#include <Arduino.h>
#include "config.h"
#include "DaikinProtocol.h"
#include "JsonBuilder.h"
#include "WebServerManager.h"


static DaikinProtocol daikin;
static JsonBuilder jsonBuilder;
static WebServerManager webServer;


static uint8_t replyBuf[128];
static String latestJson = "{}";


String provideData() {
return latestJson;
}


void setup() {
Serial.begin(115200);
DBG_PRINTF("Starting ESPAltherma_JSON optimized build\n");


daikin.begin();
webServer.setDataProvider(provideData);
webServer.begin();


// create a FreeRTOS task to poll registers periodically
xTaskCreate([](void *){
const TickType_t delayTicks = pdMS_TO_TICKS(QUERY_INTERVAL_MS);
for (;;) {
size_t outLen = 0;
bool ok = daikin.queryRegister(0x50, replyBuf, outLen, 'I');
if (ok) {
String s;
if (jsonBuilder.buildFromDaikin(replyBuf, outLen, s)) {
latestJson = s;
DBG_PRINTF("Got JSON: %s\n", latestJson.c_str());
}
} else {
DBG_PRINTF("Query failed\n");
}
vTaskDelay(delayTicks);
}
}, "DaikinPoll", 4 * 1024, nullptr, 1, nullptr);
}


void loop() {
// main loop left empty; work happens in tasks
delay(1000);
}