#ifndef DropdownControl_h
#define DropdownControl_h

#include <Arduino.h>
#include "Control.h"

#define DEFAULT_MIN_VALUE -32007;
#define DEFAULT_MAX_VALUE 32007;

#endif

class DropdownControl: public Control
{
  public:
    DropdownControl(char** values, uint8_t selectedIndex, uint8_t width);

    // Virtual
    char*     ToString();
    uint8_t   GetControlType();
    void      SwitchUp();
    void      SwitchDown();
    int16_t   GetValue();
    void      SetValue(int16_t value);

    // Public
    char**  GetValues();
    void    SetValues(char** values);

  private:
    uint8_t _selectedIndex;
    uint8_t _valueCount;
    char**  _values;
};