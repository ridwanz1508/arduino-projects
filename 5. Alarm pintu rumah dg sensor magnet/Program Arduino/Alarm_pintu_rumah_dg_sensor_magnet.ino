/*
Program : Alarm Pintu Rumah dg Sensor Magnet
Input   : Tombol Push On 
Output  : 2 Relay, LED
*/

int inSensor = 8;
int tbStSp = 9;
int alarm = 10;
int lamp = 11;
int indicator = 12;

boolean Start=false;

void setup(){
  pinMode(inSensor, INPUT);
  pinMode(tbStSp, INPUT);
  digitalWrite(inSensor, HIGH);
  digitalWrite(tbStSp, HIGH);
  pinMode(alarm, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(indicator, OUTPUT);
  digitalWrite(alarm, LOW);
  digitalWrite(lamp, LOW);
  digitalWrite(indicator, LOW);
}

void loop(){
  if (digitalRead(tbStSp)==0){
    Start=true;
    digitalWrite(indicator, HIGH);
    delay(500);
  }
  if (digitalRead(inSensor)==1 && Start==true){
      digitalWrite(alarm, HIGH);
      digitalWrite(lamp, HIGH);
      delay(2000);
      while(digitalRead(tbStSp));
      digitalWrite(alarm, LOW);
      digitalWrite(lamp, LOW);
      digitalWrite(indicator, LOW);
      Start=false;
      delay(1000);
  }
}
