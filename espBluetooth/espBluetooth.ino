#include <String.h>
#include <PulseSensorPlayground.h>
#include "BluetoothSerial.h"

BluetoothSerial ESP_BT; // Cria uma instância do BluetoothSerial

// Declare an instance of PulseSensor to access all the PulseSensor Playground functions
PulseSensorPlayground pulseSensor;

char texto[60];
char tex[24];
char tex1[24];
char tex2[24];
char tex3[24];

// Definição dos pinos para ESP32
#define PULSE_BLINK 2   // Led on-board no ESP32
#define PULSE_FADE 15   // PWM no pino 15 (ajuste conforme necessário)
#define PULSE_INPUT 32  // Pino ADC para o sensor de pulso
#define analogInPin 32  // Pino ADC para o sensor EMG
#define analogOutPin 25 // Pino PWM de saída
#define hall_din 33     // Pino digital para o sensor de efeito Hall
#define hall_ain 35     // Pino ADC para o sensor de efeito Hall
const int THRESHOLD = 550;

int sensorValue = 0;
int outputValue = 0;
int ad_value;
int i = 0;
#define PI 3.1415926535897932384626433832795
#define updateRate 1000
#define radius 0.5
int currentI;
float distance = 2 * PI * radius;
float lstTime;
float interval;
float velS = 0;
float vel = 0;
int RPXS = 0;

int BPM;
int EMG;

const int pinX = 36;
const int pinY = 39;
int valueX = 0;
int valueY = 0;
int outputValueX = 0;
int outputValueY = 0;

void setup()
{
  Serial.begin(115200);
  hallEffectSetup();
  ESP_BT.begin("ESP32_Victus");

  analogReadResolution(10); // Configura a resolução do ADC para 10 bits (0 a 1023)

  // Configure the PulseSensor manager
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);
  pulseSensor.setSerial(Serial);
  pulseSensor.setThreshold(THRESHOLD);

  // Start reading the PulseSensor signal
  if (!pulseSensor.begin())
  {
    while (1)
    {
      // If the pulseSensor object fails, flash the led
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
}

int BPMSensor()
{
  // Return the actual BPM from the PulseSensor
  return pulseSensor.getBeatsPerMinute();
}

void loop()
{
  valueX = analogRead(pinX);
  // outputValueX = map(valueX, 0, 4095, 0, 255); // No ESP32, o ADC tem resolução de 12 bits (0 a 4095)
  outputValueX = map(valueX, 0, 1023, 0, 255); // Resolução ADC alterada para 10 bits por conta do BPM
//
//  if(outputValueX < 90){
//   outputValueX = 0;
//   }

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
  sprintf(tex3, "%d", int(getDistance()));
  strcat(texto, tex3);

  ESP_BT.println(texto);
  Serial.println(texto);

  delay(40);
  Serial.flush();
}
