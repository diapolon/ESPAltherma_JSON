#include "JsonBuilder.h"
#include "config.h"


JsonBuilder::JsonBuilder() {}


bool JsonBuilder::buildFromDaikin(const uint8_t* data, size_t len, String& outJson) {
if (!data || len < 3) return false;


StaticJsonDocument<JSON_DOC_SIZE> doc;
// Example parsing: the actual mapping depends on device documentation.
// We'll store raw payload as hex array and some derived fields for demo.


JsonArray arr = doc.createNestedArray("raw");
for (size_t i = 0; i < len; ++i) arr.add(data[i]);


// Example: if at offset 3 there is a temperature word (big endian)
if (len >= 6) {
uint16_t tempRaw = (uint16_t)data[3] << 8 | data[4];
// device-specific scaling (example)
float temperature = (float)tempRaw / 10.0f;
doc["temperature"] = temperature;
}


doc["len"] = (int)len;


// Serialize to String (direct to String avoids intermediate char buffers)
outJson = "";
serializeJson(doc, outJson);
return true;
}