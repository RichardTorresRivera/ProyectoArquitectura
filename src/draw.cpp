#include "declarations.h"
#include "draw.h"


void drawClock(bool fullRedraw)
{
    ntpClient.update();
    if (fullRedraw)
    {
        display.clrScr();
        display.drawBitmap(0, 0, weather, 128, 64);
        display.setFont(SmallFont);
        display.drawLine(0, 15, 128, 15);
        display.print("Clima:", 46, 53);
        display.print("C", 96, 53);
    }
    // Dibujo de temperatura
    display.setFont(SmallFont);
    display.print("12", 83, 53);

    // Dibujo de fecha
    display.print("Martes 25/06/24", 20, 4);

    // Dibujo de hora
    display.setFont(arial_bold);
    display.print(ntpClient.getFormattedTime(), 0, 27);

    display.update();
}

void drawMenu(bool fullRedraw, int items[])
{
    if (fullRedraw)
    {
        display.clrScr();
        display.setFont(SmallFont);
        display.drawRoundRect(2, 22, 124, 19);
    }
    // dibujar item previo
    display.drawRoundRect(25, 2, 120, 8);
    display.print(menuItems[items[0]], 25, 2);
    
    // dibujar item seleccionado
    display.drawRoundRect(25, 22, 120, 8);
    display.print(menuItems[items[1]], 25, 22);
    
    // dibujar item posterior
    display.drawRoundRect(25, 42, 120, 8);
    display.print(menuItems[items[2]], 25, 42);
    
    display.update();
}
