#include "declarations.h"
#include "draw.h"
#include "bitmaps.h"

void drawClock(bool fullRedraw)
{
    ntpClient.update();
    if (fullRedraw)
    {
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.drawLine(0, 15, 128, 15, 1);
        display.setTextSize(1);
        display.setCursor(46, 53);
        display.print("Clima:");
        display.drawBitmap(26, 51, image_cloud_bits, 17, 16, 1);
        display.setCursor(96, 53);
        display.print("C");
    }
    display.setTextSize(1);
    // Dibujo de temperatura
    display.fillRect(83, 53, 12, 8, BLACK);
    display.setCursor(83, 53);
    display.print("12");
    // Dibujo de fecha
    display.fillRect(16, 4, 128, 8, BLACK);
    display.setCursor(16, 4);
    display.print("Viernes 21/06/24");
    // Dibujo de hora
    display.setTextSize(2);
    display.fillRect(16, 27, 128, 16, BLACK);
    display.setCursor(16, 27);
    display.print(ntpClient.getFormattedTime());
    display.display();
}

void drawMenu(bool fullRedraw, int items[])
{
    if (fullRedraw)
    {
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.drawRect(2, 22, 124, 19, 1);
    }
    // dibujar item previo
    display.fillRect(25, 2, 120, 8, BLACK);
    display.setCursor(25, 2);
    display.print(menuItems[items[0]]);
    // dibujar item seleccionado
    display.fillRect(25, 22, 120, 8, BLACK);
    display.setCursor(25, 22);
    display.print(menuItems[items[1]]);
    // dibujar item posterior
    display.fillRect(25, 42, 120, 8, BLACK);
    display.setCursor(25, 42);
    display.print(menuItems[items[2]]);
    display.display();
}
