#include "declarations.h"
#include "init.h"

OLED myOLED(OLED_SDA, OLED_SCL);
WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP, "south-america.pool.ntp.org", -5 * 3600, 60000);
char menuItems[NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"Notificaciones"},
    {"Musica"},
    {"Cronometro"},
    {"Salir"}};

void initWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWD, 6);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Conectandose a la red Wi-Fi...");
    }
    Serial.println("Conexion exitosa");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
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
    if (!myOLED.begin(SSD1306_128X64))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    Serial.println("Pantalla funcionando ...");
    myOLED.clrScr();
}