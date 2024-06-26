#include "declarations.h"
#include "init.h"

// Definición del display OLED
#define SCREEN_ADDRESS 0x3C  // Asegúrate de que esta es la dirección correcta para tu OLED

WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP, "south-america.pool.ntp.org", -5 * 3600, 60000);
char menuItems[NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"Cronometro"},
    {"Musica"},
    {"Salir"},
    {"Notificaciones"}};

void initWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWD, 6);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Conectandose a la red Wi-Fi...");
    }
    Serial.println("Conexion exitosa\n");
}

void initNtpClient()
{
    ntpClient.begin();
}

void initButtons()
{
    pinMode(BUTTON_SELECT, INPUT_PULLUP);
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    Serial.println("Botones incializados ...");
}

void initScreen()
{
    display.begin();
    display.clrScr();
}
