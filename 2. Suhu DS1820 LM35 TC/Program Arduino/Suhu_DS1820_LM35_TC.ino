/*
 Program : Project Sensor Suhu DS1820 LM35 TC
 Input   : Sensor LM35 di pin A0
           Sensor DS1820
           Sensor Thermocouple (MAX6675)
                  SO  --> PIN 8
                  SCK --> PIN 10
                  CS --> PIN 9
 Output  : Serial Monitor          
 */

#include <OneWire.h>
#include <Wire.h>
#include "max6675.h"

OneWire  ds(11);  // on pin 11 --DS1820
int thermoDO = 8;
int thermoCS = 9;
int thermoCLK = 10;
unsigned int adc,tempDS,tempLM,tempTC;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

//============================================
void setup(void) {
  Serial.begin(9600);  
  Serial.println("Monitoring 3 Sensor Suhu : DS1820, Thermocouple, LM35");
  
}
//===========================================
void loop(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
 //===================================

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  // the first ROM byte indicates which chip
  switch (addr[0]) {
  case 0x10: //  Chip = DS18S20 or old DS1820
    type_s = 1;
    break;
  case 0x28: //  Chip = DS18B20
    type_s = 0;
    break;
  case 0x22: //  Chip = DS1822
    type_s = 0;
    break;
  default:  //Device is not a DS18x20 family device.
    return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // need 9 bytes
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  tempDS=celsius;
  //====================
  adc = analogRead(0);   
  tempLM=(adc*5)/10;
 //==================== 
  celsius=thermocouple.readCelsius();
  tempTC=celsius; 

  Serial.print("Temp DS1820=");
  Serial.print(tempDS);
  Serial.println(" Celcius");
  Serial.print("Temp Thermocouple=");
  Serial.print(tempTC);
  Serial.println(" Celcius");
  Serial.print("Temp LM35=");
  Serial.print(tempLM);
  Serial.println(" Celcius");
  delay(1000);
}
