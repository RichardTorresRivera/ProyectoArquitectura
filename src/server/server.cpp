#include "server.h"

WebServer server(80);

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
            current = 3;

            // Mostrar el mensaje en la pantalla OLED
            drawCallMissed(contact.c_str(), number.c_str());

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

void initServer(byte &current)
{
    server.on("/", handleRoot);
    server.on("/missed_call", HTTP_POST, [&current]()
              { handleMissedCall(current); });
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}