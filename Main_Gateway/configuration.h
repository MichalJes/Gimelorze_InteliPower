#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define CONFIG_ADDRESS  0x0

struct ConfigType {
  char AP_ssid[32];
  char AP_pass[64];
  byte AP_ip[4];
  byte AP_gateway[4];
  byte AP_subnet[4];
  
  char WIFI_ssid[32];
  char WIFI_pass[64];
  byte WIFI_ip[4];
  byte WIFI_gateway[4];
  byte WIFI_subnet[4];

  byte control;
};

#endif
