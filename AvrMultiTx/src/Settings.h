#ifndef Settings_h
#define Settings_h

#include <Arduino.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"

#define CHANNEL_COUNT 8
#define CHANNEL_MIN_VALUE 1000
#define CHANNEL_MAX_VALUE 2000
#define CHANNEL_DEFAULT_VALUE 1500

#define MIX_COUNT 2

// Byte offset to store config data in EEPROM
#define CONFIG_START 32
#define EEPROM_ID "tx" // The first 3 chars saved to the EEPROM to identify the data as valid ('t' 'x' and '\0')

struct MixSettings 
{
    bool inverted;
    uint8_t inputIndex, expo, switch1Index, switch2Index, deadzone, behaviorIndex;
    int8_t weight, limitMin, limitMax;

    MixSettings()
    {
        inputIndex = weight = expo = switch1Index = switch2Index = deadzone = behaviorIndex = 0;
        inverted = false;
        limitMin = -100;
        limitMax = 100;
    }
};

class Settings
{
  public:
    MixSettings* mixes[4 * MIX_COUNT];

    void Initialize();
    void SaveToEEPROM();
    void LoadFromEEPROM();
};

#endif