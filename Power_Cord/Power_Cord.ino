#include "deviceClass.cpp"

char ssid[] = "AP_terminal";          //  your network SSID (name)
char pass[] = "password";   // your network password

 int progState=0; //0-łączenie z wi-fi, 1-połączono, zaczynamy wysyłanie, 3-czekamy na odbiór, 4-dodanie urządzenia


IPAddress ip(192, 168, 4, 22);
int port = 333;
unsigned int numOfDev=2;


int status = WL_IDLE_STATUS;
//char servername[]=ip;  // remote server we will connect to
char output;
WiFiClient client;
device devices[] = {device(0,0,0,0), device(0,0,0,0)}; 

void setup() {
  Serial.begin(9600);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);   
 
  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    // don't do anything else:
    while(true);
  }
  else {
    Serial.println("Connected to wifi");
    Serial.println("\nStarting connection...");
    // if you get a connection, report back via serial:
   
  }
  
  delay(400);
  
  while (client.available()) 
  {
        
  if(client.peek() != '\r')
  {
    output = client.read();
    Serial.print(output);
  }
  else
  {
  Serial.println("koniec transmisji");
  break;
  }
  }
}

void loop()
{
  if(progState==0)
  {
    status = WiFi.begin(ssid, pass);
    if ( status == WL_CONNECTED)
    {
    progState=1;
    }
    else
    {
    delay(2000);
    }
  }
  if(progState==1)
  {
       if (client.connect(ip, port)) {
         
         for(int i =0; i<numOfDev; i++)
         {
           String packed = devices[i].pack();
           client.println(packed);
           Serial.println(packed);
         }
        
    }
  }
  
}
