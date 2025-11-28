#include "JsonBuilder.h"
#include <map>
#include <vector>
#include "daikin_registers_config.h"

bool JsonBuilder::buildFromDaikinMap(const std::map<uint8_t,std::vector<uint8_t>>& regData, String& outJson) {
    if(regData.empty()) return false;

    JsonDocument doc;
    JsonObject regs = doc.to<JsonObject>();
   
    regs["timestamp_ms"] = millis();

    for(const auto& kv : regData){
        uint8_t reg = kv.first;
        const std::vector<uint8_t>& buf = kv.second;

        // Trova nome descrittivo dal file di configurazione
        const char* name = nullptr;
        for(auto& r : daikinRegisters){
            if(r.regID == reg){
                name = r.name;
                break;
            }
        }
        if(!name) name = String(reg).c_str();

        JsonVariant regVar = regs[name];
        JsonObject regObj = regVar.as<JsonObject>();

        // Raw array
        JsonVariant rawVar = regObj["raw"];
        JsonArray rawArr = rawVar.as<JsonArray>();        

        for(auto b : buf) rawArr.add(b);

        regObj["timestamp_ms"] = millis();

        // Decodifica automatica dei registri principali
        if(buf.size() >= 6){
            if(strcmp(name, "temperature") == 0) regObj["value"] = ((uint16_t)buf[3]<<8 | buf[4])/10.0f;
            else if(strcmp(name, "mode") == 0) regObj["value"] = buf[3];
            else if(strcmp(name, "fan") == 0) regObj["value"] = buf[3];
            else if(strcmp(name, "power") == 0) regObj["value"] = (uint16_t)buf[3]<<8 | buf[4];
        }
    }
    serializeJson(doc, outJson);
    return true;
}
