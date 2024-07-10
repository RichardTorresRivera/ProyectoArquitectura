#include "declarations.h"
#include "iniciar/init.h"
#include "dibujar/draw.h"
#include "server/server.h"
#include "items/items.h"
#include "client/client.h"

// Tarea del nucleo 0
TaskHandle_t Load;
TaskHandle_t Alarm;
void loop0(void *parameter);
void loop1(void *parameter);

// Manejo de pantallas
byte currentScreen = SCREEN_CLOCK;
byte previusScreen = SCREEN_LOAD;
// Items
int items[] = {ITEM_LINTERN, ITEM_MUSIC, ITEM_CHRONOMETER};
int items_sound[] = {SOUND_0, SOUND_50, SOUND_100};

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
unsigned short progress = 0;

// Alarma
AlarmTime alarma;
Task tasks;
bool alarmActivated = false;

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
    xTaskCreatePinnedToCore(loop0, "Load", 7000, NULL, 2, &Load, 0);
    xTaskCreatePinnedToCore(loop1, "Alarm", 7000, NULL, 2, &Alarm, 0);
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
        switch (items[ITEM_SELECTED])
        {
        case ITEM_EXIT:
            currentScreen = SCREEN_CLOCK;
            break;
        case ITEM_LINTERN:
            onOffLintern(linter_on);
            break;
        case ITEM_SOUND:
            currentScreen = SCREEN_MENU_SOUND;
            break;
        case ITEM_ALARM:
            currentScreen = SCREEN_LOAD;
            alarma = getAlarm();
            currentScreen = SCREEN_ITEM;
            progress = 0;
            break;
        case ITEM_TASK:
            currentScreen = SCREEN_LOAD;
            tasks = getTask();
            currentScreen = SCREEN_ITEM;
            progress = 0;
            break;
        default:
            currentScreen = SCREEN_ITEM;
            break;
        }
    }
    else if (currentScreen == SCREEN_MENU_SOUND)
    {
        if (items_sound[ITEM_SELECTED] == SOUND_EXIT)
        {
            currentScreen = SCREEN_MENU;
        }
        else
        {
            currentScreen = SCREEN_LOAD;
            toneMovil(items_sound[ITEM_SELECTED]);
            currentScreen = SCREEN_ITEM;
            progress = 0;
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
        default:
            currentScreen = SCREEN_MENU;
            break;
        }
    }
    else if (currentScreen == SCREEN_CALL || currentScreen == SCREEN_BATTERY || currentScreen == SCREEN_WSP || currentScreen == SCREEN_ALARM)
    {
        noTone(BUZZER);
        alarmActivated = false;
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
    else if (currentScreen == SCREEN_MENU_SOUND)
    {
        items_sound[ITEM_SELECTED]--;
        if (items_sound[ITEM_SELECTED] < 0)
        {
            items_sound[ITEM_SELECTED] = 3;
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
    else if (currentScreen == SCREEN_MENU_SOUND)
    {
        items_sound[ITEM_SELECTED]++;
        if (items_sound[ITEM_SELECTED] >= 4)
        {
            items_sound[ITEM_SELECTED] = 0;
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
        case SCREEN_ALARM:
            Serial.println("Alarma activada");
            drawAlarm(true, alarma.hour, alarma.minute);
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
        case SCREEN_MENU_SOUND:
            drawMenuSound(true, items_sound);
            break;
        case SCREEN_ITEM:
            switch (items[ITEM_SELECTED])
            {
            case ITEM_CHRONOMETER:
                // drawChronometer(true, elapsedTime);
                drawNotifications("Whats", "lorem ipsum y no se que mas, por que la vida es bella y bella es la vida");
                break;
            case ITEM_MUSIC:
                drawMusic(true);
                break;
            case ITEM_ALARM:
                drawAlarm(true, alarma.hour, alarma.minute, alarma.day);
                break;
            case ITEM_TASK:
                drawTask(true, tasks.dueDate, tasks.tasks);
                break;
            case ITEM_SOUND:
                drawSoundMovil(true, items_sound[ITEM_SELECTED]);
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
        case SCREEN_ALARM:
            drawAlarm(false, alarma.hour, alarma.minute);
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
        case SCREEN_MENU_SOUND:
            items_sound[ITEM_PREVIUS] = items_sound[ITEM_SELECTED] - 1;
            if (items_sound[ITEM_PREVIUS] < 0)
            {
                items_sound[ITEM_PREVIUS] = 3;
            }
            items_sound[ITEM_NEXT] = items_sound[ITEM_SELECTED] + 1;
            if (items_sound[ITEM_NEXT] >= 4)
            {
                items_sound[ITEM_NEXT] = 0;
            }
            drawMenuSound(false, items_sound);
            break;
        case SCREEN_ITEM:
            switch (items[ITEM_SELECTED])
            {
            case ITEM_CHRONOMETER:
                // drawChronometer(false, elapsedTime);
                drawNotifications("Whats", "lorem ipsum y no se que mas, por que la vida es bella y bella es la vida");
                break;
            case ITEM_MUSIC:
                drawMusic(false);
                break;
            case ITEM_ALARM:
                drawAlarm(false, alarma.hour, alarma.minute, alarma.day);
                break;
            case ITEM_TASK:
                drawTask(false, tasks.dueDate, tasks.tasks);
                break;
            case ITEM_SOUND:
                drawSoundMovil(false, items_sound[ITEM_SELECTED]);
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
    if (alarmActivated)
    {
        tone(BUZZER, 1200);
        delay(500);
        tone(BUZZER, 800);
        delay(500);
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
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void loop1(void *parameter)
{
    vTaskDelay(60000 / portTICK_PERIOD_MS);
    while (true)
    {
        if (isAlarmTime(ntpClient, alarma, alarmActivated))
        {
            currentScreen = SCREEN_ALARM;
        }
    }
}