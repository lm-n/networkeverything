/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

// read the input on analog pin 0:
int sensor0Pin = A0;
int sensor1Pin = A1;
int button0Pin = 12;
int button1Pin = 13;
int sensor0Value, sensor1Value;
int button0Value, button1Value;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //establishContact();
}

// the loop routine runs over and over again forever:
void loop() {
  sensor0Value = analogRead(sensor0Pin);
  sensor1Value = analogRead(sensor1Pin);
  button0Value = digitalRead(button0Pin);
  button1Value = digitalRead(button1Pin);
  // print out the value you read:
  //if (Serial.available() > 0) {
    Serial.print(sensor0Value);
    Serial.print(',');
    Serial.print(sensor1Value);
    Serial.print(',');
    Serial.print(button0Value);
    Serial.print(',');
    Serial.println(button1Value);
    //delay(50);        // delay in between reads for stability
  //}
}

// this function runs until a connected device sends a byte
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0,0");   // send an initial string
    delay(300);
  }
}
