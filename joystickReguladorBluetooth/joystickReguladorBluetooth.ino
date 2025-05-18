#include <String.h>
#include "BluetoothSerial.h"+ç

BluetoothSerial ESP_BT; // Cria uma instância do BluetoothSerial

char texto[60];
char tex[24];
char tex1[24];

// Definição dos pinos para ESP32
const int pinX = 36;   // Pino ADC para o eixo X do Joystick
const int pinY = 39;   // Pino ADC para o eixo Y do Joystick
const int potPin = 34; // Pino ADC para o potenciômetro (regulador)

int valueX = 0;
int valueY = 0;
int potValue = 0;
int outputValueX = 0;
int outputValueY = 0;
int outputValuePot = 0;

void setup()
{
  Serial.begin(115200);
  ESP_BT.begin("ESP32_Victus_Joy");
}

void loop()
{
  // Leitura dos valores do joystick
  valueX = analogRead(pinX);
  valueY = analogRead(pinY);

  // Leitura do potenciômetro
  potValue = analogRead(potPin);

  // Mapeamento dos valores (ESP32 usa ADC de 12 bits - 0 a 4095)
  outputValueX = map(valueX, 0, 4095, 0, 255);
  outputValueY = map(valueY, 0, 4095, 0, 255);
  outputValuePot = map(potValue, 0, 4095, 0, 100); // Mapeia para 0-100%

  // Formatação da string para envio via Bluetooth
  // Formato: "X#Y#POT"
  sprintf(texto, "%d", outputValueX);
  strcat(texto, "#");
  sprintf(tex, "%d", outputValueY);
  strcat(texto, tex);
  strcat(texto, "#");
  sprintf(tex1, "%d", outputValuePot);
  strcat(texto, tex1);

  // Envio dos dados via Bluetooth
  ESP_BT.println(texto);

  // Envio dos dados também pela Serial para debug
  Serial.println(texto);

  delay(40); // Pequeno delay para estabilidade
  Serial.flush();
}
