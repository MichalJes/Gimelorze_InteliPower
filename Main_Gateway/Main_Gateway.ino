#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "configuration.h"

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

struct ConfigType configuration = {0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  ConfigInit();
  ConfigRead();

  IPAddress local_IP(
    configuration.AP_ip[0],
    configuration.AP_ip[1],
    configuration.AP_ip[2],
    configuration.AP_ip[3]);
  IPAddress gateway(
    configuration.AP_gateway[0],
    configuration.AP_gateway[1],
    configuration.AP_gateway[2],
    configuration.AP_gateway[3]);
  IPAddress subnet(
    configuration.AP_subnet[0],
    configuration.AP_subnet[1],
    configuration.AP_subnet[2],
    configuration.AP_subnet[3]);

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(configuration.AP_ssid, configuration.AP_pass) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());


  //Serial.printf("Web server started, open %s in a web browser\n", WiFi.softAPIP().toString().c_str());

  // wifi connection
  IPAddress wifi_IP(
    configuration.WIFI_ip[0],
    configuration.WIFI_ip[1],
    configuration.WIFI_ip[2],
    configuration.WIFI_ip[3]);
  IPAddress wifi_gateway(
    configuration.WIFI_gateway[0],
    configuration.WIFI_gateway[1],
    configuration.WIFI_gateway[2],
    configuration.WIFI_gateway[3]);
  IPAddress wifi_subnet(
    configuration.WIFI_subnet[0],
    configuration.WIFI_subnet[1],
    configuration.WIFI_subnet[2],
    configuration.WIFI_subnet[3]);
  WiFi.config(wifi_IP, wifi_gateway, wifi_subnet);
  WiFi.begin(configuration.WIFI_ssid, configuration.WIFI_pass);
  uint32_t timeout = millis() + 10000;
  while (WiFi.status() != WL_CONNECTED && timeout > millis())
  {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("\nWiFi connected. IP: ");
    Serial.println(WiFi.localIP());
  }

  webServerInit();
  dataServerInit();
}

void loop() {
  webServerProcess();
  dataServerProcess();
}

void ConfigInit()
{
  EEPROM.begin(sizeof(struct ConfigType));
  delay(10);
}

void ConfigSave()
{
  EEPROM.put(CONFIG_ADDRESS, configuration);
  EEPROM.commit();
  EEPROM.end();
}

void ConfigRead()
{
  EEPROM.get(CONFIG_ADDRESS, configuration);

  if (configuration.control != 43)
  {
    ConfigDefault();
    ConfigSave();
  }
}

void ConfigDefault()
{
  struct ConfigType default_conf = {
    {"AP_terminal"},
    {"password"},
    {192, 168, 4, 22},
    {192, 168, 4, 9},
    {255, 255, 255, 0},
    {"homewifi"},
    {"password"},
    {192, 168, 0, 20},
    {192, 168, 0, 1},
    {255, 255, 255, 0},
    43
  };
  configuration = default_conf;

  Serial.println("Default configuration restored");
}
