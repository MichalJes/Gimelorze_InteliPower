/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read new NUID from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];
String readName;
float current1_read;
float current2_read;
float current1;
float current2;
float foto1;
float foto2;
int lightsens = 300;
String plug1;
String plug2;
float setCurrent1 = 0.4;
float setCurrent2 = 0.4;




void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  //pinMode(3, OUTPUT);
  pinMode(A0, INPUT); //foto 1
  pinMode(A1, INPUT); //foto 2
  pinMode(A2, INPUT); //current 1
  pinMode(A3, INPUT); //current 2
  pinMode(7, OUTPUT); //relay1
  pinMode(8, OUTPUT); //relay2
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), reset1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), reset2, CHANGE);
}
 
void loop() {
Serial.print("current1 = ");   
Serial.println(current1);
Serial.print("current2 = ");
Serial.println(current2);
Serial.print("foto1 = ");
Serial.println(analogRead(A0));
Serial.print("foto2 = ");
Serial.println(analogRead(A1));
Serial.print("plug1 = ");
Serial.println(plug1);
Serial.print("plug2 = ");
Serial.println(plug2);
  
// reading current
  if(analogRead(A0) < lightsens || analogRead(A1) < lightsens)   //reading only when plugged
  {
    current1_read = analogRead(A2);
    current2_read = analogRead(A3);
    current1= toCurrent(current1_read);
    current2= toCurrent(current2_read);

   if (current1> setCurrent1)
      digitalWrite(7, HIGH);
   if (current2> setCurrent2)
      digitalWrite(8, HIGH);
    
  }
//
  if(analogRead(A0)> lightsens)
    { plug1 = "";
      digitalWrite(7, HIGH); 
    }
  if(analogRead(A1)> lightsens)
    { plug2 = "";
      digitalWrite(8, HIGH);
    }



// reading RFID should be on the end of the loop
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  //if (rfid.uid.uidByte[0] != nuidPICC[0] || 
  //  rfid.uid.uidByte[1] != nuidPICC[1] || 
  //  rfid.uid.uidByte[2] != nuidPICC[2] || 
  //  rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
 // }
 // else 
 // {//Serial.println(F("Card read previously."));

readName = "";
readName = (String)rfid.uid.uidByte[0] + (String)rfid.uid.uidByte[1] + (String)rfid.uid.uidByte[2] + (String)rfid.uid.uidByte[3];
Serial.println(readName);
Serial.println(rfid.uid.uidByte[0]);
Serial.println(rfid.uid.uidByte[1]);
Serial.println(rfid.uid.uidByte[2]);
Serial.println(rfid.uid.uidByte[3]);
//Przypisanie urzadzenia do gniazdka//
for (;;)
{
  if (analogRead(A0)< lightsens && plug1 == "")
    {
      plug1 = readName;
      digitalWrite(7, LOW);
      break;
    }
  else if (analogRead(A1)< lightsens && plug2 == "")
      {
        plug2 = readName; 
        digitalWrite(8, LOW);
        break;
      }
}



//
/*if(readName=="2215633158")
{
digitalWrite(3, !digitalRead(3));
Serial.println("HALO");
}*/
  //}
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();



}



/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

float toCurrent(float readv)
{
  float result;
  readv = (readv/1024)*5;
  result=(readv- 2.26)/0.0536;
  return result;
}

void reset1() {
  digitalWrite(7, !digitalRead(7));
  Serial.println("RESET1/////////////////////////////////////");
  
}
void reset2() {
  digitalWrite(8, !digitalRead(8));
  Serial.println("RESET2////////////////////////////////////////");
  
}
