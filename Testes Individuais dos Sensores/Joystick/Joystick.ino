/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground through 220 ohm resistor

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInOutSerial
*/

// These constants won't change. They're used to give names to the pins used:
const int pinX = A0;  // Analog input pin that the potentiometer is attached to
const int pinY = A1;  // Analog input pin that the potentiometer is attached to

int valueX = 0;  // value read from the pot
int valueY = 0;  // value read from the pot
int outputValueX = 0;  // value output to the PWM (analog out)
int outputValueY = 0;  // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  valueX = analogRead(pinX);
  valueY = analogRead(pinY);
  // map it to the range of the analog out:
  outputValueX = map(valueX, 0, 1023, 0, 255);
  outputValueY = map(valueY, 0, 1023, 0, 255);

  // print the results to the Serial Monitor:
  Serial.print("X = ");
  Serial.print(outputValueX);
  Serial.print("\t Y = ");
  Serial.println(outputValueY);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}
