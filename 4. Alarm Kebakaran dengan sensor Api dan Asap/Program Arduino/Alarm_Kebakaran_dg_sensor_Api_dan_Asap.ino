/*
Program : Alarm Kebakaran dengan sensor Api dan Asap
Input   : Sensor MQ-2, Sensor Flame
Output  : LCD 2x16, 2 Relay
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

float MQ2;
float sensorApi;
int adc;
byte kedip;
byte keluar;

void setup(){ 
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);  
  pinMode(9,OUTPUT);
  digitalWrite(9,LOW); 
  lcd.begin(16, 2);
  lcd.print("Alarm kebakaran ");
  delay(2000);
  lcd.clear();
  lcd.print(" Tunggu sensor");
  lcd.setCursor(0, 1);
  lcd.print("    siap...");
  delay(2000);
  do{
    adc=(analogRead(A0));
    MQ2 = adc * (5.0 / 1023.0);  
  }
  while(MQ2 > 1.5);
  lcd.clear();
}

void loop(){
  adc=(analogRead(A0));
  MQ2 = adc * (5.0 / 1023.0);
  adc=(analogRead(A1));
  sensorApi = adc * (5.0 / 1023.0);
  lcd.setCursor(0,0);
  lcd.print("Api :");  
  lcd.print(sensorApi);  
  lcd.print(" ");  
  lcd.setCursor(0,1);
  lcd.print("Asap:");  
  lcd.print(MQ2);  
  lcd.print("    ");
  if (MQ2 > 1.2 || sensorApi<3){
    lcd.setCursor(0,0);
    lcd.print(" Ada kebakaran");
    lcd.setCursor(0,1);
    lcd.print("AlarmON ValveON");
    digitalWrite(8,HIGH); 
    digitalWrite(9,HIGH); 
    delay(1000);
    keluar=1;
    do{
      adc=(analogRead(A0));
      MQ2 = adc * (5.0 / 1023.0);  
      adc=(analogRead(A1));
      sensorApi = adc * (5.0 / 1023.0);    
      if(sensorApi>4 && MQ2<1)keluar=0;
    }
    while(keluar);
    digitalWrite(8,LOW); 
    digitalWrite(9,LOW); 
    lcd.clear();
  }
  delay(100);
}
