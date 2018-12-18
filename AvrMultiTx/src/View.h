#pragma GCC diagnostic ignored "-Wwrite-strings" // Ignores the pesky string constant to char* conversion warnings

#ifndef View_h
#define View_h

#include "Settings.h"

#include "SliderControl.h"

#include <SPI.h> //SPI is used by the TFT display
#include <TFT_S6D02A1.h> // Graphics and font library for S6D02A1 driver chip
#include <controllerEnums.h> // For PS4 controller enums

#define SCREEN_FLY              0
#define SCREEN_CHANNEL_TESTER   1
#define SCREEN_LAST             SCREEN_CHANNEL_TESTER

#define BUTTON_MARGIN_X 5
#define BUTTON_MARGIN_Y 8
#define BUTTON_WIDTH (160 - BUTTON_MARGIN_X * 2)
#define BUTTON_HEIGHT 17

extern const uint16_t millisBetweenDraws[];

class View
{
  public:
    TFT_S6D02A1 tft;
    uint8_t     currScreen;
    
    View();

    void    Initialize(int channelCount, Settings* settings);
    void    Refresh();
    void    DPadPress(ButtonEnum button);
    void    OKWasPressed();
    void    BackWasPressed();
    void    L1WasPressed();
    void    R1WasPressed();
    void    MoveToScreen(uint8_t screen, bool keepHistory);
    void    MoveBackToPrevScreen();
    void    InitCurrScreen();
    void    ShowError(char* message);
    void    SetControllerStatus(bool isConnected);
    bool    IsAnyControlDirty();
    
    MixSettings* GetSettings();
    void SetSettings(MixSettings* newSettings);

      
  private:
    bool        _shouldRefreshScreen;
    int16_t     _currX;
    int16_t     _currY;
    uint8_t     _prevScreen[5];
    uint8_t     _currHistoryIndex;
    uint8_t     _channelCount;
    Settings*   _settings;
    String      _header;
                
    void      DrawCurrScreen();

    void      InitChannelTesterScreen();
    void      DrawChannelTesterScreen();
    void      InitFlyScreen();
    void      DrawFlyScreen();

    void      InitGeneralScreen(bool drawDefaultHeader);

    void      DrawHeader(char* header, bool drawL1R1Controls);
    void      DrawButton(char* text, bool isMarked);
    void      DrawButton(int16_t x, int16_t y, int16_t width, int16_t height, char* text, uint8_t fontSize, uint16_t textColor, uint16_t bgColor, uint16_t buttonLineColor, bool shouldFill, uint16_t buttonBgColor);
    void      DrawString(char* content, int font);
    void      NewLine(int font);
    void      DrawControl(Control* control, bool isSelected);
    void      EraseControl(Control control);
};

#endif