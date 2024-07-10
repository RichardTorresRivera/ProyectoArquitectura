#ifndef BUTTONS_H
#define BUTTONS_H

#include "declarations.h"

void handleButtonPress(byte button, bool &pressFlag, void (*action)());
void selectAction(byte &currentScreen);
void upAction(byte &currentScreen);
void downAction(byte &currentScreen);

#endif // BUTTONS_H