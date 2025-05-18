#include <String.h>

char texto[60];
char tex[24];
char tex1[24];
char tex2[24];
char tex3[24];



/*
   Every Sketch that uses the PulseSensor Playground must
   define USE_ARDUINO_INTERRUPTS before including PulseSensorPlayground.h.
   Here, #define USE_ARDUINO_INTERRUPTS true tells the library to use
   interrupts to automatically read and process PulseSensor data.

   See ProcessEverySample.ino for an example of not using interrupts.
*/
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

/*
   Pulse Sensor Pinout:
     PULSE_INPUT = Analog Input. Connected to the pulse sensor
      purple (signal) wire.
     PULSE_BLINK = digital Output. Connected to an LED (and 220 ohm resistor)
      that will flash on each detected pulse.
     PULSE_FADE = digital Output. PWM pin onnected to an LED (and resistor)
      that will smoothly fade with each pulse.
      NOTE: PULSE_FADE must be a pin that supports PWM. Do not use
      pin 9 or 10, because those pins' PWM interferes with the sample timer.
      That's happens because we use timer 1 as compare match interruption.
*/
#define PULSE_INPUT A2
#define PULSE_BLINK 13    // Pin 13 is the on-board LED
#define PULSE_FADE 5
const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle

/* 
    EMG Sensor pinout:
    analogPin = Analog input pin that the sensor data pin is attached to
    analogOutPin = Analog or PWM pin that represent the sensor captured muscle force
*/
#define analogInPin A1
#define analogOutPin 9
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

/* 
    Hall effect sensor pinout:
    hall_din = Digital input that should be connected to the D0 pin of the sensor
    hall_ain = Analog input that receive data of the A0 pin of the sensor
*/
#define hall_din 10
#define hall_ain A0
int ad_value; // value read from hall analog input pin
int i = 0; // value wich counts the number of times the magnet has passed at the sensor
// for calculate the distance and velocity
#define PI 3.1415926535897932384626433832795
#define updateRate 1000 // time between each amount of time that we capture the magnet presence in milliseconds (just values multiple of 10)
#define radius 0.37 // m
int currentI;
float distance = 2*PI*radius; // m
float lstTime; // last time that the sensor notice the magnet
float interval; // interval between the first and the last time that the magnet passed by the sensor in milliseconds
float velS = 0; // m/s
float vel = 0; //km/h
int RPXS = 0; // rotation per x seconds


// variables to print on serial monitor
int BPM;
int EMG;

//Joystick
const int pinX = A3;
const int pinY = A4;
int valueX = 0;
int valueY = 0;
int outputValueX = 0;
int outputValueY = 0;


void setup(){
  Serial.begin(115200);
  hallEffectSetup();
  BPMSensorSetup();
}

void loop(){
  valueX = analogRead(pinX);
  outputValueX = map(valueX, 0, 1023, 0, 255);
  sprintf(texto, "%d", BPMSensor());
  strcat(texto, "#");
  char *valor2 = dtostrf(hallEffect(),5,2,tex);
  strcat(texto, valor2);
  strcat(texto, "#");
  sprintf(tex1, "%d", EMGSensor());
  strcat(texto, tex1);
  strcat(texto, "#");
  sprintf(tex2, "%d", outputValueX);
  strcat(texto, tex2);
  strcat(texto, "#");
  sprintf(tex3, "%d", distance());
  strcat(texto, tex3)
  
  
  Serial.println(texto);
  // Serial.print();
  // Serial.print("#");
  // Serial.print();
  // Serial.print("#");
  // Serial.println();
  delay(40);
  Serial.flush();
}
