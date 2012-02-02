/*
 On the fader we tested (inside a Vestax PMC-05 Pro III) we found that the
 fader ran from 0V to +10V. One of the pins went from 0V to +10V as you
 moved left to right, and another pin went from +10V to 0V as you moved
 left to right. We soldered on to both of these pins, and halved their voltage
 using a simple voltage divider circuit with equal valued resistors. 2x 10K
 or 2x 100K resistors should work well for the voltage divider. The ground
 should be connected to the Arduino ground as well as the fader ground. The
 tap in the center of the voltage dividers should be fed into the analog input
 on pin A0 and A1.
 
 This sketch will then send back a single value describing the position of the
 fader to the ScratchOSC app.
*/

const int analogInPin0 = A0;  // Analog input pin for Channel 1
const int analogInPin1 = A1;  // Analog input pin for Channel 2
int sensorValue0, sensorValue1;

void setup() {
  // initialize serial communications at 115200 baud
  Serial.begin(115200); 
}

void loop() {
  // read the two voltages
  sensorValue0 = analogRead(analogInPin0);
  sensorValue1 = analogRead(analogInPin1);

  // sum the two analogRead values
  int final = (1024 - sensorValue1) + sensorValue0;
  
  // divide by 2 to average the two values
  // divide by 4 to convert 10 bits -> 8 bits 
  final /= 4 * 2;
  
  // send the single byte over serial
  Serial.write((byte) final);

  delay(0);                     
}

