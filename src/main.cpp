#include "declarations.h"
#include "iniciar/init.h"
#include "dibujar/draw.h"

// pantallas
int currentScreen = SCREEN_CLOCK;
int previusScreen = -1;
// botones
bool pressSelect = false;
bool pressUp = false;
bool pressDown = false;
// items
int items[] = {ITEM_MUSIC, ITEM_NOTIFICATIONS, ITEM_CHRONOMETER};
// tiempo cronometro
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool runningChronometer = false;

void setup()
{
    Serial.begin(9600);
    initWifi();
    Serial.println("-----------------------------");
    initNtpClient();
    Serial.println("-----------------------------");
    initButtons();
    Serial.println("-----------------------------");
    initScreen();
    Serial.println("-----------------------------");
}

void handleButtonPress(int button, bool &pressFlag, void (*action)())
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

void selectAction()
{
    Serial.println("Boton Select apretado");
    if (currentScreen == SCREEN_CLOCK)
    {
        currentScreen = SCREEN_MENU;
    }
    else if (currentScreen == SCREEN_MENU)
    {
        if (items[ITEM_SELECTED] == ITEM_EXIT)
        {
            currentScreen = SCREEN_CLOCK;
        }
        else if (items[ITEM_SELECTED] == ITEM_CHRONOMETER)
        {
            currentScreen = SCREEN_ITEM;
        }
    }
    else if (currentScreen == SCREEN_ITEM)
    {
        switch (items[ITEM_SELECTED])
        {
        case ITEM_CHRONOMETER:
            elapsedTime = 0;
            runningChronometer = false;
            startTime = millis();
            break;
        default:
            break;
        }
    }
}

void upAction()
{
    Serial.println("Boton Up apretado");
    if (currentScreen == SCREEN_MENU)
    {
        items[ITEM_SELECTED]--;
        if (items[ITEM_SELECTED] < 0)
        {
            items[ITEM_SELECTED] = NUM_ITEMS - 1;
        }
    }
    else if (currentScreen == SCREEN_ITEM)
    {
        switch (items[ITEM_SELECTED])
        {
        case ITEM_CHRONOMETER:
            runningChronometer = !runningChronometer;
            if (runningChronometer)
            {
                startTime = millis() - elapsedTime;
            }
            break;
        default:
            break;
        }
    }
}

void downAction()
{
    Serial.println("Boton Down apretado");
    if (currentScreen == SCREEN_MENU)
    {
        items[ITEM_SELECTED]++;
        if (items[ITEM_SELECTED] >= NUM_ITEMS)
        {
            items[ITEM_SELECTED] = 0;
        }
    }
    else if (currentScreen == SCREEN_ITEM)
    {
        switch (items[ITEM_SELECTED])
        {
        // salir
        case ITEM_CHRONOMETER:
            runningChronometer = false;
            startTime = 0;
            elapsedTime = 0;
            currentScreen = SCREEN_MENU;
            break;
        default:
            break;
        }
    }
}

void updateScreen()
{
    if (currentScreen != previusScreen)
    {
        switch (currentScreen)
        {
        case SCREEN_CLOCK:
            drawClock(true);
            break;
        case SCREEN_MENU:
            if (previusScreen != SCREEN_ITEM)
            {
                // Reinicio de visualizion de items
                items[ITEM_PREVIUS] = ITEM_MUSIC;
                items[ITEM_SELECTED] = ITEM_NOTIFICATIONS;
                items[ITEM_NEXT] = ITEM_CHRONOMETER;
            }
            drawMenu(true, items);
            break;
        case SCREEN_ITEM:
            if (items[ITEM_SELECTED] == ITEM_CHRONOMETER)
            {
                drawChronometer(true, elapsedTime);
            }
            break;
        }
        previusScreen = currentScreen;
    }
    else
    {
        switch (currentScreen)
        {
        case SCREEN_CLOCK:
            drawClock(false);
            break;
        case SCREEN_MENU:
            items[ITEM_PREVIUS] = items[ITEM_SELECTED] - 1;
            if (items[ITEM_PREVIUS] < 0)
            {
                items[ITEM_PREVIUS] = NUM_ITEMS - 1;
            }
            items[ITEM_NEXT] = items[ITEM_SELECTED] + 1;
            if (items[ITEM_NEXT] >= NUM_ITEMS)
            {
                items[ITEM_NEXT] = 0;
            }
            drawMenu(false, items);
            break;
        case SCREEN_ITEM:
            if (items[ITEM_SELECTED] == ITEM_CHRONOMETER)
            {
                drawChronometer(false, elapsedTime);
            }
            break;
        }
    }
}

void loop()
{
    handleButtonPress(BUTTON_SELECT, pressSelect, selectAction);
    handleButtonPress(BUTTON_UP, pressUp, upAction);
    handleButtonPress(BUTTON_DOWN, pressDown, downAction);

    if (runningChronometer)
    {
        elapsedTime = millis() - startTime;
    }

    updateScreen();
}