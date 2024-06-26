#include "declarations.h"
#include "init.h"
#include "draw.h"

OLED display(SCREEN_ADDRESS, OLED_SDA, OLED_SCL);
// pantallas
int currentScreen = SCREEN_CLOCK;
int previusScreen = -1;
// botones
bool pressSelect = false;
bool pressUp = false;
bool pressDown = false;
// items
int items[] = {3, 0, 1};

void setup() {
    Serial.begin(9600);
    Wire.begin(OLED_SDA, OLED_SCL);
    display.begin();
    display.clrScr(); 
    display.update();

    initWifi();
    initNtpClient();
    initButtons();
    initScreen();
}

void loop()
{
    if (currentScreen == SCREEN_CLOCK)
    {
        if (digitalRead(BUTTON_SELECT) == LOW)
        {
            pressSelect = true;
        }
        if (digitalRead(BUTTON_SELECT) == HIGH && pressSelect)
        {
            pressSelect = false;
            currentScreen = SCREEN_MENU;
        }
    }
    else if (currentScreen == SCREEN_MENU)
    {
        if (digitalRead(BUTTON_UP) == LOW)
        {
            pressUp = true;
        }
        if (digitalRead(BUTTON_UP) == HIGH && pressUp)
        {
            pressUp = false;
            items[1]--;
            if (items[1] < 0)
            {
                items[1] = NUM_ITEMS - 1;
            }
        }
        if (digitalRead(BUTTON_DOWN) == LOW)
        {
            pressDown = true;
        }
        if (digitalRead(BUTTON_DOWN) == HIGH && pressDown)
        {
            pressDown = false;
            items[1]++;
            if (items[1] >= NUM_ITEMS)
            {
                items[1] = 0;
            }
        }
        if (digitalRead(BUTTON_SELECT) == LOW)
        {
            pressSelect = true;
        }
        if (digitalRead(BUTTON_SELECT) == HIGH && pressSelect)
        {
            pressSelect = false;
            if (items[1] == NUM_ITEMS - 2)
            {
                currentScreen = SCREEN_CLOCK;
            }
        }
        items[0] = items[1] - 1;
        if (items[0] < 0)
        {
            items[0] = NUM_ITEMS - 1;
        }
        items[2] = items[1] + 1;
        if (items[2] >= NUM_ITEMS)
        {
            items[2] = 0;
        }
    }

    if (currentScreen != previusScreen)
    {
        if (currentScreen == SCREEN_CLOCK)
        {
            drawClock(true);
        }
        else if (currentScreen == SCREEN_MENU)
        {
            items[0] = 3;
            items[1] = 0;
            items[2] = 1;
            drawMenu(true, items);
        }
        previusScreen = currentScreen;
    }
    else
    {
        if (currentScreen == SCREEN_CLOCK)
        {
            drawClock(false);
        }
        else if (currentScreen == SCREEN_MENU)
        {
            drawMenu(false, items);
        }
    }
}