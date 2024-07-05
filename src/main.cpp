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
// variable de musica
bool play_pause_music = true;
//  variable de linterna
bool linter_on = false;
// Peticiones HTTP
// const char *name = "Richi ttt";
// const char *num = "123456777";

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

void sendIFTTTRequest(String eventName)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        String url = "https://maker.ifttt.com/trigger/" + eventName + "/with/key/coIu6h1x2g7m_a3RP9gVM_";
        // Enviar peticion a IFTTT
        HTTPClient http;
        http.begin(url);
        Serial.println("[HTTP] GET...");
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK)
            {
                String payload = http.getString();
                Serial.println(payload);
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    else
    {
        Serial.println("WiFi not connected");
    }
}

void playPauseMusic()
{
    play_pause_music = !play_pause_music;
    if (play_pause_music)
    {
        // logica de play
        Serial.println("Play a music");
        sendIFTTTRequest("spotify_play");
    }
    else
    {
        // logica de pausa
        Serial.println("Pausa a music");
        sendIFTTTRequest("spotify_pause");
    }
}

void nextMusic()
{
    Serial.println("Siguiente cancion");
    sendIFTTTRequest("spotify_next");
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
            }
            else
            {
                Serial.println("Apagando linterna");
            }
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
            runningChronometer = false;
            startTime = 0;
            elapsedTime = 0;
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
        case ITEM_NOTIFICATIONS:
            // exit
            Serial.println("Saliendo de notificaciones");
            currentScreen = SCREEN_MENU;
            break;
        case ITEM_GAME:
            // exit
            Serial.println("Saliendo de juego");
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
            // run - pause
            runningChronometer = !runningChronometer;
            if (runningChronometer)
            {
                startTime = millis() - elapsedTime;
            }
            break;
        case ITEM_MUSIC:
            playPauseMusic();
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
            elapsedTime = 0;
            runningChronometer = false;
            startTime = millis();
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
    if (currentScreen != SCREEN_CALL)
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
                case ITEM_NOTIFICATIONS:
                    drawNotifications(true);
                    // drawCallMissed(name, num);
                    break;
                case ITEM_GAME:
                    drawGame(true);
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
                case ITEM_NOTIFICATIONS:
                    drawNotifications(false);
                    // drawCallMissed(name, num);
                    break;
                case ITEM_GAME:
                    drawGame(false);
                    break;
                default:
                    break;
                }
                break;
            }
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