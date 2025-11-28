#include "JsonBuilder.h"
#include <map>
#include <vector>

bool JsonBuilder::buildFromDaikinMap(const std::map<uint8_t,std::vector<uint8_t>>& regData, String& outJson) {
    if(regData.empty()) return false;

    JsonDocument doc;
    JsonObject regs = doc["registers"].to<JsonObject>();

    for(const auto& kv : regData){
        uint8_t reg = kv.first;
        const std::vector<uint8_t>& buf = kv.second;
        
        JsonArray arr = regs[String(reg)].to<JsonArray>();        
        for(auto b : buf) arr.add(b);

        // Decodifica automatica dei registri principali
        switch(reg){
            case 0x50: // Temperatura
                if(buf.size() >= 6){
                    uint16_t tempRaw = (uint16_t)buf[3]<<8 | buf[4];
                    regs["temperature"] = tempRaw / 10.0f;
                }
                break;
            case 0x56: // Modalità
                if(buf.size() >= 6) regs["mode"] = buf[3];
                break;
            case 0x5A: // Ventola
                if(buf.size() >= 6) regs["fan"] = buf[3];
                break;
            case 0x5C: // Potenza
                if(buf.size() >= 6){
                    uint16_t power = (uint16_t)buf[3]<<8 | buf[4];
                    regs["power"] = power;
                }
                break;
                // Aggiungere altri registri da decodificare qui
            default:
                break;
        }        
    }

    serializeJson(doc, outJson);
    return true;
}
