#ifndef View_Screens_cpp
#define View_Screens_cpp
#include "View.h"

void View::InitMixSelectScreen()
{
    _header = String(currChannel + 1);
    _header = "Ch." + _header;

    InitGeneralScreen(false);
    
    DrawHeader((char*)_header.c_str(), true);
}

void View::DrawMixSelectScreen(const char* MIX_EDIT_INPUT_NAMES[])
{
    _currY = 30;
    _currX = BUTTON_MARGIN_X * 2;

    for (int mix = 0 ; mix < MIX_COUNT ; mix++)
    {
        if (_erasePreviousMarker && mix == _prevMarkedIndex)
            EraseMarker();

        if (mix == _markedIndex[currScreen])
            DrawMarker();

        tft.setTextColor(TFT_WHITE);
        _currY += 5;

        String mixId = String(mix + 1) + ". ";
        DrawString((char*)mixId.c_str(), 1);
        DrawString(MIX_EDIT_INPUT_NAMES[_settings -> mixes[currChannel * MIX_COUNT + mix] -> inputIndex], 1);
        DrawString(" (", 1);
        DrawPercent(_settings -> mixes[currChannel * MIX_COUNT + mix] -> weight, 1);
        DrawString(")", 1);
        NewLine(2);

        _currX = BUTTON_MARGIN_X * 2;
    }
}

void View::InitMixEditScreen(
    uint8_t page, 
    const char* MIX_EDIT_INPUT_NAMES[], const char* MIX_EDIT_BOOL_NAMES[], 
    const char* MIX_EDIT_SWITCH_NAMES[], const char* MIX_EDIT_BEHAVIOR_NAMES[])
{
    _header = String(currChannel + 1);
    _header = "Ch." + _header;
    _header = _header + " - Mix " + String(currMix + 1);

    InitGeneralScreen(false);
    
    DrawHeader((char*)_header.c_str(), true);

    _currPage = page;
    _totalPages = 2;
    
    switch (_currPage)
    {
        case 0:
            _currPageMinControlIndex = 0;
            _currPageMaxControlIndex = MIX_EDIT_PAGE1_LAST_CONTROL;
            _markedIndex[currScreen] = 0;
            break;
        case 1:
            _currPageMinControlIndex = MIX_EDIT_PAGE1_LAST_CONTROL + 1;
            _currPageMaxControlIndex = MIX_EDIT_CONTROL_COUNT - 1;
            _markedIndex[currScreen] = MIX_EDIT_PAGE1_LAST_CONTROL + 1;
            break;
    }

    if (_mix_edit_controls[0] == NULL)
        InitializeMixEditControls(MIX_EDIT_INPUT_NAMES, MIX_EDIT_BOOL_NAMES, MIX_EDIT_SWITCH_NAMES, MIX_EDIT_BEHAVIOR_NAMES);

    // Apply the currently edited mix settings to the controls on the screen
    SetSettings(_settings -> mixes[currChannel * MIX_COUNT + currMix]);

    _selectedControl = NULL;
    _erasePreviousMarker = true;
}

void View::InitializeMixEditControls(
    const char* MIX_EDIT_INPUT_NAMES[], const char* MIX_EDIT_BOOL_NAMES[], 
    const char* MIX_EDIT_SWITCH_NAMES[], const char* MIX_EDIT_BEHAVIOR_NAMES[])
{
    DropdownControl* input = new DropdownControl((char**)MIX_EDIT_INPUT_NAMES, 0, 50);
    _mix_edit_controls[MIX_EDIT_INPUT] = input;
    
    IntControl* weight = new IntControl(100, 1, -100, 100, 32, true);
    _mix_edit_controls[MIX_EDIT_WEIGHT] = weight;

    DropdownControl* inverted = new DropdownControl((char**)MIX_EDIT_BOOL_NAMES, 0, 20);
    _mix_edit_controls[MIX_EDIT_INVERTED] = inverted;

    IntControl* expo = new IntControl(0, 1, 0, 100, 32, true);
    _mix_edit_controls[MIX_EDIT_EXPO] = expo;

    DropdownControl* switch1 = new DropdownControl((char**)MIX_EDIT_SWITCH_NAMES, 0, 50);
    _mix_edit_controls[MIX_EDIT_SWITCH1] = switch1;

    DropdownControl* switch2 = new DropdownControl((char**)MIX_EDIT_SWITCH_NAMES, 0, 50);
    _mix_edit_controls[MIX_EDIT_SWITCH2] = switch2;

    IntControl* deadzone = new IntControl(0, 1, 0, 90, 32, true);
    _mix_edit_controls[MIX_EDIT_DEAD_ZONE] = deadzone;

    DropdownControl* behavior = new DropdownControl((char**)MIX_EDIT_BEHAVIOR_NAMES, 0, 68);
    _mix_edit_controls[MIX_EDIT_BEHAVIOUR] = behavior;

    IntControl* limitMin = new IntControl(-100, 1, -100, 100, 40, true);
    _mix_edit_controls[MIX_EDIT_LIMIT_MIN] = limitMin;

    IntControl* limitMax = new IntControl(100, 1, -100, 100, 40, true);
    _mix_edit_controls[MIX_EDIT_LIMIT_MAX] = limitMax;
}

void View::DrawMixEditScreen()
{
    if (!IsAnyControlDirty())
        return;

    _currY = 22;
    _currX = 2;

    if (_erasePreviousMarker)
    {
        EraseControl(*_mix_edit_controls[_prevMarkedIndex]);
        _erasePreviousMarker = false;
    }

    tft.setTextColor(TFT_WHITE);

    switch (_currPage)
    {
        case 0:
        {
            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_INPUT;
            DrawButton("Input:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_INPUT], _isCurrControlMarked && isInEditMode);
            NewLine(3);
            
            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_WEIGHT;
            DrawButton("Weight:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_WEIGHT], _isCurrControlMarked && isInEditMode);
            NewLine(3);

            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_INVERTED;
            DrawButton("Inverted:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_INVERTED], _isCurrControlMarked && isInEditMode);
            NewLine(3);

            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_EXPO;
            DrawButton("Expo:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_EXPO], _isCurrControlMarked && isInEditMode);
            NewLine(3);

            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_SWITCH1;
            DrawButton("Switch1:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_SWITCH1], _isCurrControlMarked && isInEditMode);
            NewLine(3);

            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_SWITCH2;
            DrawButton("Switch2:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_SWITCH2], _isCurrControlMarked && isInEditMode);
            NewLine(3);
            break;
        }
        case 1:
        {
            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_DEAD_ZONE;
            DrawButton("Dead Zone:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_DEAD_ZONE], _isCurrControlMarked && isInEditMode);
            NewLine(3);

            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_BEHAVIOUR;
            DrawButton("Behavior:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_BEHAVIOUR], _isCurrControlMarked && isInEditMode);
            NewLine(3);

            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_LIMIT_MIN;
            DrawButton("Limit[min]:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_LIMIT_MIN], _isCurrControlMarked && isInEditMode);
            NewLine(3);

            _isCurrControlMarked = _markedIndex[currScreen] == MIX_EDIT_LIMIT_MAX;
            DrawButton("Limit[max]:", _isCurrControlMarked);
            _currX = CONTROL_VALUE_X_POS;
            DrawControl(_mix_edit_controls[MIX_EDIT_LIMIT_MAX], _isCurrControlMarked && isInEditMode);
            //NewLine(3);
            break;
        }
    }
}

#endif