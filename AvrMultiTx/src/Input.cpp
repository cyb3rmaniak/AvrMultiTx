#include "Input.h"

USB     _Usb;
PS4USB  _PS4(&_Usb);

// Public methods
Input::Input()
{
    for (int i = 0 ; i < 4 ; i++)
        _ps4AnalogValuesPercent[i] = CHANNEL_PERCENT_ON_START;
}

void Input::Initialize(View* view, Settings* settings)
{
    // Save a pointer to the view in case we need to display some message
    _view = view;

    // Save a pointer to the settings for when the user updates the controls and wants to save the settings
    _settings = settings;

    // Initialize the PS4 controller
    if (_Usb.Init() == -1)
    {
        Serial.println("\nError: PS4 USB Library OSC did not start. Stopping run.");
        _view -> ShowError("\nError: PS4 USB Library OSC did not start. Stopping run.");
        while (1); // Halt
    }
    //Serial.print(F("\r\nPS4 USB Library Started"));

    _prevWasControllerConnected = true;
}

void Input::ReadFromController(uint16_t currTime)
{
    _Usb.Task();

    if (_prevWasControllerConnected != _PS4.connected())
    {
        _view -> SetControllerStatus(_PS4.connected());
        _prevWasControllerConnected = _PS4.connected();
    }

    if (!_PS4.connected())
        return;

    // Read analog values
    _ps4AnalogValuesPercent[LeftHatX] = _PS4.getAnalogHat(LeftHatX) * ONE_OVER_255;
    _ps4AnalogValuesPercent[LeftHatY] = (255 - _PS4.getAnalogHat(LeftHatY)) * ONE_OVER_255;
    _ps4AnalogValuesPercent[RightHatX] = _PS4.getAnalogHat(RightHatX) * ONE_OVER_255;
    _ps4AnalogValuesPercent[RightHatY] = (255 - _PS4.getAnalogHat(RightHatY)) * ONE_OVER_255;

    CheckInput(currTime);
}

//============================================================================================
// Private Methods:
//============================================================================================

void Input::CheckInput(uint16_t currTime)
{
    if (_view -> IsAnyControlDirty())
        return;

    if (_PS4.getButtonClick(PS))
        _view -> MoveToScreen(SCREEN_MIX_SELECT, true);
    else if (_PS4.getButtonClick(CIRCLE))
        _view -> BackWasPressed();
    else if (_PS4.getButtonClick(CROSS))
        _view -> OKWasPressed();
    else if (_PS4.getButtonClick(UP) || (_view -> allowFastControl && _PS4.getButtonPress(UP)))
        _view -> DPadPress(UP);
    else if (_PS4.getButtonClick(RIGHT))
        _view -> DPadPress(RIGHT);
    else if (_PS4.getButtonClick(DOWN) || (_view -> allowFastControl && _PS4.getButtonPress(DOWN)))
        _view -> DPadPress(DOWN);
    else if (_PS4.getButtonClick(LEFT))
        _view -> DPadPress(LEFT);
    else if (_PS4.getButtonClick(L1))
        _view -> L1WasPressed();
    else if (_PS4.getButtonClick(R1))
        _view -> R1WasPressed();
    else if (_PS4.getButtonClick(OPTIONS))
        _settings -> SaveToEEPROM();
    else if (currTime > _nextTimeMenuInputIsAllowed)
    {
        if (_ps4AnalogValuesPercent[LeftHatY] > 0.55)
            _view -> DPadPress(UP);
        else if (_ps4AnalogValuesPercent[LeftHatY] < 0.45)
            _view -> DPadPress(DOWN);

        if (!_view -> isInEditMode || !_view -> allowFastControl)
            _nextTimeMenuInputIsAllowed = currTime + 180;
    }

    //Serial.println(_ps4AnalogValuesPercent[LeftHatY]);
    // if (PS4.getButtonClick(TRIANGLE)) 
    //     PS4.setRumbleOn(RumbleLow);
    //     PS4.setRumbleOn(RumbleHigh);
    // if (PS4.getButtonClick(CROSS))
    //     PS4.setLedFlash(10, 10); // Set it to blink rapidly
    // if (PS4.getButtonClick(SQUARE))
    //     PS4.setLedFlash(0, 0); // Turn off blinking
        // PS4.setLed(Red);
        // PS4.setLed(Blue);
        // PS4.setLed(Yellow);
        // PS4.setLed(Green);
}

// void ps4_actions()
// {
    // Set the outgoing values according to the channels mapped to each control...

    // if (ps4AnalogValues[LeftHatX] >= 117 && ps4AnalogValues[LeftHatX] <= 137)
    //     ppm_outbox[PS4_MAPPING_L_STICK_X] = CHANNEL_DEFAULT_VALUE;
    // else
    //     ppm_outbox[PS4_MAPPING_L_STICK_X] = map(ps4AnalogValues[LeftHatX], 0, 255, CHANNEL_MIN_VALUE, CHANNEL_MAX_VALUE);

    // if (ps4AnalogValues[LeftHatY] >= 117 && ps4AnalogValues[LeftHatY] <= 137)
    //     ppm_outbox[PS4_MAPPING_L_STICK_Y] = CHANNEL_DEFAULT_VALUE;
    // else
    //     ppm_outbox[PS4_MAPPING_L_STICK_Y] = map(ps4AnalogValues[LeftHatY], 0, 255, CHANNEL_MIN_VALUE, CHANNEL_MAX_VALUE);

    // if (ps4AnalogValues[RightHatX] >= 117 && ps4AnalogValues[RightHatX] <= 137)
    //     ppm_outbox[PS4_MAPPING_R_STICK_X] = CHANNEL_DEFAULT_VALUE;
    // else
    //     ppm_outbox[PS4_MAPPING_R_STICK_X] = map(ps4AnalogValues[RightHatX], 0, 255, CHANNEL_MIN_VALUE, CHANNEL_MAX_VALUE);

    // if (ps4AnalogValues[RightHatY] >= 117 && ps4AnalogValues[RightHatY] <= 137)
    //     ppm_outbox[PS4_MAPPING_R_STICK_Y] = CHANNEL_DEFAULT_VALUE;
    // else
    //     ppm_outbox[PS4_MAPPING_R_STICK_Y] = map(ps4AnalogValues[RightHatY], 0, 255, CHANNEL_MIN_VALUE, CHANNEL_MAX_VALUE);

// if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117) 
// {
//     Serial.print(F("\r\nLeftHatX: "));
//     Serial.print(PS4.getAnalogHat(LeftHatX));
//     Serial.print(F("\tLeftHatY: "));
//     Serial.print(PS4.getAnalogHat(LeftHatY));
//     Serial.print(F("\tRightHatX: "));
//     Serial.print(PS4.getAnalogHat(RightHatX));
//     Serial.print(F("\tRightHatY: "));
//     Serial.print(PS4.getAnalogHat(RightHatY));
// }

// if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller
//   Serial.print(F("\r\nL2: "));
//   Serial.print(PS4.getAnalogButton(L2));
//   Serial.print(F("\tR2: "));
//   Serial.print(PS4.getAnalogButton(R2));
// }
// if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different
//   PS4.setRumbleOn(PS4.getAnalogButton(L2), PS4.getAnalogButton(R2));
// oldL2Value = PS4.getAnalogButton(L2);
// oldR2Value = PS4.getAnalogButton(R2);

// if (PS4.getButtonClick(PS))
//     Serial.print(F("\r\nPS"));
// if (PS4.getButtonClick(TRIANGLE)) {
//     Serial.print(F("\r\nTraingle"));
//     PS4.setRumbleOn(RumbleLow);
// }
// if (PS4.getButtonClick(CIRCLE)) {
//     Serial.print(F("\r\nCircle"));
//     PS4.setRumbleOn(RumbleHigh);
// }
// if (PS4.getButtonClick(CROSS)) {
//     Serial.print(F("\r\nCross"));
//     PS4.setLedFlash(10, 10); // Set it to blink rapidly
// }
// if (PS4.getButtonClick(SQUARE)) {
//     Serial.print(F("\r\nSquare"));
//     PS4.setLedFlash(0, 0); // Turn off blinking
// }

// if (PS4.getButtonClick(UP)) {
//     Serial.print(F("\r\nUp"));
//     PS4.setLed(Red);
// } if (PS4.getButtonClick(RIGHT)) {
//     Serial.print(F("\r\nRight"));
//     PS4.setLed(Blue);
// } if (PS4.getButtonClick(DOWN)) {
//     Serial.print(F("\r\nDown"));
//     PS4.setLed(Yellow);
// } if (PS4.getButtonClick(LEFT)) {
//     Serial.print(F("\r\nLeft"));
//     PS4.setLed(Green);
// }

// if (PS4.getButtonClick(L1))
//     Serial.print(F("\r\nL1"));
// if (PS4.getButtonClick(L3))
//     Serial.print(F("\r\nL3"));
// if (PS4.getButtonClick(R1))
//     Serial.print(F("\r\nR1"));
// if (PS4.getButtonClick(R3))
//     Serial.print(F("\r\nR3"));

// if (PS4.getButtonClick(SHARE))
//   Serial.print(F("\r\nShare"));
// if (PS4.getButtonClick(OPTIONS)) {
//   Serial.print(F("\r\nOptions"));
//   printAngle = !printAngle;
// }
// if (PS4.getButtonClick(TOUCHPAD)) {
//   Serial.print(F("\r\nTouchpad"));
//   printTouch = !printTouch;
// }

// if (printAngle) { // Print angle calculated using the accelerometer only
//   Serial.print(F("\r\nPitch: "));
//   Serial.print(PS4.getAngle(Pitch));
//   Serial.print(F("\tRoll: "));
//   Serial.print(PS4.getAngle(Roll));
// }

// if (printTouch) { // Print the x, y coordinates of the touchpad
//   if (PS4.isTouching(0) || PS4.isTouching(1)) // Print newline and carriage return if any of the fingers are touching the touchpad
//     Serial.print(F("\r\n"));
//   for (uint8_t i = 0; i < 2; i++) { // The touchpad track two fingers
//     if (PS4.isTouching(i)) { // Print the position of the finger if it is touching the touchpad
//       Serial.print(F("X")); Serial.print(i + 1); Serial.print(F(": "));
//       Serial.print(PS4.getX(i));
//       Serial.print(F("\tY")); Serial.print(i + 1); Serial.print(F(": "));
//       Serial.print(PS4.getY(i));
//       Serial.print(F("\t"));
//     }
//   }
// }
//}

// Private methods:
