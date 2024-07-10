#include "buttons.h"

void handleButtonPress(byte button, bool &pressFlag, void (*action)())
{
    if (digitalRead(button) == LOW)
    {
        pressFlag = true;
    }
    if (digitalRead(button) == HIGH && pressFlag)
    {
        pressFlag = false;
        action();
    }
}

void selectAction(byte &currentScreen)
{
    switch (currentScreen)
    {
    case SCREEN_CLOCK:
        currentScreen = SCREEN_MENU;
        break;
    case SCREEN_MENU:

        break;
    default:
        break;
    }
}