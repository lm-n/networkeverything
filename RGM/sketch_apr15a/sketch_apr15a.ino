#include <Servo.h>
/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
bool nextStepBegin;
bool marbleOut;
Servo berryservo;  // create servo object to control a servo
Servo blenderservo;
Servo marbleservo;
int blenderPin = 12;
int cupSensorPin = A5;    // select the input pin for the pressure sensor
int lightSensorPin = A0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  nextStepBegin = true;
  marbleOut = true;
  berryservo.attach(9);  // attaches the servo on pin 9 to the servo object
  blenderservo.attach(2);
  marbleservo.attach(4);
  berryservo.write(0);
  blenderservo.write(0);
  marbleservo.write(0);
  pinMode(blenderPin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int lightSensorValue = analogRead(lightSensorPin);
  // print out the value you read:
  if (nextStepBegin == true && lightSensorValue < 750){
    Serial.print("detected");
    berryservo.write(180);
    delay(10000);
    blenderservo.write(180);
    delay(5000);
    digitalWrite(blenderPin, HIGH);
    delay(10000);
    digitalWrite(blenderPin, LOW);
    delay(3000);
    blenderservo.write(0);
    delay(1000);
    nextStepBegin = false;
  }
  delay(1);        // delay in between reads for stability
  int cupSensorValue = analogRead(cupSensorPin);
  if (cupSensorValue > 400 && marbleOut){
    marbleservo.write(180);
    marbleOut == false;
  }
  delay(1);
}
