#include "BluetoothSerial.h"

// Cria uma instância do objeto BluetoothSerial
BluetoothSerial ESP_BT;

// --- Definição dos Pinos para o ESP32 ---
// É uma boa prática usar 'const int' para definir os pinos.
// Escolhi pinos do ADC1, que são mais estáveis para leitura analógica.
const int potPin = 34;  // Pino para o potenciômetro (simula BPM, vel, EMG) - GPIO34 é ADC1_CH6
const int joyXPin = 36; // Pino para o eixo X do joystick (simula direção) - GPIO36 é ADC1_CH0
const int distPin = 35; // Pino para o sensor de distância - GPIO35 é ADC1_CH7

// --- Configurações Iniciais ---
void setup()
{
  // Inicia a comunicação Serial via USB para debug
  Serial.begin(115200);

  // Inicia o Bluetooth com um nome específico para o dispositivo
  ESP_BT.begin("Victus_Exergame_ESP32");

  Serial.println("ESP32 pronto. Conecte via Bluetooth ('Victus_Exergame_ESP32') para receber os dados.");
}

// --- Loop Principal ---
void loop()
{
  // 1. Leitura dos valores analógicos brutos
  // O ADC do ESP32 tem 12 bits, então os valores vão de 0 a 4095
  int potValue = analogRead(potPin);
  int joyXValue = analogRead(joyXPin);
  int distValue = analogRead(distPin); // Leitura do sensor de distância

  // 2. Mapeamento e Simulação dos Dados
  // Primeiro, mapeamos o valor do potenciômetro para uma escala de controle de 1 a 100
  // Note que usamos 4095 como máximo de entrada, diferente do Arduino (1023)
  long controlValue = map(potValue, 0, 4095, 1, 100);

  // Agora, usamos esse valor de controle para gerar os dados simulados
  int BPM = map(controlValue, 1, 100, 50, 170);
  int EMG = map(controlValue, 1, 100, 300, 500);

  // Para a velocidade (vel), calculamos diretamente como float para ter casas decimais
  float vel = (float)controlValue / 100.0f * 20.0f;

  // Mapeamos o valor do joystick para a direção (0-255)
  int direcao = map(joyXValue, 0, 4095, 0, 255);

  // Mapeamos a distância para um valor em centímetros (0-300cm)
  int distance = map(distValue, 0, 4095, 0, 300);

  // 3. Formatação da String de Saída
  // Usamos um único 'sprintf' para criar a string final. É mais eficiente e limpo.
  // Formato: "BPM#vel#EMG#direcao#distance"
  char dataString[60];
  // "%.2f" formata a variável 'vel' como um float com 2 casas decimais
  sprintf(dataString, "%d#%.2f#%d#%d#%d", BPM, vel, EMG, direcao, distance);

  // 4. Envio dos Dados
  // Envia a string formatada tanto pela Serial USB quanto pelo Bluetooth
  Serial.println(dataString);
  ESP_BT.println(dataString);

  // 5. Delay
  // Uma pequena pausa para não sobrecarregar a comunicação
  delay(30);
}
