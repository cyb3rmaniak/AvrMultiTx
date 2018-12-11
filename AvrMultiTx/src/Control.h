#ifndef Control_h
#define Control_h

#include <Arduino.h>

#define CONTROL_TYPE_BASE     0
#define CONTROL_TYPE_INT      1
#define CONTROL_TYPE_DROPDOWN 2
#define CONTROL_TYPE_FLOAT    3

class Control
{
  public:
    virtual char*   ToString();
    virtual uint8_t GetControlType();
    virtual void    SwitchUp(); 
    virtual void    SwitchDown();
    virtual int16_t GetValue();
    virtual void    SetValue(int16_t value);

    uint8_t   width;
    uint16_t  prevX;
    uint16_t  prevY;
    bool      isDirty;
};

#endif