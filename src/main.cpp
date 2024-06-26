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
int items[] = {3, 0, 1};
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
            items[ITEM_SELECTED]--;
            if (items[ITEM_SELECTED] < 0)
            {
                items[ITEM_SELECTED] = NUM_ITEMS - 1;
            }
        }
        if (digitalRead(BUTTON_DOWN) == LOW)
        {
            pressDown = true;
        }
        if (digitalRead(BUTTON_DOWN) == HIGH && pressDown)
        {
            pressDown = false;
            items[ITEM_SELECTED]++;
            if (items[ITEM_SELECTED] >= NUM_ITEMS)
            {
                items[ITEM_SELECTED] = 0;
            }
        }
        if (digitalRead(BUTTON_SELECT) == LOW)
        {
            pressSelect = true;
        }
        if (digitalRead(BUTTON_SELECT) == HIGH && pressSelect)
        {
            pressSelect = false;
            if (items[ITEM_SELECTED] == ITEM_EXIT)
            {
                currentScreen = SCREEN_CLOCK;
            }
            else
            {
                if (items[ITEM_SELECTED] == ITEM_CHRONOMETER)
                {
                    currentScreen = SCREEN_ITEM;
                }
            }
        }
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
    }
    else if (currentScreen == SCREEN_ITEM)
    {
        if (items[ITEM_SELECTED] == ITEM_CHRONOMETER)
        {
            // Boton de start y pause
            if (digitalRead(BUTTON_UP) == LOW)
            {
                pressUp = true;
            }
            if (digitalRead(BUTTON_UP) == HIGH && pressUp)
            {
                pressUp = false;
                Serial.println("$$$$ START:PAUSE presionado");
                runningChronometer = !runningChronometer;
                if (runningChronometer)
                {
                    startTime = millis() - elapsedTime;
                }
            }
            // Boton de reset
            if (digitalRead(BUTTON_DOWN) == LOW)
            {
                pressDown = true;
            }
            if (digitalRead(BUTTON_DOWN) == HIGH && pressDown)
            {
                pressDown = false;
                Serial.println(">>>> RESET presionado");
                elapsedTime = 0;
                runningChronometer = false;
                startTime = millis();
            }
            // Boton de salir
            if (digitalRead(BUTTON_SELECT) == LOW)
            {
                pressSelect = true;
            }
            if (digitalRead(BUTTON_SELECT) == HIGH && pressSelect)
            {
                pressSelect = false;
                runningChronometer = false;
                startTime = 0;
                elapsedTime = 0;
                currentScreen = SCREEN_MENU;
            }
            if (runningChronometer)
            {
                elapsedTime = millis() - startTime;
            }
        }
    }
    // dibujar
    if (currentScreen != previusScreen)
    {
        if (currentScreen == SCREEN_CLOCK)
        {
            drawClock(true);
        }
        else if (currentScreen == SCREEN_MENU)
        {
            if (previusScreen != SCREEN_ITEM)
            {
                // Reinicio de visualizion de items
                items[ITEM_PREVIUS] = ITEM_MUSIC;
                items[ITEM_SELECTED] = ITEM_NOTIFICATIONS;
                items[ITEM_NEXT] = ITEM_CHRONOMETER;
            }
            drawMenu(true, items);
        }
        else if (currentScreen == SCREEN_ITEM)
        {
            if (items[ITEM_SELECTED] == ITEM_CHRONOMETER)
            { // cronometro
                drawChronometer(true, elapsedTime);
            }
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
        else if (currentScreen == SCREEN_ITEM)
        {
            if (items[ITEM_SELECTED] == ITEM_CHRONOMETER)
            { // cronometro
                drawChronometer(false, elapsedTime);
            }
        }
    }
}
