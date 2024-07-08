#include "items.h"
#include "declarations.h"

void startPauseChronometer(bool &run, unsigned long &start, unsigned long &elapsed)
{
    // start - pause
    run = !run;
    if (run)
    {
        start = millis() - elapsed;
    }
}

void exitChronometer(bool &run, unsigned long &start, unsigned long &elapsed)
{
    run = false;
    start = 0;
    elapsed = 0;
}

void resetChronometer(bool &run, unsigned long &start, unsigned long &elapsed)
{
    elapsed = 0;
    run = false;
    start = millis();
}

void onOffLintern(bool &onLintern)
{
    onLintern = !onLintern;
    if (onLintern)
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