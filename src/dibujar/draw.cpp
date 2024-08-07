#include "declarations.h"
#include "draw.h"
#include "bitmaps.h"

// Auxiliares

int getTextWidth(String text)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    return w;
}

void displayTextWrapped(String text, int x, int y, int maxWidth)
{
    int cursorX = x;
    int cursorY = y;

    // Separa el texto en palabras
    int start = 0;
    int end = text.indexOf(' ', start);

    while (end != -1)
    {
        String word = text.substring(start, end + 1);
        int wordWidth = display.width() - cursorX;
        int wordWidthPixels = getTextWidth(word);

        if (wordWidthPixels > wordWidth)
        {
            // Si la palabra no cabe, mover a la siguiente línea
            cursorX = x;
            cursorY += 10; // Desplazamiento en Y (altura de la línea) puede necesitar ajuste
        }

        if (cursorY >= 50)
        {
            display.setCursor(17, 50);
            display.print("... continua ...");
            return;
        }
        // Dibujar la palabra
        display.setCursor(cursorX, cursorY);
        display.print(word);
        cursorX += wordWidthPixels;

        // Ir a la siguiente palabra
        start = end + 1;
        end = text.indexOf(' ', start);
    }

    // Imprimir la última palabra
    String lastWord = text.substring(start);
    display.setCursor(cursorX, cursorY);
    display.print(lastWord);
}

void drawCruz(int x, int y)
{
    display.drawBitmap(x, y, image_cruz_bits, 3, 3, 1);
}

void drawFrame()
{
    drawCruz(2, 2);
    drawCruz(123, 2);
    drawCruz(2, 16);
    drawCruz(123, 16);
    drawCruz(2, 59);
    drawCruz(123, 59);
    display.drawLine(6, 17, 121, 17, 1);
    display.drawLine(3, 6, 3, 14, 1);
    display.drawLine(124, 6, 124, 14, 1);
    display.drawLine(6, 3, 121, 3, 1);
    display.drawLine(3, 20, 3, 57, 1);
    display.drawLine(124, 20, 124, 57, 1);
    display.drawLine(6, 60, 121, 60, 1);
}

void drawRectMenu()
{
    display.drawRoundRect(2, 2, 119, 18, 2, 1);
    display.drawRoundRect(2, 22, 119, 20, 2, 1);
    display.drawRect(3, 23, 117, 18, 1);
    display.drawRoundRect(2, 44, 119, 18, 2, 1);
}

void centerText(String text, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int x = (SCREEN_WIDTH - w) / 2;
    display.setCursor(x, y);
    display.print(text);
}

// Pantallas

void drawClock(bool fullRedraw)
{
    ntpClient.update();
    if (fullRedraw)
    {
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        drawFrame();
        // Dibujo de clima
        display.drawBitmap(26, 47, image_cloud_bits, 17, 16, 1);
        display.setCursor(46, 49);
        display.print("Clima:");
        display.setCursor(96, 49);
        display.print("C");
        // Dibujo de fecha
        unsigned long epochTime = ntpClient.getEpochTime();
        struct tm *ptm = gmtime((time_t *)&epochTime);
        int day = ptm->tm_mday;
        int month = ptm->tm_mon;
        int year = ptm->tm_year + 1900;
        int weekday = ptm->tm_wday;
        display.setCursor(26, 7);
        display.printf("%s %02d-%s-%02d", daysOfTheWeek[weekday], day, monthsOfTheYear[month], year % 100);
    }
    display.setTextSize(1);
    // Dibujo de temperatura
    display.fillRect(83, 49, 12, 8, BLACK);
    display.setCursor(83, 49);
    display.print("28");
    // Dibujo de hora
    display.setTextSize(2);
    display.fillRect(16, 27, 98, 16, BLACK);
    display.setCursor(17, 27);
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
        drawRectMenu();
    }
    // dibujar barra
    display.fillRect(123, 0, 3, 64, BLACK);
    display.drawLine(124, 2, 124, 61, 1);
    display.drawRect(123, 64 / NUM_ITEMS * items[ITEM_PREVIUS] + 1, 3, 64 / NUM_ITEMS, 1);
    // dibujar item previo
    display.fillRect(4, 4, 114, 15, BLACK);
    display.drawBitmap(6, 5, icons_bitmaps[items[0]], 17, 12, 1);
    display.setCursor(27, 7);
    display.print(menuItems[items[0]]);
    // dibujar item seleccionado
    display.fillRect(4, 24, 114, 16, BLACK);
    display.drawBitmap(6, 26, icons_bitmaps[items[1]], 17, 12, 1);
    display.setCursor(27, 28);
    display.print(menuItems[items[1]]);
    // dibujar item posterior
    display.fillRect(4, 46, 114, 15, BLACK);
    display.drawBitmap(6, 47, icons_bitmaps[items[2]], 17, 12, 1);
    display.setCursor(27, 49);
    display.print(menuItems[items[2]]);
    display.display();
}

void drawCallMissed(const String contact, const String number)
{
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.drawRect(2, 2, 124, 60, 1);
    display.setCursor(20, 7);
    display.print("LLAMADA PERDIDA");
    display.drawBitmap(57, 20, image_phone_not_connected_bits, 15, 16, 1);
    // Contacto
    centerText(contact, 39);
    // Número
    centerText(number, 49);
    display.display();
}

void drawNotifications(const String app, const String title, const String msg)
{
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    drawFrame();
    centerText(app, 7);
    display.setCursor(10, 10);
    display.print(title);
    displayTextWrapped(msg, 10, 30, 19);
    display.display();
}

void drawBatteryLow(const String n)
{
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    drawFrame();
    display.setCursor(30, 7);
    display.print("BATERIA BAJA");
    display.drawBitmap(51, 25, image_battery_33_bits, 24, 16, 1);
    display.setCursor(19, 46);
    display.printf("Porcentaje: %s%%", n);
    display.display();
}

void drawLoad(bool fullRedraw, unsigned short &progress)
{
    if (fullRedraw)
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(25, 50);
        display.print(F("Cargando ..."));
        display.drawRect(0, 10, 128, 20, 1);
    }
    // Dibujar la barra de progreso
    display.fillRect(10, 15, progress, 10, 1);
    // Actualizar la pantalla con todo lo dibujado
    display.display();
    // Actualizar el progreso
    if (progress < 108)
    {
        progress += 10;
    }
    else
    {
        progress = 0;
    }
}

// Items

void drawChronometer(bool fullRedraw, unsigned long elapsedTime)
{
    if (fullRedraw)
    {
        display.clearDisplay();
        drawFrame();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(35, 7);
        display.print("Cronometro");
        // pausa
        display.drawBitmap(35, 40, image_icon_pausa_bits, 11, 11, 1);
        display.drawBitmap(31, 53, str_pausa_bits, 19, 5, 1);
        // exit
        display.drawBitmap(59, 40, image_icon_exit_bits, 11, 11, 1);
        display.drawBitmap(55, 53, str_exit_bits, 19, 5, 1);
        // reset
        display.drawBitmap(83, 40, image_icon_reset_bits, 11, 11, 1);
        display.drawBitmap(79, 53, str_reset_bits, 19, 5, 1);
    }
    // Dibujo de la cuenta
    display.fillRect(16, 21, 98, 16, BLACK);
    unsigned int milliseconds = (elapsedTime % 1000) / 10;
    unsigned int seconds = (elapsedTime / 1000) % 60;
    unsigned int minutes = (elapsedTime / 60000) % 60;
    display.setTextSize(2);
    display.setCursor(17, 22);
    display.printf("%02d:%02d:%02d", minutes, seconds, milliseconds);
    display.display();
}

void drawMusic(bool fullRedraw)
{
    if (fullRedraw)
    {
        display.clearDisplay();
        drawFrame();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(43, 7);
        display.print("Spotify");
        // pausa
        display.drawBitmap(32, 27, image_music_play_button_bits, 15, 16, 1);
        display.drawBitmap(30, 46, str_pausa_bits, 19, 5, 1);
        // exit
        display.drawBitmap(57, 27, image_music_exit_bits, 15, 16, 1);
        display.drawBitmap(55, 46, str_exit_bits, 19, 5, 1);
        // next
        display.drawBitmap(82, 27, image_music_next_bits, 15, 16, 1);
        display.drawBitmap(80, 46, str_next_bits, 19, 5, 1);
    }
    display.display();
}

void drawAlarm(bool fullRedraw, const String hour, const String minute, const String day)
{
    if (fullRedraw)
    {
        display.clearDisplay();
        drawFrame();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(43, 7);
        display.print("Alarma");
        // Verificar si hour y minute son cadenas vacías
        if (hour.isEmpty() || minute.isEmpty())
        {
            display.setCursor(10, 25);
            display.print("No tienes una");
            display.setCursor(10, 35);
            display.print("alarma definida");
        }
        else
        {
            centerText(day, 44);
            // Mostrar la hora y minutos de la alarma
            display.setTextSize(2);
            display.setCursor(35, 27);
            display.printf("%02d:%02d", hour.toInt(), minute.toInt());
        }
    }

    display.display();
}

void drawTask(bool fullRedraw, const String dueDate, const std::vector<String> tasks)
{
    if (fullRedraw)
    {
        display.clearDisplay();
        drawFrame();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(42, 7);
        display.print("Tareas");
        // Verificar si el vector de tareas está vacío
        if (tasks.empty())
        {
            display.setCursor(10, 25);
            display.print("No hay tareas");
            display.setCursor(10, 35);
            display.print("asignadas");
        }
        else
        {
            // Mostrar la fecha de vencimiento
            display.setCursor(10, 20);
            display.print("Fecha: " + dueDate);
            // Mostrar las tareas
            int y = 30;
            for (const auto &task : tasks)
            {
                display.setCursor(10, y);
                display.print(task);
                y += 10; // Incrementar la coordenada Y para la siguiente tarea
                if (y >= 50)
                {
                    display.setCursor(17, y);
                    display.print("... continua ...");
                    break;
                }
            }
        }
    }
    display.display();
}

void drawMenuSound(bool fullRedraw, int items[])
{
    if (fullRedraw)
    {
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        drawRectMenu();
    }
    // dibujar barra
    display.fillRect(123, 0, 3, 64, BLACK);
    display.drawLine(124, 2, 124, 61, 1);
    display.drawRect(123, 64 / 4 * items[0] + 1, 3, 64 / 4, 1);
    // dibujar item previo
    display.fillRect(4, 4, 114, 15, BLACK);
    display.drawBitmap(6, 5, icons_sound[items[0]], 17, 12, 1);
    display.setCursor(27, 7);
    display.print(menuSounds[items[0]]);
    // dibujar item seleccionado
    display.fillRect(4, 24, 114, 16, BLACK);
    display.drawBitmap(6, 26, icons_sound[items[1]], 17, 12, 1);
    display.setCursor(27, 28);
    display.print(menuSounds[items[1]]);
    // dibujar item posterior
    display.fillRect(4, 46, 114, 15, BLACK);
    display.drawBitmap(6, 47, icons_sound[items[2]], 17, 12, 1);
    display.setCursor(27, 49);
    display.print(menuSounds[items[2]]);
    display.display();
}

void drawSoundMovil(bool fullRedraw, short n)
{
    if (fullRedraw)
    {
        display.clearDisplay();
        display.setTextColor(1);
        display.drawRect(2, 2, 124, 60, 1);
        display.setCursor(16, 27);
        display.print("Se establecio el");
        display.setCursor(14, 38);
        display.print("tono del movil al");
        display.setCursor(54, 48);
        char msg[4];
        sprintf(msg, "%0d%%", n * 50);
        display.print(msg);
        display.drawBitmap(53, 7, image_volume_loud_bits, 20, 16, 1);
    }
    display.display();
}