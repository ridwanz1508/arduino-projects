/*
 Program : Project 11. Baca ID Sidik Jari Tampilan LCD
 Input   : Sensor Fingerprint
 Output  : LCD 2x16
 */

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

int getFingerprintIDez();

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup()  
{ 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Baca ID");
  lcd.setCursor(0,1);
  lcd.print("Sidik Jari");
  delay(2000);
  finger.begin(57600);
  lcd.clear();
  lcd.print("Test Koneksi");
  lcd.setCursor(0, 1);
  
  if (finger.verifyPassword()) {
    lcd.print("OK !");
  } else {
    lcd.print("ERROR !");
    while (1);
  }
  delay(2000);
}

void loop()                     // run over and over again
{
  lcd.setCursor(0,0);
  lcd.print(" Letakkan Jari  ");
  lcd.setCursor(0,1);
  lcd.print("     Anda!      ");
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)return p;
    
    lcd.clear();
    lcd.print("Lepaskan Jari   ");
    delay(1000);
    //==============convert image
    p = finger.image2Tz();
    //------------ OK converted!
    
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_NOTFOUND) {
      lcd.setCursor(0, 1);
      lcd.print("Jari tdk Cocok!");
      delay(2000);
      return p;
    } 
    
  // found a match!
  lcd.setCursor(0, 1);
  lcd.print("No ID:"); lcd.print(finger.fingerID);
  lcd.print(" Cocok!  ");
  delay(2000);
}

