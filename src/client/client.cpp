#include "client.h"

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

void toneMovil(short n)
{
    char msg[10];
    sprintf(msg, "cell_tone%d", n);
    sendIFTTTRequest(msg);
}

AlarmTime getAlarm()
{
    AlarmTime alarmTime;
    // Leer los datos de Firebase
    if (Firebase.ready())
    {
        if (Firebase.getString(firebaseData, "proxAlarm/nextAlarm/hour"))
        {
            alarmTime.hour = firebaseData.stringData();
        }
        if (Firebase.getString(firebaseData, "proxAlarm/nextAlarm/minute"))
        {
            alarmTime.minute = firebaseData.stringData();
        }
    }
    return alarmTime;
}

bool isAlarmTime(NTPClient ntpClient, AlarmTime alarmTime, bool &alarmActivated)
{
    if (alarmTime.hour.isEmpty() || alarmTime.minute.isEmpty())
    {
        return false;
    }
    static String previousTime = "";
    ntpClient.update();
    String currentTime = ntpClient.getFormattedTime();
    // Extraer horas y minutos de currentTime
    int currentHour = currentTime.substring(0, 2).toInt();
    int currentMinute = currentTime.substring(3, 5).toInt();

    // Extraer horas y minutos de alarmTime
    int alarmHour = alarmTime.hour.toInt();
    int alarmMinute = alarmTime.minute.toInt();

    // Comparar tiempos
    bool isAlarmNow = (currentHour == alarmHour && currentMinute == alarmMinute);
    if (isAlarmNow && !alarmActivated)
    {
        alarmActivated = true;      // Activar la alarma
        previousTime = currentTime; // Actualizar el tiempo anterior
        return true;                // Indicar que es hora de la alarma
    }
    else if (!isAlarmNow)
    {
        alarmActivated = false; // Reiniciar el estado de la alarma
    }
    return false; // No es hora de la alarma
}

Task getTask()
{
    Task taskInfo;
    // Leer los datos de Firebase
    if (Firebase.ready())
    {
        // Leer la fecha de vencimiento (dueDate)
        if (Firebase.getString(firebaseData, "proxTask/next/tasks/0/dueDate"))
        {
            taskInfo.dueDate = firebaseData.stringData().substring(0, 10); // Guardar solo la parte de la fecha
        }

        int i = 0;
        while (true)
        {
            String taskPath = "proxTask/next/tasks/" + String(i) + "/task";
            String task;
            if (Firebase.getString(firebaseData, taskPath))
            {
                task = firebaseData.stringData();
                taskInfo.tasks.push_back(task); // Agregar la tarea a la lista
                i++;
            }
            else
            {
                break; // Salir del bucle si no se puede leer la tarea
            }
        }
    }
    else
    {
        Serial.println("Firebase no está listo");
    }
    return taskInfo;
}
