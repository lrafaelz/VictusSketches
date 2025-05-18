#include <BluetoothSerial.h>
#include <String.h>

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

// Bluetooth Serial object
BluetoothSerial SerialBT;

char texto[60];
char tex[24];
char tex1[24];
char tex2[24];
char tex3[24];

// Hall Effect Sensor
#define hall_din 33 // Changed to D33 as requested
#define hall_ain 35 // Changed to D35 as requested
int ad_value;       // value read from hall analog input pin
int i = 0;          // value which counts the number of times the magnet has passed at the sensor
#define PI 3.1415926535897932384626433832795
#define updateRate 1000 // time between each amount of time that we capture the magnet presence in milliseconds
#define radius 0.37     // m
int currentI;
float distance_val = 2 * PI * radius; // m
float lstTime;                        // last time that the sensor notice the magnet
float interval;                       // interval between the first and the last time that the magnet passed by the sensor in milliseconds
float velS = 0;                       // m/s
float vel = 0;                        // km/h
int RPXS = 0;                         // rotation per x seconds

// Pulse Sensor
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#define PULSE_INPUT 36 // Changed to avoid conflict with hall_ain on D35
#define PULSE_BLINK 2  // Changed from 13 to avoid conflicts
#define PULSE_FADE 5
const int THRESHOLD = 550;
const int OUTPUT_TYPE = SERIAL_PLOTTER;
PulseSensorPlayground pulseSensor;
int BPM;

// EMG Sensor
#define analogInPin 32 // Changed to ESP32 analog pin
#define analogOutPin 4 // Changed to ESP32 PWM pin
int sensorValue = 0;
int outputValue = 0;

// Joystick
const int pinX = 2;  // Changed to D2 as requested
const int pinY = 15; // Changed to D15 as requested
int valueX = 0;
int valueY = 0;
int outputValueX = 0;
int outputValueY = 0;

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("Victus_ESP32"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with Bluetooth!");

  hallEffectSetup();
  BPMSensorSetup();
}

void loop()
{
  // Read joystick values
  valueX = analogRead(pinX);
  outputValueX = map(valueX, 0, 4095, 0, 255); // ESP32 has 12-bit ADC (0-4095)

  valueY = analogRead(pinY);
  outputValueY = map(valueY, 0, 4095, 0, 255);

  // Compile all data into a string
  sprintf(texto, "%d", BPMSensor());
  strcat(texto, "#");
  char *valor2 = dtostrf(hallEffect(), 5, 2, tex);
  strcat(texto, valor2);
  strcat(texto, "#");
  sprintf(tex1, "%d", EMGSensor());
  strcat(texto, tex1);
  strcat(texto, "#");
  sprintf(tex2, "%d", outputValueX);
  strcat(texto, tex2);
  strcat(texto, "#");
  sprintf(tex3, "%d", distance());
  strcat(texto, tex3);

  // Send data via Serial and Bluetooth
  Serial.println(texto);
  SerialBT.println(texto);

  delay(40);
  Serial.flush();
  SerialBT.flush();
}

// EMG Sensor function
int EMGSensor()
{
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 4095, 0, 255); // ESP32 has 12-bit ADC
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);
  return sensorValue;
}

// Hall Effect Sensor functions
void hallEffectSetup()
{
  pinMode(hall_din, INPUT_PULLUP);
  pinMode(hall_ain, INPUT);
  attachInterrupt(digitalPinToInterrupt(hall_din), hallSensorInterrupt, CHANGE);
  Serial.println(distance_val);
  delay(2000);
}

void hallSensorInterrupt()
{
  // debounce is necessary to not catch the magnet twice or more at the same read
  static unsigned long debounce = millis();
  if ((millis() - debounce) > 300)
  {
    debounce = millis();
    if (i == 1)
    {
      lstTime = millis();
      currentI = i;
    }
    i++;
  }
}

float distance()
{
  return i * distance_val;
}

float hallEffect()
{
  interval = millis() - lstTime;
  if (interval >= updateRate)
  {
    lstTime = millis();
    RPXS = i - currentI;
    currentI = i;
    velS = (RPXS * distance_val / updateRate);

    velS = velS / (updateRate / 1000);
    vel = velS * 3600;
  }
  return vel;
}

// BPM Sensor functions
void BPMSensorSetup()
{
  // Configure the PulseSensor manager
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

  // Now that everything is ready, start reading the PulseSensor signal
  if (!pulseSensor.begin())
  {
    // If initialization fails, blink LED to show error
    while (1)
    {
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
}

int BPMSensor()
{
  delay(20);

  if (pulseSensor.sawStartOfBeat())
  {
    return pulseSensor.getBeatsPerMinute();
  }
  return BPM; // Return last BPM if no new beat detected
}