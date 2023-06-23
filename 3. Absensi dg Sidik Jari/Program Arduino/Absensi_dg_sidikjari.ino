/*************************************
 * Program : Project 3 Absensi dg sidik jari
 * Input   : Sensor Fingerprint, DS1307 
 * Output  : LCD 2x16, BUZZER
 * ***********************************/
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Wire.h>
#define DS1307_ADDRESS 0x68
byte zero = 0x00;

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

byte second ,minute,hour, weekDay;
byte monthDay,month,year;

uint8_t id;
uint8_t getFingerprintEnroll();

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup(){
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  delay(500);    

  Wire.begin();
  /*
   // Seting RTC
    second=0;
    minute=50;
    hour=12;
    monthDay=15;
    month=10;
    year=16;
    setingRTC();   
    delay(100); 
  */ 
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Absensi karyawan");
  lcd.setCursor(0,1);
  lcd.print("  dg sidik jari");
  delay(2000);
  lcd.clear();
  lcd.print("Test Koneksi");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    lcd.setCursor(0,1);
    lcd.print("OK !");
  } else {
    lcd.setCursor(0,1);
    lcd.print("ERROR !");
    while (1);
  }
  delay(2000);
}


//===================program utama
void loop(){
//  bacaRTC();
  lcd.setCursor(0, 0);
  lcd.print("Absensi Karyawan");
  lcd.setCursor(0, 1);
  lcd.print("  Letakkan Jari ");
  getFingerprintIDez(); 
  kirimSerial();
  delay(50);
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();  
  if (p == FINGERPRINT_NOTFOUND){ 
      lcd.clear();
      lcd.print("     Akses");
      lcd.setCursor(0,1);
      lcd.print(" Tidak Diterima"); 
      digitalWrite(7, HIGH);
      delay(3000);
      digitalWrite(7, LOW);
      delay(100);
    return p;
  }

  if (p != FINGERPRINT_OK)  return -1;
  // found a match!
  digitalWrite(7, HIGH);
  delay(500);
  digitalWrite(7, LOW);
  delay(50);
  
  bacaRTC(); 
  lcd.clear();
  lcd.print("Karyawan ");
  lcd.print(finger.fingerID); 
  lcd.setCursor(0,1);
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);
  lcd.print("  ");
  
  lcd.print(monthDay);
  lcd.print("/");
  lcd.print(month);
  lcd.print("    ");
  
//-------------------simpan data
  id=finger.fingerID;
  byte addr = (monthDay*10)+id;
  EEPROM.write(addr, 0);
  delay(4000);
  lcd.clear();
  return finger.fingerID;
}
/****************************************
 * peta penyimpanan di EEPROM
 * tanggal disimpan pada alamt 0
 * ---------------------------------------------
 * tgl |          Alamat EEPROM karyawan
 *     |  1   2   3   4   5   6   7   8   9   10
 * ----------------------------------------------  
 *  1  |  11  12  13  14  15  16  17  18  19  20
 *  2  |  21  22  23  24  25  26  27  28  29  30
 *  3  |  31  32  33  34  35  36  37  38  39  40
 *  
 *  30 |  311 312 313 314 315 316 317 318 319 320
 *  ---------------------------------------------
 *  Rumus Penyimpanan EEPROM=
 *    alamatSimpan = (tgl * 10)+id
 *  
 *  kode absensi:
 *    * 0 = Hadir
 *    * 255 = Alpha
 */


//---------------------------RTC Area
byte decToBcd(byte val){
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
  return ( (val/16*10) + (val%16) );
}

void bacaRTC(){
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0b111111);
  weekDay = bcdToDec(Wire.read());
  monthDay = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
}

void setingRTC(){
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //stop RTC
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.write(zero); //start 
  Wire.endTransmission();
}

void kirimSerial(){
  if(Serial.available()){
    char str;
    str=Serial.read();
    if (str=='R'){
      /*
       * ---------------------------------------------
       * tgl |          Absensi karyawan
       *     |  1   2   3   4   5   6   7   8   9   10
       * ----------------------------------------------  
       *  1  |  H   H   -   -   H   H   H   H   H   H
       *  2  |  H   H   H   H   -   H   H   H   H   H
       *  3  |  
       *  
       *  30 |  
       *  ---------------------------------------------
       *  H = HADIR
       *  - = ALPHA
       */
       
      for(byte i=0; i<60;i++){
        Serial.print('-');
      }      
      Serial.println('-');
      
      Serial.println(" tgl |       Absensi Karyawan");
      Serial.print  ("     |  ");
      
      for(byte i=1; i<11;i++){
        Serial.print(i);
        Serial.print("    ");
      }
      Serial.println("   ");
      
      for(byte i=0; i<60;i++){
        Serial.print('-');
      }
      Serial.println('-');
      
      byte value;
      byte address;
      byte j=0;
      String absensi="";
      for(byte tgl=1; tgl<31; tgl++){
        Serial.print(tgl);
        Serial.print("    |");
        j=tgl*10;
        for(byte i=1; i<11; i++){
          address=i+j;
          value = EEPROM.read(address);
          if (value==0) absensi="H";
          else absensi="-";
          Serial.print(" "+absensi+"   ");
        }
        Serial.println("  ");
      } 
      str=' ';
    }
    else if(str=='D'){
      Serial.println("Hapus Data");
      for(word i=0; i<310; i++){
        EEPROM.write(i, 255);
        Serial.println(".");
        delay(5);
      }
      Serial.println("Hapus Data Sukses!");
    }
  }
}
  
