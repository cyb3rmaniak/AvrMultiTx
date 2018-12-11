#ifndef IntControl_h
#define IntControl_h

#include <Arduino.h>
#include "Control.h"

#define DEFAULT_MIN_VALUE -32007;
#define DEFAULT_MAX_VALUE 32007;

#endif

class IntControl: public Control
{
  public:
    IntControl(int16_t value, int16_t increaseStep, int16_t minValue, int16_t maxValue, uint8_t width, bool showPercent);

    // Virtual
    char*   ToString();
    uint8_t GetControlType();
    void    SwitchUp();
    void    SwitchDown();
    int16_t GetValue();
    void    SetValue(int16_t value);

    // Public
    bool showPercent;

  private:
    int16_t _value;
    int16_t _increaseStep;
    int16_t _minValue;
    int16_t _maxValue;
    String _tempString;
};