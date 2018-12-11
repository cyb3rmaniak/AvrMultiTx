#include "IntControl.h"

IntControl::IntControl(int16_t value, int16_t increaseStep, int16_t minValue, int16_t maxValue, uint8_t width, bool showPercent)
{
    _minValue = minValue;
    _maxValue = maxValue;
    _value = constrain(value, _minValue, _maxValue);
    
    _increaseStep = increaseStep;

    this -> showPercent = showPercent;
    this -> width = width;

    isDirty = true;
}

//==================================================================
// Virtual methods
//==================================================================
char* IntControl::ToString()
{
    _tempString = String(_value);
    char* returned = (char*)_tempString.c_str();
    return returned;
}

uint8_t IntControl::GetControlType()
{
    return CONTROL_TYPE_INT;
}

void IntControl::SwitchUp()
{
    SetValue(_value + _increaseStep);
}

void IntControl::SwitchDown()
{
    SetValue(_value - _increaseStep);
}

int16_t IntControl::GetValue()
{
    return _value;
}

void IntControl::SetValue(int16_t value)
{
    if (value == _value)
        return;

    _value = constrain(value, _minValue, _maxValue);
    isDirty = true;
}

//==================================================================
// Public methods
//==================================================================


//==================================================================
// Private Methods:
//==================================================================