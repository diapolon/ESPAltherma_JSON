#pragma once
#include <vector>
#include <cstdint>

// ===== Configurazione registri Daikin =====
// Abilitare/disabilitare registri impostando true/false

struct DaikinRegisterConfig {
    uint8_t regID;
    bool enabled;
    const char* name; // nome descrittivo per JSON
};

static std::vector<DaikinRegisterConfig> daikinRegisters = {
    {0x50, true, "temperature"},
    {0x56, true, "mode"},
    {0x5A, true, "fan"},
    {0x5C, true, "power"},
    {0x5E, false, "humidity"},
    {0x60, false, "setpoint"},
    {0x62, false, "timer"},
    {0x64, false, "error"},
    {0x66, false, "eco_mode"},
    {0x68, false, "unknown1"},
    {0x6A, false, "unknown2"}
    // aggiungere altri registri qui
};
