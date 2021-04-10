#include <ESP8266WiFi.h>

const char *ssid = "AP_terminal";
const char *password = "password";
IPAddress ip(192, 168, 4, 22);
int port = 333;
WiFiClient client;
int i =0;

void setup()
{
    Serial.begin(115200);
    Serial.println();

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        ++i;
    }
    Serial.print(" Connected after ");
    Serial.print(i/2);
    Serial.println(" seconds");
}

void loop()
{
    Serial.println(WiFi.status());
    Serial.printf("\n[Connecting to %s ... ", ssid);
    if (client.connect(ip, port))
    {
        Serial.println("connected]");

        Serial.println("[Sending a request]");
        client.print(12);
        client.print(" a asasdasdda aaaa");

        Serial.println("[Response:]");
        while (client.connected() || client.available())
        {
            if (client.available())
            {
                String line = client.readStringUntil('\n');
                Serial.println(line);
            }
        }
        Serial.println("\n[Disconnected]");
    }
    else
    {
        Serial.println("connection failed!]");
    }
    delay(5000);
}
