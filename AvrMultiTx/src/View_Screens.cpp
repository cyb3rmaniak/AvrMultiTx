#ifndef View_Screens_cpp
#define View_Screens_cpp
#include "View.h"

void View::InitChannelTesterScreen()
{
    _header = String("Channel Tester");

    InitGeneralScreen(false);
    
    DrawHeader((char*)_header.c_str(), true);
}

void View::DrawChannelTesterScreen()
{
    _currY = 30;
    _currX = BUTTON_MARGIN_X * 2;

    for (int channel = 0 ; channel < CHANNEL_COUNT ; channel++)
    {
        if (_shouldRefreshScreen)
            tft.fillScreen(TFT_BLACK);

        tft.setTextColor(TFT_WHITE);
        _currY += 5;

        String channelId = "Channel " + String(channel + 1) + ". ";
        DrawString((char*)channelId.c_str(), 1);
        NewLine(2);

        _currX = BUTTON_MARGIN_X * 2;
    }
}

void View::InitFlyScreen()
{
    InitGeneralScreen(false);
    
    _shouldRefreshScreen = true;

    // DropdownControl* input = new DropdownControl((char**)MIX_EDIT_INPUT_NAMES, 0, 50);
    // _mix_edit_controls[MIX_EDIT_INPUT] = input;
}


void View::DrawFlyScreen()
{
    if (!IsAnyControlDirty())
        return;
}

#endif