#include "Input.h"
#include "Control.h"

// Public methods
View::View()
{
    currScreen = SCREEN_CHANNEL_TESTER;
}

void View::Initialize(int channelCount, Settings* settings)
{
    _channelCount = channelCount;
    _settings = settings;

    // Initialize the TFT display
    tft.init();
    tft.setRotation(3);
    tft.drawPixel(tft.width() - 1, tft.height() - 1, TFT_NAVY); // Needed to clear the screen buffer for some reason...

    InitChannelTesterScreen();
}

void View::Refresh()
{
    DrawCurrScreen();
}

void View::DPadPress(ButtonEnum button)
{
    switch (currScreen)
    {
        default:
            return;
    }
}

void View::OKWasPressed()
{
    switch (currScreen)
    {
        default:
            return;
    }
}

void View::BackWasPressed()
{
    if (currScreen == 0)
        return;
            
    MoveBackToPrevScreen();
}

void View::L1WasPressed()
{
    switch (currScreen)
    {
        default:
            return;
    }
}

void View::R1WasPressed()
{
    switch (currScreen)
    {
        default:
            return;
    }
}

void View::MoveToScreen(uint8_t screen, bool keepHistory)
{
    _shouldRefreshScreen = true;

    if (keepHistory)
    {
        _currHistoryIndex++;
        _prevScreen[_currHistoryIndex] = currScreen;
    }

    currScreen = screen;
    
    InitCurrScreen();
    DrawCurrScreen();
}

void View::MoveBackToPrevScreen()
{
    MoveToScreen(_prevScreen[_currHistoryIndex], false);

    if (_currHistoryIndex > 0)
        _currHistoryIndex--;
}

void View::ShowError(char* message)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);  
    tft.setTextSize(1);
    tft.drawCentreString(message, 80, 50, 2);
}

void View::SetControllerStatus(bool isConnected)
{
    tft.setTextSize(1);
    if (isConnected)
    {
        // Clear previous text
        tft.setTextColor(TFT_BLACK, TFT_BLACK);
        tft.drawString("Controller NOT connected", 70, 118, 1);
    }
    else
    {
        // Show message
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
        tft.drawString("Controller NOT connected", 70, 118, 1);
    }
}

bool View::IsAnyControlDirty()
{
    if (_shouldRefreshScreen)
        return true;

    // TODO - Check if controls are dirty

    return false;
}

// =================================================================================
// Private methods:
// =================================================================================

void View::InitCurrScreen()
{
    switch (currScreen)
    {
        case SCREEN_CHANNEL_TESTER:
            InitChannelTesterScreen();
            break;
        case SCREEN_FLY:
            InitFlyScreen();
            break;
    }
}

void View::DrawCurrScreen()
{
    switch (currScreen)
    {
        case SCREEN_CHANNEL_TESTER:
            DrawChannelTesterScreen();
            break;
        case SCREEN_FLY:
            DrawFlyScreen();
            break;
    }
}

void View::InitGeneralScreen(bool drawDefaultHeader)
{
    tft.fillScreen(TFT_BLACK);
    
    _currX = 2;
    _currY = 2;
    
    if (drawDefaultHeader)
        DrawHeader((char*)SCREEN_NAMES[currScreen], false);
}

void View::DrawHeader(char* header, bool drawL1R1Controls)
{
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    if (drawL1R1Controls)
    {
        tft.drawRect(0, 0, 35, 13, TFT_WHITE);
        tft.drawString("< L1", 3, 3, 1);
        tft.drawRect(125, 0, 35, 13, TFT_WHITE);
        tft.drawString("R1 >", 128, 3, 1);
    }

    tft.drawCentreString(header, 80, 0, 2);

    _currY = 12;
}

void View::DrawButton(char* text, bool isMarked)
{
    _currY -= BUTTON_HEIGHT * 0.3;

    if (isMarked)
        DrawButton(BUTTON_MARGIN_X, _currY , BUTTON_WIDTH, BUTTON_HEIGHT, text, 2, TFT_YELLOW, 0x00002, TFT_YELLOW, false, 0x00002);
    else
        DrawButton(BUTTON_MARGIN_X, _currY, BUTTON_WIDTH, BUTTON_HEIGHT, text, 2, TFT_WHITE, TFT_BLACK, TFT_LIGHTGREY, false, TFT_BLACK);
}

void View::DrawButton(int16_t x, int16_t y, int16_t width, int16_t height, char* text, uint8_t font, uint16_t textColor, uint16_t bgColor, uint16_t buttonLineColor, bool shouldFill, uint16_t buttonBgColor)
{
    if (shouldFill)
        tft.fillRoundRect(x, y, width, height, 5, buttonBgColor);
    
    tft.drawRoundRect(x, y, width, height, 5, buttonLineColor);

    if (bgColor == buttonBgColor)
        tft.setTextColor(textColor);
    else
        tft.setTextColor(textColor, bgColor);
    
    tft.drawString(text, x + 6, y, font);
}

void View::DrawString(char* content, int font)
{
    tft.drawString(content, _currX, _currY, font);
    _currX += strlen(content) * 6;
}

void View::NewLine(int font)
{
    switch (font)
    {
        case 1:
            _currY += 10;
            break;
        case 2:
            _currY += 14;
            break;
        case 3:
            _currY += BUTTON_HEIGHT + BUTTON_MARGIN_Y;
            break;
        default:
            _currY += 9;
    }
    _currX = 2;
}

void View::DrawControl(Control* control, bool isSelected)
{
    control -> prevX = _currX;
    control -> prevY = _currY;

    if (isSelected)
    {
        tft.drawRect(_currX - 4, _currY + 1, control -> width + 8, BUTTON_HEIGHT - 2, TFT_CYAN);
        tft.setTextColor(TFT_YELLOW);
    }

    switch (control -> GetControlType())
    {
        // case CONTROL_TYPE_INT:
        // {
        //     IntControl* intControl = (IntControl*)control;
        //     if (intControl -> showPercent)
        //         DrawPercent(intControl -> GetValue(), 2);
        //     else
        //         DrawString(control -> ToString(), 2);
        //     break;
        // }
        // case CONTROL_TYPE_DROPDOWN:
        // {
        //     DropdownControl* dropControl = (DropdownControl*)control;
        //     DrawString(dropControl -> ToString(), 2);
        //     break;
        // }
    }

    control -> isDirty = false;

    tft.setTextColor(TFT_WHITE);
}

void View::EraseControl(Control control)
{
    int tempX = _currX;
    int tempY = _currY;

    _currX = control.prevX;
    _currY = control.prevY;

    tft.fillRect(_currX - 4, _currY + 1, control.width + 8, BUTTON_HEIGHT - 2, TFT_BLACK);

    _currX = tempX;
    _currY = tempY;
}