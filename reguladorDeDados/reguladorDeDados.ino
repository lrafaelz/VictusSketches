#include <String.h>
int BPM, EMG, vel;
char texto[60];
char tex[24];
char tex1[24];


// --- Variáveis Globais ---
int adc;                     //Armazena grandeza analógica


// --- Configurações Iniciais ---
void setup(){
Serial.begin(115200);
pinMode(A3, INPUT);
} //end setup

void loop(){
  adc = analogRead(A3);
  adc = map(adc,0,1023,1,100);
  // Serial.println(adc); // BPM#Vel#EMG
  BPM = map(adc, 1, 100, 50, 170);   
  vel = map(adc, 1, 100, 1, 20);
  EMG = map(adc, 1, 100, 300, 500);
  sprintf(texto, "%d", BPM);
  strcat(texto, "#");
  char *valor2 = dtostrf(vel,5,2,tex);  
  strcat(texto, valor2);
  strcat(texto, "#");
  sprintf(tex1, "%d", EMG);
  strcat(texto, tex1);
  // strcat(texto, "\n");
  // Serial.print(BPM);
  // Serial.print("#");
  // Serial.print(vel);
  // Serial.print("#");
  // Serial.println(EMG);
  Serial.println(texto);
  delay(30);
} //end loop