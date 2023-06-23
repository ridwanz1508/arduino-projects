
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
   pinMode(A0,INPUT);
   pinMode(A1,INPUT);
   pinMode(A2,INPUT);
   digitalWrite(A0,HIGH);  
   digitalWrite(A1,HIGH);
   digitalWrite(A2,HIGH);
   pinMode(5,OUTPUT);
   pinMode(6,OUTPUT);
   pinMode(7,OUTPUT);
   kode=6;
   lcd.begin(16, 2);
   lcd.print(" Keamanan Pintu ");
   lcd.setCursor(0,1);
   lcd.print(" dg Fingerprint ");  
   finger.begin(57600);
 
}

void loop()                     
{
  lcd.clear();
  lcd.print("Pasang Jari");
  lcd.setCursor(0,1);
  lcd.print("di Sensor FP");
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
      //Serial.print("Sidik Jari benar");
      lcd.clear();
      lcd.print("Sidik Jari Benar");
      lcd.setCursor(0,1);
      lcd.print("Tekan Tombol !");
      while(digitalRead(A2));
      lcd.clear();
      lcd.print("Pintu Terbuka");
      lcd.setCursor(0,1);
      lcd.print("Silakan Masuk !");
      motor_buka();
      delay(10000);
      lcd.clear();
      lcd.print("Tutup Pintu");
      motor_tutup();      
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
