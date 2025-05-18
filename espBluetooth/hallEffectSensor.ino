void hallEffectSetup() {
  pinMode(hall_din, INPUT_PULLUP);
  pinMode(hall_ain, INPUT);
  attachInterrupt(digitalPinToInterrupt(hall_din), magnetDetected, FALLING);  // Configura a interrupção no pino digital
  Serial.println(distance);
  delay(2000);
}

void magnetDetected() {
  // debounce é necessário para evitar capturar o imã duas vezes ou mais na mesma leitura
  static unsigned long debounce = millis();
  if ((millis() - debounce) > 300) {
    debounce = millis();
    if (i == 1) {
      lstTime = millis();
      currentI = i;
    }
    i++;
  }
}

float getDistance() {
  return i * distance;
}

float hallEffect() {
  interval = millis() - lstTime;
  if (interval >= updateRate) {
    lstTime = millis();
    RPXS = i - currentI;
    currentI = i;
    velS = (RPXS * distance / updateRate);
    velS = velS / (updateRate / 1000);
    vel = velS * 3600;
  }
  return vel;
}
