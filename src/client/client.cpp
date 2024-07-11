#include "client.h"

int dayOfWeekToNumber(const String &day)
{
    if (day == "domingo")
        return 0;
    if (day == "lunes")
        return 1;
    if (day == "martes")
        return 2;
    if (day == "miercoles")
        return 3;
    if (day == "jueves")
        return 4;
    if (day == "viernes")
        return 5;
    if (day == "sabado")
        return 6;
    return -1; // Valor inválido si no coincide con ningún día de la semana
}

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
        if (Firebase.getString(firebaseData, "next_alarm/hour"))
        {
            alarmTime.hour = firebaseData.stringData();
        }
        if (Firebase.getString(firebaseData, "next_alarm/minute"))
        {
            alarmTime.minute = firebaseData.stringData();
        }
        if (Firebase.getString(firebaseData, "next_alarm/day"))
        {
            alarmTime.day = firebaseData.stringData();
        }
    }
    return alarmTime;
}

bool isAlarmTime(NTPClient ntpClient, AlarmTime alarmTime, bool &alarmActivated)
{
    if (alarmTime.hour.isEmpty() || alarmTime.minute.isEmpty() || alarmTime.day.isEmpty())
    {
        return false;
    }

    static String previousTime = "";
    ntpClient.update();

    // Obtener la hora actual en formato "HH:MM:SS"
    String currentTime = ntpClient.getFormattedTime();

    // Obtener la fecha actual
    unsigned long epochTime = ntpClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);
    int currentHour = ptm->tm_hour;
    int currentMinute = ptm->tm_min;
    int currentWeekday = ptm->tm_wday;

    // Convertir el día de la alarma a número (0-6)
    int alarmWeekday = dayOfWeekToNumber(alarmTime.day);
    if (alarmWeekday == -1)
    {
        return false; // Valor inválido de día de la semana
    }

    // Convertir la hora y minuto de la alarma a enteros
    int alarmHour = alarmTime.hour.toInt();
    int alarmMinute = alarmTime.minute.toInt();

    // Comparar tiempos y día de la semana
    bool isAlarmNow = (currentHour == alarmHour && currentMinute == alarmMinute && currentWeekday == alarmWeekday);
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
