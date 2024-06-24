#include "declarations.h"
#include "init.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
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
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    Serial.println("Pantalla funcionando ...");
}