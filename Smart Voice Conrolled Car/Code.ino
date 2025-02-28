#include <SoftwareSerial.h>
#include <Servo.h>
int servopin = 13;
 int trigPinFWDSensor = 9;
 int trigPinREVSensor = 12;
 int echoPinFWDSensor = 10;
 int echoPinREVSensor = 2;
 int forwardSensorLED = 7;
 int reverseSensorLED = 11;
 int pos = 0;

Servo myservo;
SoftwareSerial BT(0, 1); //TX, RX respetively
String readvoice;

int PIN3STATUS,PIN4STATUS,PIN5STATUS,PIN6STATUS;

void setup() {
 //Serial.begin(9600);
 pinMode(forwardSensorLED, OUTPUT);
 pinMode(reverseSensorLED,OUTPUT);
 pinMode(trigPinFWDSensor, OUTPUT);
 pinMode(echoPinFWDSensor, INPUT);
 pinMode(trigPinREVSensor, OUTPUT);
 pinMode(echoPinREVSensor, INPUT);


 BT.begin(9600);
 Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}
 int duration,distance;
 int distanceRev, durationRev;

 void activateForward()
 {
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6,LOW);
    PIN3STATUS = HIGH;
    PIN4STATUS = HIGH;
    PIN5STATUS = LOW;
    PIN6STATUS = LOW;
 }


 void stopMotors()
 {
   digitalWrite(3, LOW);
   digitalWrite(4, LOW);
   digitalWrite(5, LOW);
   digitalWrite(6,LOW);
   PIN3STATUS = LOW;
   PIN4STATUS = LOW;
   PIN5STATUS = LOW;
   PIN6STATUS = LOW;
 }

 void activateReverse()
 {
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    PIN3STATUS = LOW;
    PIN4STATUS = LOW;
    PIN5STATUS = HIGH;
    PIN6STATUS = HIGH;
 }
void checkSensorStatus()
{


    delayMicroseconds(2);

if(PIN3STATUS || PIN4STATUS)
{
    digitalWrite(trigPinFWDSensor,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinFWDSensor,HIGH);
    delayMicroseconds(2);
    digitalWrite(trigPinFWDSensor,LOW);
    delayMicroseconds(100);
    duration = pulseIn(echoPinFWDSensor,HIGH);
    distance = 0.0343 * (duration/2);
    delayMicroseconds(100);
   if(distance>25)
   {
    digitalWrite(forwardSensorLED, LOW);
    digitalWrite(reverseSensorLED, LOW);
    delay(1);

   }
   else
  {
     digitalWrite(forwardSensorLED, HIGH);
     digitalWrite(reverseSensorLED, LOW);
     stopMotors();
     delay(1);
  }

}
else if(PIN5STATUS || PIN6STATUS)
{
    digitalWrite(trigPinREVSensor,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinREVSensor,HIGH);
    delayMicroseconds(2);
    digitalWrite(trigPinREVSensor,LOW);
    delayMicroseconds(100);
    durationRev = pulseIn(echoPinREVSensor,HIGH);
    distanceRev = 0.0343 * (durationRev/2);
    
   if(distanceRev > 25)
  {
     digitalWrite(forwardSensorLED, LOW);
     digitalWrite(reverseSensorLED, LOW);
  }
   else
  {
     digitalWrite(forwardSensorLED, LOW);
     digitalWrite(reverseSensorLED, HIGH);
     stopMotors();
     delay(1);
  }
}
else
{

}

}

void loop() {



  long duration, distance;
  digitalWrite(trigPinFWDSensor,HIGH);
  digitalWrite(trigPinREVSensor,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPinFWDSensor, LOW);
  digitalWrite(trigPinREVSensor, LOW);

  checkSensorStatus();

 while (BT.available()){  //Check if there is an available byte to read
  delay(10); //Delay added to make thing stable
  char c = BT.read(); //Conduct a serial read
  readvoice += c; //build the string- "forward", "reverse", "left" and "right", "start blinking"
  }
  if (readvoice.length() > 0) {
    Serial.println(readvoice);


      if(readvoice == "*forward#")
  {
    PIN3STATUS = HIGH;
    PIN4STATUS = HIGH;
    PIN5STATUS = LOW;
    PIN6STATUS = LOW;
    digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(100);
  }

 else if(readvoice == "*front#")
  {
    PIN3STATUS = HIGH;
    PIN4STATUS = HIGH;
    PIN5STATUS = LOW;
    PIN6STATUS = LOW;
    digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(100);
  }


else if(readvoice == "*backward#")
  {
  PIN5STATUS = HIGH;
  PIN6STATUS = HIGH;
  PIN3STATUS = LOW;
  PIN4STATUS = LOW;
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6,HIGH);
    delay(100);
  }

else if(readvoice == "*back#")
  {
  PIN5STATUS = HIGH;
  PIN6STATUS = HIGH;
  PIN3STATUS = LOW;
  PIN4STATUS = LOW;
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6,HIGH);
    delay(100);
  }

  else if (readvoice == "*left#")
  {
    PIN3STATUS = HIGH;
    PIN4STATUS = LOW;
    PIN5STATUS = LOW;
    PIN6STATUS = HIGH;
    digitalWrite (3,HIGH);
    digitalWrite (4,LOW);
    digitalWrite (5,LOW);
    digitalWrite (6,HIGH);
   delay (800);
      digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(100);

    PIN3STATUS = HIGH;
  PIN4STATUS = HIGH;
  PIN5STATUS = LOW;
  PIN6STATUS = LOW;

  }

 else if ( readvoice == "*right#")
 {
   digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, HIGH);
   digitalWrite (6, LOW);
   PIN3STATUS = LOW;
   PIN4STATUS = HIGH;
   PIN5STATUS = HIGH;
   PIN6STATUS = LOW;
   delay (800);
      digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(100);
    PIN3STATUS = HIGH;
  PIN4STATUS = HIGH;
  PIN5STATUS = LOW;
  PIN6STATUS = LOW;
 }

 else if (readvoice == "*stop#")
 {
   digitalWrite (3, LOW);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   PIN3STATUS = LOW;
   PIN4STATUS = LOW;
   PIN5STATUS = LOW;
   PIN6STATUS = LOW;
   delay (100);
 }

 else if (readvoice == "*keep watch in all direction#")
 {
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay (100);

 }
  else if (readvoice == "*show me Garba#")
 {
 digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay (400);
      digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(600);
    digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, HIGH);
   digitalWrite (6, LOW);
   delay (500);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, HIGH);
   delay (500);

digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay (400);
      digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(600);
    digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, HIGH);
   digitalWrite (6, LOW);
   delay (500);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, HIGH);
   delay (500);digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay (400);
      digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(600);
    digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, HIGH);
   digitalWrite (6, LOW);
   delay (500);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, HIGH);
   delay (500);
   digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay (400);
    digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(600);
    digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, HIGH);
   digitalWrite (6, LOW);
   delay (500);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, HIGH);
   delay (500);
   digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay (400);
    digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(600);
    digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, HIGH);
   digitalWrite (6, LOW);
   delay (500);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, HIGH);
   delay (500);digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay (400);
      digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(600);
    digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, HIGH);
   digitalWrite (6, LOW);
   delay (500);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, HIGH);
   delay (500);digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay (400);
      digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(600);
    digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, HIGH);
   digitalWrite (6, LOW);
   delay (500);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, HIGH);
   delay (500);digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, LOW);
   digitalWrite (6, LOW);
   delay (400);
      digitalWrite(3, HIGH);
    digitalWrite (4, HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    delay(600);
    digitalWrite (3, LOW);
   digitalWrite (4, HIGH);
   digitalWrite (5, HIGH);
   digitalWrite (6, LOW);
   delay (500);
   digitalWrite (3, HIGH);
   digitalWrite (4, LOW);
   digitalWrite (5, LOW);
   digitalWrite (6, HIGH);
   delay (500);
 }


 readvoice="";}} //Reset the variable
