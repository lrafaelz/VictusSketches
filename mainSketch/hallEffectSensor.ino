

void hallEffectSetup()
{
  pinMode(hall_din,INPUT_PULLUP);
  pinMode(hall_ain,INPUT);
  pinChangeSetup();
  Serial.println(distance);
  delay(2000);
}

void pinChangeSetup(){
  cli();
  PCICR = 1;
  PCMSK0 = 16;
  sei();
}

ISR(PCINT0_vect){
  cli();
  // debounce is necessary to not catch the magnet twice or more at the same read
  static unsigned long debounce = millis();
  if((millis() - debounce) > 300){
    debounce = millis();
    if(i == 1){
      lstTime = millis();
      currentI = i;
    }
    i++;
    // Serial.println(i);
  }
  sei();
}

float distance(){
  return i*distance;
}

float hallEffect(){
  interval = millis() - lstTime;
  if(interval >= updateRate){
    
    lstTime = millis();
    RPXS = i - currentI;
    currentI = i;
    velS = (RPXS*distance/updateRate);
      
    velS = velS/(updateRate/1000);
    vel = velS*3600;
  }
  // Serial.println(vel);
  
  return vel;
}
