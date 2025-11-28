#include "JsonBuilder.h"
#include "config.h"

bool JsonBuilder::buildFromDaikin(const uint8_t* data, size_t len, String& outJson){
    if(!data || len<3) return false;

    JsonDocument doc;
    JsonArray arr = doc["raw"].to<JsonArray>();
    for(size_t i=0;i<len;i++) arr.add(data[i]);

    // esempio mappatura
    if(len>=6){
        uint16_t tempRaw = (uint16_t)data[3]<<8 | data[4];
        doc["temperature"] = tempRaw/10.0f;
    }

    doc["len"] = (int)len;

    serializeJson(doc, outJson);
    return true;
}
