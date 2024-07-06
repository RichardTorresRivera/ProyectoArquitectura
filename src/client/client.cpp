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

void toneMaxMovil()
{
    Serial.println("tono del movil al maximo");
    sendIFTTTRequest("cell_tone");
}