#ifndef DECLARATIONS_H
#define DECLARATIONS_H

/****************************************************************
                       LIBRERIAS NECESARIAS
****************************************************************/
// Comunicacions 12C
#include <Wire.h>
// Manejo de graficos
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Conexion a Wifi
#include <WiFi.h>
#include <WiFiUdp.h>
// Cliente NTP
#include <NTPClient.h>

/****************************************************************
                       PINES DE HARDWARE
****************************************************************/
// Pantalla OLED
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RESET -1
// BOTONES
#define BUTTON_SELECT 12
#define BUTTON_UP 13
#define BUTTON_DOWN 14

/****************************************************************
                        CONFIGURACION DE PANTALLA
****************************************************************/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
extern Adafruit_SSD1306 display;

/****************************************************************
                       CONFIGURACION DE WIFI
****************************************************************/
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWD ""
extern WiFiUDP ntpUDP;
extern NTPClient ntpClient;

/****************************************************************
                       PANTALLAS
****************************************************************/
#define SCREEN_CLOCK 0
#define SCREEN_MENU 1
#define SCREEN_CALL 2

/****************************************************************
                       ITEMS
****************************************************************/
const int NUM_ITEMS = 4;
const int MAX_ITEM_LENGTH = 20;
extern char menuItems[NUM_ITEMS][MAX_ITEM_LENGTH];

#endif
