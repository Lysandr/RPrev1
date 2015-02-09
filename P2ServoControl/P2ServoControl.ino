#include <Servo.h>
#include <Wire.h> //I2C Arduino Library
#include <stdlib.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#define address 0x1E //0011110b, I2C 7bit address of HMC5883

Servo xaxis;//control fins parallel with lat lines (Change in lat. lines precips change in angle)
Servo yaxis;//control fins parallel with long lines (Change in long. lines precips change in angle)

int x , y , z; //triple axis data for HMC
int readA; //
int readB; //
int xpin = 11; //pwm pin for 'x' axis
int ypin = 12; //pwm pin for 'y' axis
unsigned long Newlat = 0; //create a long int for the lat * 100,000
unsigned long Newlon = 0; //create a long int for the long * 100,000

void setup(){
  xaxis.attach(xpin); //set the servos up antiparallel to eachother 
  yaxis.attach(ypin);//if this attachment doesnt work for two servos simultaneously
    
  Wire.begin();
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission(); //ends the setup transmission
  
}

void loop(){
  
  Wire.beginTransmission(address); //open up communication again
  Wire.write(0x03); //select register 3, X Most Significant Bit register
  Wire.endTransmission();
  
  abs(SD_lat * 100000)= Newlat; // to make the latitude an integer, sign is trivial?
  abs(SD_lon * 100000)= Newlon; // repeat wiht longitude
  
  readA= map(&Newlat, lcoord +5, lcoord-5-((lcoord-tcoord)*2), 60, 120); // if tcoord<lcoord  (PROBABLY XAXIS)
  //newread= map($Newlat, lcoord -5, lcoord+5+((tcoord-lcoord)*2), 120, 60);  if tcoord>lcoord
  readB= map(&Newlon, lcoord +5, lcoord-5-((lcoord-tcoord)*2), 60, 120); // if tcoord<lcoord  (PROBABLY YAXIS)
  //newread= map(&Newlon, lcoord -5, lcoord+5+((tcoord-lcoord)*2), 120, 60);  if tcoord>lcoord
  servorun(readA, readB);
  
  Wire.requestFrom(address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
     
}
void servorun(readA, readB){
  //do all if statements and x,y switching here
  readA = 
}




