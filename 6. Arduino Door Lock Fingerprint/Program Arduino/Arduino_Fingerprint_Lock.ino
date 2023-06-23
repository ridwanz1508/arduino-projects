/*************************************
 * Program : Project 9. Arduino Door Lock Fingerprint
 * Input   : Sensor Fingerprint
 * Output  : LCD 2x16, Relay
 * 125 Proyek Arduino Inkubatek
 * www.tokotronik.com
 * ***********************************/
 
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,10,11,12,13);
int getFingerprintIDez();
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
char kode,i;

void setup()  
{
   
   pinMode(5,OUTPUT);   
   kode=6;
   lcd.begin(16, 2);
   lcd.print("Ardu Fingerprint");
   lcd.setCursor(0,1);
   lcd.print("    Door Lock   ");  
   finger.begin(57600);
 
}

void loop()                     
{
  lcd.clear();
  lcd.print("Tempelkan Jari");
  lcd.setCursor(0,1);
  lcd.print("di Fingerprint");
  i=1;
  while(i){
  getFingerprintIDez();
  delay(50);            
  }
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_IMAGEFAIL:
      return p;
    default:
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      return p;
    default:
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    return p;
  } else {
    return p;
  }   
  
  }

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  i=0;
  if(finger.fingerID==kode){     
      lcd.clear();
      lcd.print("Sidik Jari Benar");
      lcd.setCursor(0,1);      
      lcd.print("Silakan Masuk..");
      digitalWrite(5,HIGH);      
      delay(10000);              
      }
   else{
      lcd.clear();
      lcd.print("Sidik Jari Salah");
      
   }
  return finger.fingerID;   
}

void motor_buka(){
  digitalWrite(7,1);
  digitalWrite(6,1);
  digitalWrite(5,0);
  while(digitalRead(A0));
  digitalWrite(7,0);
  delay(1000);
}

void motor_tutup(){
  digitalWrite(7,1);
  digitalWrite(6,0);
  digitalWrite(5,1);
  while(digitalRead(A1));
  digitalWrite(7,0);
  delay(1000);
}
