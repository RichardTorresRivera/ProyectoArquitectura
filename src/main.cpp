#include "declarations.h"
#include "iniciar/init.h"
#include "dibujar/draw.h"
#include "server/server.h"
#include "items/items.h"
#include "client/client.h"

// Tarea del nucleo 0
TaskHandle_t Load;
void loop0(void *parameter);

// Manejo de pantallas
byte currentScreen = SCREEN_CLOCK;
byte previusScreen = SCREEN_LOAD;
// Items
int items[] = {ITEM_LINTERN, ITEM_MUSIC, ITEM_CHRONOMETER};

// Botones
bool pressSelect = false;
bool pressUp = false;
bool pressDown = false;

// Tiempo cronometro
bool runningChronometer = false;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

// Musica
bool play_pause_music = true;

// Linterna
bool linter_on = false;

// Progreso
unsigned int progress = 0;

void setup()
{
    Serial.begin(9600);
    initWifi();
    Serial.println("-----------------------------");
    initFirebase();
    Serial.println("-----------------------------");
    initNtpClient();
    Serial.println("-----------------------------");
    initButtons();
    Serial.println("-----------------------------");
    initPines();
    Serial.println("-----------------------------");
    initScreen();
    Serial.println("-----------------------------");
    initServer(currentScreen);
    xTaskCreatePinnedToCore(loop0, "Load", 10000, NULL, 1, &Load, 0);
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
        else if (items[ITEM_SELECTED] == ITEM_LINTERN)
        {
            linter_on = !linter_on;
            if (linter_on)
            {
                Serial.println("Encendiendo linterna");
                digitalWrite(LED, HIGH);
            }
            else
            {
                Serial.println("Apagando linterna");
                digitalWrite(LED, LOW);
            }
        }
        else if (items[ITEM_SELECTED] == ITEM_SOUND)
        {
            currentScreen = SCREEN_LOAD;
            toneMaxMovil();
            currentScreen = SCREEN_ITEM;
        }
        else
        {
            currentScreen = SCREEN_ITEM;
        }
    }
    else if (currentScreen == SCREEN_ITEM)
    {
        switch (items[ITEM_SELECTED])
        {
        case ITEM_CHRONOMETER:
            // exit
            exitChronometer(runningChronometer, startTime, elapsedTime);
            currentScreen = SCREEN_MENU;
            break;
        case ITEM_MUSIC:
            // exit
            Serial.println("Saliendo de musica");
            currentScreen = SCREEN_MENU;
            break;
        case ITEM_ALARM:
            // exit
            Serial.println("Saliendo de alarma");
            currentScreen = SCREEN_MENU;
            break;
        case ITEM_GAME:
            // exit
            Serial.println("Saliendo de juego");
            currentScreen = SCREEN_MENU;
            break;
        case ITEM_SOUND:
            currentScreen = SCREEN_MENU;
            break;
        default:
            break;
        }
    }
    else if (currentScreen == SCREEN_CALL)
    {
        currentScreen = SCREEN_CLOCK;
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
            startPauseChronometer(runningChronometer, startTime, elapsedTime);
            break;
        case ITEM_MUSIC:
            playPauseMusic(play_pause_music);
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
        case ITEM_CHRONOMETER:
            // reset
            resetChronometer(runningChronometer, startTime, elapsedTime);
            break;
        case ITEM_MUSIC:
            nextMusic();
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
                items[ITEM_PREVIUS] = ITEM_LINTERN;
                items[ITEM_SELECTED] = ITEM_MUSIC;
                items[ITEM_NEXT] = ITEM_CHRONOMETER;
            }
            drawMenu(true, items);
            break;
        case SCREEN_ITEM:
            switch (items[ITEM_SELECTED])
            {
            case ITEM_CHRONOMETER:
                drawChronometer(true, elapsedTime);
                break;
            case ITEM_MUSIC:
                drawMusic(true);
                break;
            case ITEM_ALARM:
                drawAlarm(true);
                break;
            case ITEM_GAME:
                drawGame(true);
                break;
            case ITEM_SOUND:
                drawSoundMovil(true);
                break;
            default:
                break;
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
            switch (items[ITEM_SELECTED])
            {
            case ITEM_CHRONOMETER:
                drawChronometer(false, elapsedTime);
                break;
            case ITEM_MUSIC:
                drawMusic(false);
                break;
            case ITEM_ALARM:
                drawAlarm(false);
                break;
            case ITEM_GAME:
                drawGame(false);
                break;
            case ITEM_SOUND:
                drawSoundMovil(false);
                break;
            default:
                break;
            }
            break;
        }
    }
}

void loop()
{
    server.handleClient();
    handleButtonPress(BUTTON_SELECT, pressSelect, selectAction);
    handleButtonPress(BUTTON_UP, pressUp, upAction);
    handleButtonPress(BUTTON_DOWN, pressDown, downAction);

    if (runningChronometer)
    {
        elapsedTime = millis() - startTime;
    }

    updateScreen();
}

void loop0(void *parameter)
{
    while (true)
    {
        if (currentScreen == SCREEN_LOAD)
        {
            drawLoad(true, progress);
        }
        // espera de 100 ms
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}