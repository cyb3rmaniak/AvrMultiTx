#include "DropdownControl.h"

DropdownControl::DropdownControl(char** values, uint8_t selectedIndex, uint8_t width)
{
    _selectedIndex = selectedIndex;
    this -> width = width;

    SetValues(values);
}

//==================================================================
// Virtual methods
//==================================================================
char* DropdownControl::ToString()
{
    return _values[_selectedIndex];
}

uint8_t DropdownControl::GetControlType()
{
    return CONTROL_TYPE_DROPDOWN;
}

void DropdownControl::SwitchUp()
{
    _selectedIndex = constrain(_selectedIndex - 1, 0, _valueCount - 1);
    isDirty = true;
}

void DropdownControl::SwitchDown()
{
    _selectedIndex = constrain(_selectedIndex + 1, 0, _valueCount - 1);
    isDirty = true;
}

//==================================================================
// Public methods
//==================================================================
int16_t DropdownControl::GetValue()
{
    return _selectedIndex;
}

void DropdownControl::SetValue(int16_t value)
{
    _selectedIndex = value;
}

char** DropdownControl::GetValues()
{
    return _values;
}

void DropdownControl::SetValues(char** values)
{
    _values = values;
    for (int currIndex = 0 ; currIndex < 15 ; currIndex++)
    {
        if (_values[currIndex] == NULL)
        {
            _valueCount = currIndex;
            break;
        }
    }

    isDirty = true;
}

//==================================================================
// Private Methods:
//==================================================================