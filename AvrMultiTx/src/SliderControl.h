#ifndef SliderControl_h
#define SliderControl_h

#include <Arduino.h>
#include "Control.h"

#endif

class SliderControl: public Control
{
  public:
    SliderControl(int16_t value, int16_t minValue, int16_t maxValue, uint8_t width);

    // Virtual
    char*   ToString();
    uint8_t GetControlType();
    int16_t GetValue();
    void    SetValue(int16_t value);

  private:
    int16_t _value;
    int16_t _minValue;
    int16_t _maxValue;
    String _tempString;
};