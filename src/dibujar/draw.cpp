#include "declarations.h"
#include "draw.h"
#include "bitmaps.h"

void drawClock(bool fullRedraw)
{
    ntpClient.update();
    if (fullRedraw)
    {
        myOLED.clrScr();
        myOLED.setFont(SmallFont);
        myOLED.drawBitmap(26, 44, image_weather_cloud_sunny_bits, 17, 16);
        myOLED.drawLine(0, 15, 128, 15);
        myOLED.print("Clima:", 46, 53);
        myOLED.print("C", 96, 53);
    }
    // Dibujo de temperatura
    myOLED.setFont(SmallFont);
    // myOLED.clrRect(83, 53, 12, 8);
    myOLED.print("28", 83, 53);
    // Dibujo de fecha
    // myOLED.clrRect(16, 4, 128, 8);
    myOLED.print("Miercoles 26/06/24", 20, 4);
    // Dibujo de hora
    myOLED.setFont(arial_bold);
    // myOLED.clrRect(16, 27, 128, 16);
    myOLED.print(ntpClient.getFormattedTime(), 0, 27);
    myOLED.update();
}

void drawMenu(bool fullRedraw, int items[])
{
    if (fullRedraw)
    {
        myOLED.clrScr();
        myOLED.setFont(SmallFont);
        myOLED.drawRoundRect(2, 22, 124, 19);
    }
    // dibujar item previo
    myOLED.drawRoundRect(25, 2, 120, 8);
    myOLED.print(menuItems[items[0]], 25, 2);
    // dibujar item seleccionado
    myOLED.drawRoundRect(25, 22, 120, 8);
    myOLED.print(menuItems[items[1]], 25, 22);
    // dibujar item posterior
    myOLED.drawRoundRect(25, 42, 120, 8);
    myOLED.print(menuItems[items[2]], 25, 42);
    myOLED.update();
}

void drawChronometer(bool fullRedraw, unsigned long elapsedTime)
{
    if (fullRedraw)
    {
        myOLED.clrScr();
        myOLED.setFont(arial_bold);
        // dibujar indicadores de start/pause, salir y reset
    }
    myOLED.clrScr();
    unsigned int milliseconds = (elapsedTime % 1000) / 10;
    unsigned int seconds = (elapsedTime / 1000) % 60;
    unsigned int minutes = (elapsedTime / 60000) % 60;
    char formattedTime[12];
    sprintf(formattedTime, "%02d:%02d:%02d", minutes, seconds, milliseconds);
    myOLED.print(formattedTime, 20, 30);
    myOLED.update();
}
