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
        String response = nodeHandleFromString(line);
        client.println(response);
        break;
      }
    }

    client.stop();
    Serial.println("[Client disconnected]");
  }
}

void dataServerInit()
{
  dataserver.begin();
}
