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