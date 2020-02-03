#include <EEPROM.h>
#include <Wire.h>
#include <math.h>
#include <MS5xxx.h>
#define MS5607_ADDRESS 0x77
MS5xxx sensor(&Wire);

// Set the reference pressure to the current pressure to get relative altitude changes
double pressure, temperature, p_ref, t_ref, altitude;
char receivedChar; 
bool newData = false;
int cast;
int highest = 0;
int writeAddress = 0;
const int newTestButton = 6;

void setup() {
  //Serial.begin(9600);
  //Serial.print("1");
  sensor.setI2Caddr(MS5607_ADDRESS);
  //Serial.print("1");
  if(sensor.connect()>0) {
    //Serial.print("1");
    //Serial.println("Error connecting...");
    delay(200);
    setup();
  }
  //Serial.print("1");
  sensor.ReadProm();
  sensor.Readout();
  p_ref =  sensor.GetPres();
  t_ref = sensor.GetTemp() / 100.0f;
}

double getAltitude( double pres, double pres_ref, double temp )
{
  double a1 = pow( ( pres_ref / pres ), ( 1 / 5.257 ) );
  double a = (a1 - 1) * ( temp + 273.15 ) / 0.0065f;
  return a;
}


void recvOneChar() {
 if (Serial.available() > 0) {
 receivedChar = Serial.read();
 newData = true;
 }
} 

void printSerial()
{
  sensor.ReadProm();
  sensor.Readout();
  temperature = sensor.GetTemp() / 100.0f;
  pressure = sensor.GetPres();
  altitude = getAltitude( pressure, p_ref, temperature );
  cast = (int)altitude;
  //Serial.print("\nAlt: ");
  //Serial.print( cast );
  if(cast>highest)
    EEPROM.write(writeAddress,cast);
}

void loop() {
    printSerial();
    delay(50);
    if(digitalRead(6)){
      writeAddress += 1;
    }
    
}
