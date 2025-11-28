#include "JsonBuilder.h"
#include <map>
#include <vector>
#include "daikin_registers_config.h"

bool JsonBuilder::buildFromDaikinMap(const std::map<uint8_t,std::vector<uint8_t>>& regData, String& outJson) {
    if(regData.empty()) return false;

    JsonDocument doc;
    JsonObject regs = doc["registers"].to<JsonObject>();

    unsigned long ts = millis();
    regs["timestamp_ms"] = ts; // timestamp del momento della lettura

    for(const auto& kv : regData){
        uint8_t reg = kv.first;
        const std::vector<uint8_t>& buf = kv.second;
        
        // Cerca il nome descrittivo dal file di configurazione
        const char* name = nullptr;
        for(auto& r : daikinRegisters) {
            if(r.regID == reg){
                name = r.name;
                break;
            }
        }
        if(!name) name = String(reg).c_str(); // fallback a ID

        JsonArray arr = regs[String(reg)].to<JsonArray>();        
        for(auto b : buf) arr.add(b);

        // Decodifica automatica dinamica basata sul nome nel JSON
        if(buf.size() >= 6){
            if(strcmp(name, "temperature") == 0) regs["temperature"] = ((uint16_t)buf[3]<<8 | buf[4])/10.0f;
            else if(strcmp(name, "mode") == 0) regs["mode"] = buf[3];
            else if(strcmp(name, "fan") == 0) regs["fan"] = buf[3];
            else if(strcmp(name, "power") == 0) regs["power"] = (uint16_t)buf[3]<<8 | buf[4];
            // altri registri decodificabili automaticamente possono essere aggiunti qui
        }                
    }
    serializeJson(doc, outJson);
    return true;
}
