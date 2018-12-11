#pragma GCC diagnostic ignored "-Wwrite-strings" // Ignores the pesky string constant to char* conversion warnings

#ifndef View_h
#define View_h

#include "Settings.h"

#include <SPI.h> //SPI is used by the TFT display
#include <TFT_S6D02A1.h> // Graphics and font library for S6D02A1 driver chip
#include <controllerEnums.h> // For PS4 controller enums
#include "IntControl.h"
#include "DropdownControl.h"

#define SCREEN_MIX_SELECT       0
#define SCREEN_MIX_EDIT         1
#define SCREEN_LAST             1

#define BUTTON_MARGIN_X 5
#define BUTTON_MARGIN_Y 8
#define BUTTON_WIDTH (160 - BUTTON_MARGIN_X * 2)
#define BUTTON_HEIGHT 17

#define MIX_EDIT_INPUT          0
#define MIX_EDIT_WEIGHT         1
#define MIX_EDIT_INVERTED       2
#define MIX_EDIT_EXPO           3
#define MIX_EDIT_SWITCH1        4
#define MIX_EDIT_SWITCH2        5
#define MIX_EDIT_DEAD_ZONE      6
#define MIX_EDIT_BEHAVIOUR      7
#define MIX_EDIT_LIMIT_MIN      8
#define MIX_EDIT_LIMIT_MAX      9

#define MIX_EDIT_PAGE1_LAST_CONTROL 5

#define MIX_EDIT_CONTROL_COUNT  10

#define CONTROL_HEIGHT      12
#define CONTROL_VALUE_X_POS 82

extern const char *SCREEN_NAMES[];
extern const uint16_t millisBetweenDraws[];

class View
{
  public:
    TFT_S6D02A1 tft;
    uint8_t     currScreen;
    uint8_t     currChannel;
    uint8_t     currMix;
    bool        isInEditMode;
    bool        allowFastControl;
    
    View();

    void    Initialize(int channelCount, Settings* settings);
    void    Refresh();
    void    MarkSelection(int8_t index); // Can be negative. Use -1 to mark the last available index.
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
    uint8_t CycleIndex(int index, int maxIndex);
    uint8_t CycleIndex(int newIndex, int minIndex, int maxIndex);
    
    MixSettings* View::GetSettings();
    void SetSettings(MixSettings* newSettings);

      
  private:
    uint8_t     _markedIndex[SCREEN_LAST + 1];
    uint8_t     _prevMarkedIndex;
    bool        _erasePreviousMarker;
    int16_t     _currX;
    int16_t     _currY;
    int16_t     _prevMarkerX;
    int16_t     _prevMarkerY;
    uint8_t     _splashScreenIteration;
    uint8_t     _prevScreen[5];
    uint8_t     _currHistoryIndex;
    uint8_t     _channelCount;
    uint8_t     _currPage;
    uint8_t     _totalPages;
    uint8_t     _currPageMinControlIndex;
    uint8_t     _currPageMaxControlIndex;
    Control*    _mix_edit_controls[MIX_EDIT_CONTROL_COUNT];
    Control*    _selectedControl;
    bool        _isCurrControlMarked;
    int16_t     _valueBeforeEdit;
    Settings*   _settings;
    String      _header;
                
    void      DrawCurrScreen();

    void      InitMixSelectScreen();
    void      DrawMixSelectScreen(const char* MIX_EDIT_INPUT_NAMES[]);
    void      InitMixEditScreen(uint8_t page, 
                                const char* MIX_EDIT_INPUT_NAMES[], const char* MIX_EDIT_BOOL_NAMES[], 
                                const char* MIX_EDIT_SWITCH_NAMES[], const char* MIX_EDIT_BEHAVIOR_NAMES[]);

    void      InitializeMixEditControls(const char* MIX_EDIT_INPUT_NAMES[], const char* MIX_EDIT_BOOL_NAMES[], 
                                        const char* MIX_EDIT_SWITCH_NAMES[], const char* MIX_EDIT_BEHAVIOR_NAMES[]);
    void      DrawMixEditScreen();
    void      EnterEditMode();
    void      ExitEditMode(bool saveChange);
    void      InitGeneralScreen(bool drawDefaultHeader);

    void      DrawHeader(char* header, bool drawL1R1Controls);
    void      DrawButton(char* text, bool isMarked);
    void      DrawButton(int16_t x, int16_t y, int16_t width, int16_t height, char* text, uint8_t fontSize, uint16_t textColor, uint16_t bgColor, uint16_t buttonLineColor, bool shouldFill, uint16_t buttonBgColor);
    void      DrawString(char* content, int font);
    void      NewLine(int font);
    uint8_t   GetNumberLength(long number);
    void      DrawPercent(long number, int font);
    void      DrawControl(Control* control, bool isSelected);
    void      EraseControl(Control control);
    void      DrawMarker();
    void      EraseMarker();
};

#endif