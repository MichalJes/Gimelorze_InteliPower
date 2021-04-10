WiFiServer dataserver(333);

void dataServerProcess()
{
  WiFiClient client = dataserver.available();

  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        //if (line.length() == 1 && line[0] == '\n')
        //{
        client.println("OK\r\n");
        break;
        //}
      }
    }

    delay(1);
    client.stop();
    Serial.println("[Client disconnected]");
  }
}

void dataServerInit()
{
  dataserver.begin();
}
