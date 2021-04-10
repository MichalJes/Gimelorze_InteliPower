ESP8266WebServer webserver(80);

void webServerInit()
{
  webserver.on("/", webServerHandleRoot);
  webserver.on("/config.htm", webServerHandleConfig);
  webserver.on("/post", webServerHandleConfigPost);
  webserver.on("/postcontrol", webServerHandleControlPost);
  webserver.on("/status.htm", webServerHandleStatus);
  webserver.begin();
}

void webServerHandleRoot()
{
  webserver.send(200, "text/html", prepareHtmlRootPage());
}

void webServerHandleConfig()
{
  webserver.send(200, "text/html", prepareHtmlConfigPage());
}

void webServerProcess()
{
  webserver.handleClient();
}

void webServerHandleStatus()
{
  webserver.send(200, "text/html", prepareHtmlStatusPage());
}

String prepareHtmlConfigPage()
{
  String htmlPage;
  htmlPage.reserve(2048);               // prevent ram fragmentation
  htmlPage = F("<!DOCTYPE HTML>"
               "<html>");
  // configuration form
  htmlPage += F("<form action=\"/post\" method=\"post\">"
                "<pattern for=\"ip\">"
  );
  // ap ssid
  htmlPage += F("AP_ssid"
                "<input type=\"text\" size=31 minlength=1 maxlength=31 name=\"APssid\" value=\"");
  htmlPage += configuration.AP_ssid;
  htmlPage += F("\"><br>");
  // ap password
  htmlPage += F("AP_pass"
                "<input type=\"text\" size=31 minlength=8 maxlength=63 name=\"APpass\" value=\"");
  htmlPage += configuration.AP_pass;
  htmlPage += F("\"><br>");
  // ap ip
  htmlPage += F("AP_ip"
                "<input type=\"text\" size=15 pattern=\"\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b\" name=\"APip\" value=\"");
  htmlPage += configIpToString(configuration.AP_ip);
  htmlPage += F("\"><br>");
  // ap gateway
  htmlPage += F("AP_gateway"
                "<input type=\"text\" size=15 pattern=\"\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b\" name=\"APgateway\" value=\"");
  htmlPage += configIpToString(configuration.AP_gateway);
  htmlPage += F("\"><br>");
  // ap subnet
  htmlPage += F("AP_subnet"
                "<input type=\"text\" size=15 pattern=\"\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b\" name=\"APsubnet\" value=\"");
  htmlPage += configIpToString(configuration.AP_subnet);
  htmlPage += F("\"><br>");
    // WIFI
  // wifi ssid
  htmlPage += F("WIFI_ssid"
                "<input type=\"text\" size=31 minlength=1 maxlength=31 name=\"WIFIssid\" value=\"");
  htmlPage += configuration.WIFI_ssid;
  htmlPage += F("\"><br>");
  // WIFI password
  htmlPage += F("WIFI_pass"
                "<input type=\"text\" size=31 minlength=8 maxlength=63 name=\"WIFIpass\" value=\"");
  htmlPage += configuration.WIFI_pass;
  htmlPage += F("\"><br>");
  // WIFI ip
  htmlPage += F("WIFI_ip"
                "<input type=\"text\" size=15 pattern=\"\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b\" name=\"WIFIip\" value=\"");
  htmlPage += configIpToString(configuration.WIFI_ip);
  htmlPage += F("\"><br>");
  // WIFI gateway
  htmlPage += F("WIFI_gateway"
                "<input type=\"text\" size=15 pattern=\"\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b\" name=\"WIFIgateway\" value=\"");
  htmlPage += configIpToString(configuration.WIFI_gateway);
  htmlPage += F("\"><br>");
  // WIFI subnet
  htmlPage += F("WIFI_subnet"
                "<input type=\"text\" size=15 pattern=\"\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b\" name=\"WIFIsubnet\" value=\"");
  htmlPage += configIpToString(configuration.WIFI_subnet);
  htmlPage += F("\"><br>");
  // submit
  htmlPage += F("<input type=\"submit\" value=\"Send\">"
                "</form>");
  // main page
  htmlPage += F("<br><br><a href=\"/\">Main page</a>");
  htmlPage += F("</html>"
                "\r\n");
  return htmlPage;
}

String prepareHtmlRootPage()
{
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("<!DOCTYPE HTML>"
               "<html>");
  htmlPage += F("Terminal<br>");
  htmlPage += F("<a href=\"config.htm\">Config Terminal</a><br>"
                "<a href=\"status.htm\">System Status</a><br><br><br>"
                "AP_ssid = ");
  htmlPage += configuration.AP_ssid;
  htmlPage += F("<br>");
  htmlPage += WiFi.softAPIP().toString();
  htmlPage += F("<br><br>");

  if (WiFi.status() == WL_CONNECTED)
  {
    htmlPage += F("WIFI_ssid = ");
    htmlPage += WiFi.SSID();//configuration.WIFI_ssid;
    htmlPage += F("<br>");
    htmlPage += WiFi.localIP().toString();
    htmlPage += F("<br>");
  }
  else
  {
    htmlPage += F("WIFI not connected<br>");
  }
  
  htmlPage += F("</html>"
                "\r\n");
  return htmlPage;
}

String prepareHtmlAcceptPage()
{
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("<!DOCTYPE HTML>"
               "<html>");
  htmlPage += F("Device configured<br>"
                "Auto restart in 5 sec...<br>");
  htmlPage += F("<a href=\"/\">Main page</a>");
  
  htmlPage += F("</html>"
                "\r\n");
  return htmlPage;  
}

void webServerHandleConfigPost()
{
  if (webserver.method() == HTTP_POST)
  {
    String tmp = "";
    if (webserver.hasArg("APssid"))
    {
      tmp = webserver.arg("APssid");
      memset(configuration.AP_ssid, 0, sizeof(configuration.AP_ssid));
      tmp.toCharArray(configuration.AP_ssid, tmp.length()+1);
    }
    if (webserver.hasArg("APpass"))
    {
      tmp = webserver.arg("APpass");
      memset(configuration.AP_pass, 0, sizeof(configuration.AP_pass));
      tmp.toCharArray(configuration.AP_pass, tmp.length()+1);
    }
    if (webserver.hasArg("APip"))
    {
      tmp = webserver.arg("APip");
      //memset(configuration.AP_ip, 0, sizeof(configuration.AP_ip));
      //tmp.toCharArray(configuration.AP_ssid, tmp.length()+1);
    }
    if (webserver.hasArg("APgateway"))
    {
      tmp = webserver.arg("APgateway");
      //memset(configuration.AP_gateway, 0, sizeof(configuration.AP_gateway));
      //tmp.toCharArray(configuration.AP_ssid, tmp.length()+1);
    }
    if (webserver.hasArg("APsubnet"))
    {
      tmp = webserver.arg("APsubnet");
      //memset(configuration.AP_subnet, 0, sizeof(configuration.AP_subnet));
      //tmp.toCharArray(configuration.AP_ssid, tmp.length()+1);
    }
    if (webserver.hasArg("WIFIssid"))
    {
      tmp = webserver.arg("WIFIssid");
      memset(configuration.WIFI_ssid, 0, sizeof(configuration.WIFI_ssid));
      tmp.toCharArray(configuration.WIFI_ssid, tmp.length()+1);
    }
    if (webserver.hasArg("WIFIpass"))
    {
      tmp = webserver.arg("WIFIpass");
      memset(configuration.WIFI_pass, 0, sizeof(configuration.WIFI_pass));
      tmp.toCharArray(configuration.WIFI_pass, tmp.length()+1);
    }
    if (webserver.hasArg("WIFIip"))
    {
      tmp = webserver.arg("WIFIip");
      stringIpToArray(tmp, configuration.WIFI_ip);
    }
    if (webserver.hasArg("WIFIgateway"))
    {
      tmp = webserver.arg("WIFIgateway");
      stringIpToArray(tmp, configuration.WIFI_gateway);
    }
    if (webserver.hasArg("WIFIsubnet"))
    {
      tmp = webserver.arg("WIFIsubnet");
      stringIpToArray(tmp, configuration.WIFI_subnet);
    }

    configuration.control = 43;
    ConfigSave();
  }

  webserver.send(200, "text/html", prepareHtmlAcceptPage());
}

void webServerHandleControlPost()
{
  String tmp;
  if (webserver.method() == HTTP_POST)
  {
    if (webserver.hasArg("control"))
    {
      tmp = webserver.arg("control");
      if(tmp == "0")
      {
        // all off
        Serial.println("Turn off all");
        nodeSwitchAll(0);
      }
      else
      {
        // all on
        Serial.println("Turn on all");
        nodeSwitchAll(1);
      }
    }
  }
  webserver.send(200, "text/html", prepareHtmlControlAcceptPage());
}

String prepareHtmlControlAcceptPage()
{
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("<!DOCTYPE HTML>"
               "<html>");
  htmlPage += F("Control Sent<br>");
  htmlPage += F("<a href=\"/status.htm\">Status page</a>");
  
  htmlPage += F("</html>"
                "\r\n");
  return htmlPage;  
}

static String configIpToString(byte ipbytes[])
{
  String result = "";
  for(int i=0; i<3; i++)
  {
    result += String(ipbytes[i]);
    result += ".";
  }
  result += String(ipbytes[3]);
  return result;
}

static void stringIpToArray(String str_ip, byte ipbytes[])
{
  char str[30]={0};
  str_ip.replace('.', ' ');
  str_ip.toCharArray(str, str_ip.length()+1);
  unsigned int t[4];
  sscanf(str, "%d %d %d %d", &t[0], &t[1], &t[2], &t[3]);
  for(int i=0; i<4; i++) ipbytes[i] = byte(t[i]);
}

String prepareHtmlStatusPage()
{
  String htmlPage;
  htmlPage.reserve(2048);               // prevent ram fragmentation
  htmlPage = F("<!DOCTYPE HTML>"
               "<html>");
  htmlPage += F("SYSTEM STATUS<br><br>");
  htmlPage += F("<a href=\"/\">Main page</a><br><br>");

  htmlPage += F("<form action=\"/postcontrol\" method=\"post\">"
                "TURN_ON<input type=\"submit\" name=\"control\" value=\"1\"><br>"
                "TURNOFF<input type=\"submit\" name=\"control\" value=\"0\">"
                "</form>");
  
  htmlPage += F("<br><br>code name state[0] state[1] current[0] current[1] control[0] control[1]<br>");
  if (nodes.size() > 0)
  {
    for(int i = 0; i < nodes.size(); i++)
    {
      htmlPage += String(nodes[i].dev_code) + " ";
      htmlPage += nodes[i].dev_name;
      htmlPage += F(" ");
      htmlPage += String(nodes[i].state[0]) + " ";
      htmlPage += String(nodes[i].state[1]) + " ";
      htmlPage += String(nodes[i].current[0]) + " ";
      htmlPage += String(nodes[i].current[1]) + " ";
      htmlPage += String(nodes[i].control[0]) + " ";
      htmlPage += String(nodes[i].control[1]) + " ";
      htmlPage += F("<br>");
    }    
  }
  
  htmlPage += F("</html>"
                "\r\n");
  return htmlPage;   
}
