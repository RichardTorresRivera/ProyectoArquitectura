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
//  Cliente NTP
#include <NTPClient.h>
// Firebase
#include <FirebaseESP32.h>
//Json
#include <ArduinoJson.h>

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
// Led
#define LED 2
// Buzzer
#define BUZZER 4

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
extern WiFiUDP ntpUDP;
extern NTPClient ntpClient;

/****************************************************************
                       CONFIGURACION DE TELEGRAM
****************************************************************/

/****************************************************************
                       CONFIGURACION DE FECHAS
****************************************************************/
extern const char *daysOfTheWeek[7];
// Array de meses en español
extern const char *monthsOfTheYear[12];

/****************************************************************
                       PANTALLAS
****************************************************************/
#define SCREEN_CLOCK 0
#define SCREEN_MENU 1
#define SCREEN_ITEM 2
#define SCREEN_CALL 3
#define SCREEN_ALARM 4
#define SCREEN_WSP 5
#define SCREEN_BATTERY 6
#define SCREEN_LOAD 7

/****************************************************************
                       ITEMS
****************************************************************/
// Posicion en pantalla
#define ITEM_PREVIUS 0
#define ITEM_SELECTED 1
#define ITEM_NEXT 2

// Arreglo de items
const int NUM_ITEMS = 7;
const int MAX_ITEM_LENGTH = 20;
extern char menuItems[NUM_ITEMS][MAX_ITEM_LENGTH];
extern const unsigned char *icons_bitmaps[NUM_ITEMS];

// Numero de item (indice de array de menuItems)
#define ITEM_LINTERN 0
#define ITEM_MUSIC 1
#define ITEM_CHRONOMETER 2
#define ITEM_ALARM 3
#define ITEM_GAME 4
#define ITEM_SOUND 5
#define ITEM_EXIT 6

/****************************************************************
                       CONFIGURACION DE FIREBASE
****************************************************************/
#define FIREBASE_HOST "https://smart-13c8c-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "bg61NYKSEj2R6fmOgmZtFCHysEvdD5nlHlf7Fims"

// Crear instancias de Firebase y FirebaseData
extern FirebaseData firebaseData;
extern FirebaseAuth auth;
extern FirebaseConfig config;

#endif
