#include <String.h>
#include <PulseSensorPlayground.h>
#include "BluetoothSerial.h"


BluetoothSerial ESP_BT;  // Cria uma instância do BluetoothSerial

char texto[60];
char tex[24];
char tex1[24];
char tex2[24];
char tex3[24];

// Definição dos pinos para ESP32
#define PULSE_BLINK 2   // Led on-board no ESP32
#define PULSE_FADE 15   // PWM no pino 15 (ajuste conforme necessário)
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

const int pinX = 36;  // Defina os pinos do Joystick conforme necessário
const int pinY = 39;
int valueX = 0;
int valueY = 0;
int outputValueX = 0;
int outputValueY = 0;

void setup(){
  Serial.begin(115200);
  hallEffectSetup();
  ESP_BT.begin("ESP32_Victus");

}

int BPMSensor() {
  // Simula um valor de BPM aleatório entre 60 e 100
  int simulatedBPM = random(60, 100);
  return simulatedBPM;
}


void loop(){
  valueX = analogRead(pinX);
  outputValueX = map(valueX, 0, 4095, 0, 255);  // No ESP32, o ADC tem resolução de 12 bits (0 a 4095)
  
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

  delay(40);
  Serial.flush();
}
