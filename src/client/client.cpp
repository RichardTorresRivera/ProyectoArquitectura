#include "client.h"
#include "declarations.h"

void sendIFTTTRequest(const String &eventName)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        String url = "https://maker.ifttt.com/trigger/" + eventName + "/with/key/coIu6h1x2g7m_a3RP9gVM_";
        HTTPClient http;
        http.begin(url);
        Serial.println("[HTTP] GET...");
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK)
            {
                String payload = http.getString();
                Serial.println(payload);
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    else
    {
        Serial.println("WiFi not connected");
    }
}

void playPauseMusic(bool &play_pause_music)
{
    play_pause_music = !play_pause_music;
    if (play_pause_music)
    {
        // logica de play
        Serial.println("Play a music");
        sendIFTTTRequest("spotify_play");
    }
    else
    {
        // logica de pausa
        Serial.println("Pausa a music");
        sendIFTTTRequest("spotify_pause");
    }
}

void nextMusic()
{
    Serial.println("Siguiente cancion");
    sendIFTTTRequest("spotify_next");
}

void toneMaxMovil()
{
    Serial.println("tono del movil al maximo");
    sendIFTTTRequest("cell_tone");
}

void getAlarm()
{
    // Leer los datos de Firebase
    if (Firebase.ready())
    {
        String hour, minute;
        if (Firebase.getString(firebaseData, "proxAlarm/nextAlarm/hour"))
        {
            hour = firebaseData.stringData();
        }
        if (Firebase.getString(firebaseData, "proxAlarm/nextAlarm/minute"))
        {
            minute = firebaseData.stringData();
        }
        
        // Mostrar la hora y minuto en la pantalla
        display.setTextSize(2);
        display.setCursor(43, 27);
        display.printf("%02d:%02d", hour.toInt(), minute.toInt());
    }
}

void getTask() {
    // Limpiar la pantalla antes de mostrar las tareas
    display.clearDisplay();
    
    // Configurar el tamaño del texto
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    // Leer los datos de Firebase
    if (Firebase.ready()) {
        // Leer la fecha de vencimiento (dueDate)
        String dueDate;
        if (Firebase.getString(firebaseData, "proxTask/next/tasks/0/dueDate")) {
            dueDate = firebaseData.stringData();
            
            // Mostrar la fecha en la pantalla LCD
            display.setCursor(0, 0);
            display.println("Fecha: " + dueDate.substring(0, 10));  // Mostrar solo la parte de la fecha
        }
        
        int i = 0;
        int y = 10;  // Coordenada Y para mostrar las tareas en diferentes líneas, debajo de la fecha
        while (true) {
            String taskPath = "proxTask/next/tasks/" + String(i) + "/task";
            String task;
            if (Firebase.getString(firebaseData, taskPath)) {
                task = firebaseData.stringData();
                // Mostrar la tarea en la pantalla LCD
                display.setCursor(0, y);
                display.println(task);
                y += 10;  // Incrementar la coordenada Y para la siguiente tarea
                i++;
            } else {
                break; // Salir del bucle si no se puede leer la tarea
            }
        }
        
        // Enviar contenido a la pantalla
        display.display();
    } else {
        Serial.println("Firebase no está listo");
    }
}
