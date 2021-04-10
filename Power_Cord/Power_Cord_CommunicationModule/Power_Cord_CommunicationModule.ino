#include "deviceClass.cpp"

char ssid[] = "AP_terminal";          //  your network SSID (name)
char pass[] = "password";   // your network password

 int progState=0; //0-łączenie z wi-fi, 1-połączono, zaczynamy wysyłanie, 2-czekamy na odbiór, 3-sterowanie, zmienianie stanu wg centrali

IPAddress ip(192, 168, 4, 22);
int port = 333;
unsigned int numOfDev=2;
bool firstSig=0;

int status = WL_IDLE_STATUS;
//char servername[]=ip;  // remote server we will connect to
char output;
String outputS1, outputS2;
WiFiClient client;
device devices[] = {device(1, "testid1" ,0,0), device(0,"testid2",0,0)}; 

void setup() {
  Serial.begin(9600);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);   

  pinMode(12, OUTPUT);
   pinMode(13, OUTPUT);
   digitalWrite(12, LOW);
   digitalWrite(13, LOW);
 
  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    
  }
  else {
    Serial.println("Connected to wifi");
    Serial.println("\nStarting connection...");
    // if you get a connection, report back via serial:
   
  }
  
  delay(400);
  
  
}

void loop()
{
  //Serial.println(progState);
  if(progState==0)//łączenie
  {  
    if ( status == WL_CONNECTED)
    {
    progState=1;
    Serial.print("connected");
    }
    else
    {
    Serial.println("reconnecting...");
    status = WiFi.begin(ssid, pass);
    delay(1000);
    }
  }
  if(progState==1)//comunication
  {
       if (client.connect(ip, port)) {
         
         for(int i =0; i<1; i++)
         {
           String packed = devices[i].pack();
           client.println(packed);
           firstSig=0;
          // Serial.println(packed);
            while(firstSig==0)
            {
                if(client.available()>0)
                {

                if(client.peek() != '\r')
                 {
                 output = client.read();
                 outputS1=outputS1+output;       
                 }
                 else
                 {
                  Serial.println(outputS1);
                  Serial.println(nodeHandleFromString1(outputS1));
                  Serial.println(nodeHandleFromString2(outputS1));
                  if(nodeHandleFromString1(outputS1)==true)
                          digitalWrite(12, HIGH);
                          else
                         digitalWrite(12, LOW);                
                  if(nodeHandleFromString2(outputS1)==true)
                          digitalWrite(13, HIGH);
                          else
                         digitalWrite(13, LOW);  
                  
                  Serial.println("-------");
                  outputS1="";
                  output="";
                  firstSig=1;   
                   delay(1000);        
                 }      
               }
             }
  
        }

  }
  
}
}





bool nodeHandleFromString1(String message)
{
  char *message_str = new char[message.length()+1];
  message.toCharArray(message_str, message.length()+1);
  // dev_code dev_special_name status
  int dev_code, state[2], current[2];
  char dev_name[30];
  sscanf(message_str, "%d %s %d %d %d %d", &dev_code, dev_name, &state[0], &state[1], &current[0], &current[1]);
  return state[0];
}
bool nodeHandleFromString2(String message)
{
  char *message_str = new char[message.length()+1];
  message.toCharArray(message_str, message.length()+1);
  // dev_code dev_special_name status
  int dev_code, state[2], current[2];
  char dev_name[30];
  sscanf(message_str, "%d %s %d %d %d %d", &dev_code, dev_name, &state[0], &state[1], &current[0], &current[1]);
  return state[1];
}




  
