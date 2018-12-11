#include "Control.h"

//==================================================================
// Virtual methods
//==================================================================
char* Control::ToString()
{
    return ((char*)"NoValue");
}

uint8_t Control::GetControlType()
{
    return CONTROL_TYPE_BASE;
}

void Control::SwitchUp() { return; }

void Control::SwitchDown() { return; }

int16_t Control::GetValue() { return; }

void Control::SetValue(int16_t value) { return; }

//==================================================================
// Public methods
//==================================================================

//==================================================================
// Private Methods:
//==================================================================