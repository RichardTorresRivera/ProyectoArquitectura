#include "server.h"
#include "declarations.h"

WebServer server(80);

void playNotificationTone()
{
    tone(BUZZER, 2000, 500); // Reproduce un tono de 2000 Hz durante 500 ms
    delay(500);              // Espera 500 ms
    tone(BUZZER, 3000, 500); // Reproduce un tono de 3000 Hz durante 500 ms
    delay(500);              // Espera 500 ms
    tone(BUZZER, 4000, 500); // Reproduce un tono de 4000 Hz durante 500 ms
    delay(500);              // Espera 500 ms
    noTone(BUZZER);          // Apaga el buzzer
}

void handleRoot()
{
    server.send(200, "text/plain", "Hola mundo");
}

void handleNotFound()
{
    server.send(404, "text/plain", "Not found");
}

void handleMissedCall(byte &current)
{
    if (server.hasArg("plain"))
    {
        String plainData = server.arg("plain");

        // Dividir los datos en nombre y número
        int separatorIndex = plainData.indexOf('|');
        if (separatorIndex != -1)
        {
            String contact = plainData.substring(0, separatorIndex);
            String number = plainData.substring(separatorIndex + 1);

            // Cambiar la pantalla actual a la pantalla de llamadas perdidas
            current = SCREEN_CALL;

            // Mostrar el mensaje en la pantalla OLED
            drawCallMissed(contact, number);
            playNotificationTone();

            Serial.println("Contact: " + contact);
            Serial.println("Number: " + number);

            server.send(200, "text/plain", "success");
        }
        else
        {
            server.send(400, "text/plain", "Missing parameters");
        }
    }
    else
    {
        server.send(400, "text/plain", "Missing parameters");
    }
}

void handleBatteryLow(byte &current)
{
    if (server.hasArg("plain"))
    {
        String batteryLevel = server.arg("plain");

        // Cambiar la pantalla actual a la pantalla de advertencia de batería baja
        current = SCREEN_BATTERY;

        // Mostrar el mensaje en la pantalla OLED
        drawBatteryLow(batteryLevel);
        playNotificationTone();

        Serial.println("Battery Level: " + batteryLevel);

        server.send(200, "text/plain", "success");
    }
    else
    {
        server.send(400, "text/plain", "Missing parameters");
    }
}

void handleMsgWsp(byte &current)
{
    if (server.hasArg("plain"))
    {
        String plainData = server.arg("plain");

        // Dividir los datos en app, título y mensaje
        int firstSeparatorIndex = plainData.indexOf('|');
        int secondSeparatorIndex = plainData.indexOf('|', firstSeparatorIndex + 1);

        if (firstSeparatorIndex != -1 && secondSeparatorIndex != -1)
        {
            String app = plainData.substring(0, firstSeparatorIndex);
            String title = plainData.substring(firstSeparatorIndex + 1, secondSeparatorIndex);
            String msg = plainData.substring(secondSeparatorIndex + 1);

            if (app.equals("WhatsApp"))
            {
                // Cambiar la pantalla actual a la pantalla de notificaciones de WhatsApp
                current = SCREEN_WSP;
                // Mostrar el título y mensaje en la pantalla OLED
                drawNotifications(app, title, msg);
                playNotificationTone();
            }
            Serial.println("App: " + app);
            Serial.println("Título: " + title);
            Serial.println("Mensaje: " + msg);
            server.send(200, "text/plain", "success");
        }
        else
        {
            server.send(400, "text/plain", "Incorrect format");
        }
    }
    else
    {
        server.send(400, "text/plain", "Missing parameters");
    }
}

void initServer(byte &current)
{
    server.on("/", handleRoot);
    server.on("/missed_call", HTTP_POST, [&current]()
              { handleMissedCall(current); });
    server.on("/battery_low", HTTP_POST, [&current]()
              { handleBatteryLow(current); });
    server.on("/msg_wsp", HTTP_POST, [&current]()
              { handleMsgWsp(current); });
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}