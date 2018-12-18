#include "SliderControl.h"

SliderControl::SliderControl(int16_t value, int16_t minValue, int16_t maxValue, uint8_t width)
{
    _minValue = minValue;
    _maxValue = maxValue;
    _value = constrain(value, _minValue, _maxValue);
    
    this -> width = width;

    isDirty = true;
}

//==================================================================
// Virtual methods
//==================================================================
char* SliderControl::ToString()
{
    _tempString = String(_value);
    char* returned = (char*)_tempString.c_str();
    return returned;
}

uint8_t SliderControl::GetControlType()
{
    return CONTROL_TYPE_SLIDER;
}

int16_t SliderControl::GetValue()
{
    return _value;
}

void SliderControl::SetValue(int16_t value)
{
    if (value == _value)
        return;

    _value = constrain(value, _minValue, _maxValue);
    isDirty = true;
}