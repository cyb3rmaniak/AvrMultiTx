#include <Arduino.h>
#include "Settings.h"

void Settings::Initialize()
{
    // Initiallize default mix settings
    for(int channel = 0 ; channel < 4 ; channel++)
        for (int mix = 0 ; mix < MIX_COUNT ; mix++)
            mixes[channel * MIX_COUNT + mix] = new MixSettings();
    
    LoadFromEEPROM();
}

void Settings::LoadFromEEPROM()
{
    if (EEPROM.read(CONFIG_START)     != EEPROM_ID[0] ||
        EEPROM.read(CONFIG_START + 1) != EEPROM_ID[1] ||
        EEPROM.read(CONFIG_START + 2) != EEPROM_ID[2])
    {
        Serial.println("No settings found to read from EEPROM");
        return;
    }

    Serial.println("Reading settings found in the EEPROM");

    int currOffset = CONFIG_START + 3;
    for (int mix = 0 ; mix < 4 * MIX_COUNT ; mix++)
    {
        EEPROM_readAnything(currOffset, *mixes[mix]);
        currOffset += sizeof(MixSettings);

        // Serial.print("Mix #");
        // Serial.println(mix);
        // Serial.println(mixes[mix] -> inputIndex);
        // Serial.println(mixes[mix] -> weight);
        // Serial.println(mixes[mix] -> inverted);
        // Serial.println(mixes[mix] -> expo);
        // Serial.println(mixes[mix] -> switch1Index);
        // Serial.println(mixes[mix] -> switch2Index);
        // Serial.println(mixes[mix] -> deadzone);
        // Serial.println(mixes[mix] -> behaviorIndex);
        // Serial.println(mixes[mix] -> limitMin);
        // Serial.println(mixes[mix] -> limitMax);
    }
}

void Settings::SaveToEEPROM()
{
    Serial.println("Writing settings to EEPROM");

    if (EEPROM.read(CONFIG_START) != EEPROM_ID[0])
    {
        EEPROM.write(CONFIG_START, EEPROM_ID[0]);
        EEPROM.write(CONFIG_START + 1, EEPROM_ID[1]);
        EEPROM.write(CONFIG_START + 2, EEPROM_ID[2]);
    }

    int currOffset = CONFIG_START + 3;
    for (int mix = 0 ; mix < 4 * MIX_COUNT ; mix++)
    {
        EEPROM_writeAnything(currOffset, *mixes[mix]);
        currOffset += sizeof(MixSettings);
    }
}