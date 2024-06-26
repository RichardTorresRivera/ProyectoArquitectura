#ifndef DECLARATIONS_H
#define DECLARATIONS_H

/****************************************************************
                       LIBRERIAS NECESARIAS
****************************************************************/
// Comunicacions 12C
#include <Wire.h>
// Manejo de graficos
#include "OLED_I2C.h"
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
extern OLED myOLED;
// Fuentes
extern uint8_t SmallFont[];
extern uint8_t arial_bold[];

/****************************************************************
                       CONFIGURACION DE WIFI
****************************************************************/
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWD ""
extern WiFiUDP ntpUDP;
extern NTPClient ntpClient;

/****************************************************************
                       CONFIGURACION DE TELEGRAM
****************************************************************/

/****************************************************************
                       PANTALLAS
****************************************************************/
#define SCREEN_CLOCK 0
#define SCREEN_MENU 1
#define SCREEN_ITEM 2
#define SCREEN_CALL 3

/****************************************************************
                       ITEMS
****************************************************************/
// Posicion en pantalla
#define ITEM_PREVIUS 0
#define ITEM_SELECTED 1
#define ITEM_NEXT 2
// Arreglo de items
const int NUM_ITEMS = 4;
const int MAX_ITEM_LENGTH = 20;
extern char menuItems[NUM_ITEMS][MAX_ITEM_LENGTH];
// Numero de item
#define ITEM_MUSIC 0
#define ITEM_NOTIFICATIONS 1
#define ITEM_CHRONOMETER 2
#define ITEM_EXIT 3

#endif
