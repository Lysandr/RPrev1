/*
   code by ******* ********* May 15, 2014 KC9UUS
   
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   9600-baud serial GPS device hooked up on pins 0(rx) and 1(tx).
   This code should work with a Ublox Neo-6m GPS (super cheap)
   
   The problem is that the servo movement updates every interval (1.005 seconds or whatever it may be)
   
   
*/

#include <TinyGPS++.h>        
#include <SoftwareSerial.h>   
#include <stdlib.h>
#include <SD.h>

static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus Ublox;                 // The TinyGPS++ object
static char dtostrfbuffer[20];    //this is some sort of char string conversion method. uses the stdlib library
int CS = 10;
SoftwareSerial ss(RXPin, TXPin);

String SD_lat = "invalid";
String SD_lon = "invalid";
String SD_alt = "invalid";
String SD_mph=  "invalid";
String dataString ="";


void setup()
{
  Serial.begin(115200);  //Change if new GPS unit uses diff rate
  ss.begin(GPSBaud);
  pinMode(CS, OUTPUT);  //Chip Select Pin for the SD Card

if(!SD.begin(CS))
  {
    Serial.println("Card Failure");
    return;
  }
}



void loop()
{
  
  printFloat(Ublox.location.lat(), Ublox.location.isValid(), 11, 6, 1); //last value is for SD logging within the string (for lat,lng, alt, speed)
  printFloat(Ublox.location.lng(), Ublox.location.isValid(), 12, 6, 2); //2 SD
  printFloat(Ublox.altitude.meters(), Ublox.altitude.isValid(), 7, 2, 3); //3SD
  printFloat(Ublox.speed.kmph(), Ublox.speed.isValid(), 6, 2, 4); //4 SD

  dataString = SD_lat + "," + SD_lon + "," + SD_alt + "," + SD_mph;
  File dataFile = SD.open("FlightLog.csv", FILE_WRITE);
  
  if (dataFile)
  {
    dataFile.println(dataString);
                                  //This is where code P2 needs to go.
    Serial.println(dataString);
    dataFile.close();
  }
  else
  {Serial.println("\nCouldn't open the data logging file!");}
  Serial.println();
  smartDelay(500);
  
  
  if (millis() > 5000 && Ublox.charsProcessed() < 10)
    Serial.println(F("Your GPS is fucked up, make sure you hooked shit up correctly"));
}



static void printFloat(float val, bool valid, int len, int prec, int SD_val)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    switch(SD_val)
    {
    case 1:
     SD_lat = dtostrf(val,10,5,dtostrfbuffer);
     break;
    case 2:
      SD_lon = dtostrf(val,10,5,dtostrfbuffer);
      break;
    case 3:
      SD_alt = dtostrf(val,10,5,dtostrfbuffer);
      break;
    case 4:
      SD_mph = dtostrf(val,10,5,dtostrfbuffer);
      break;
    }
    
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}


static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do {
    while (ss.available())
      Ublox.encode(ss.read());
  } 
  while (millis() - start < ms);
}

//   ___  __    ________ ________  ___  ___  ___  ___  ________      
//  |\  \|\  \ |\   ____\ \  ___  \|\  \|\  \|\  \|\  \|\   ____\     
//  \ \  \/  /|\ \  \___\  \____   \ \  \\\  \ \  \\\  \ \  \___|_    
//   \ \   ___  \ \  \    \|____|\  \ \  \\\  \ \  \\\  \ \_____  \   
//    \ \  \\ \  \ \  \____   __\_\  \ \  \\\  \ \  \\\  \|____|\  \  
//     \ \__\\ \__\ \_______ \\_______\ \_______\ \_______\____\_\  \ 
//      \|__| \|__|\|_______ \|_______|\|_______|\|_______|\_________\
//                                                        \|_________|

