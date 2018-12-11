#ifndef Input_h
#define Input_h

#include <SPI.h> //SPI is used by the PS4 library
#include <PS4USB.h> // USB host shield library ver2 for PS4 controller using USB.
#include <controllerEnums.h> // For PS4 controller enums
#include "Settings.h"

#ifndef View_h
#include "View.h"
#endif

#define ONE_OVER_255             0.003921568627451
#define CHANNEL_PERCENT_ON_START 0.5

extern const char*    SCREEN_NAMES[];
extern const char*    MIX_EDIT_INPUT_NAMES[];
extern const uint16_t millisBetweenUpdates[];


class Input
{
  public:
    Input();
    void Initialize(View* view, Settings* settings);
    void ReadFromController(uint16_t currTime);
      
  private:
    void CheckInput(uint16_t currTime);

    View*     _view;
    Settings* _settings;
    float     _ps4AnalogValuesPercent[4];
    bool      _prevWasControllerConnected;
    uint16_t  _nextTimeMenuInputIsAllowed;
};
#endif