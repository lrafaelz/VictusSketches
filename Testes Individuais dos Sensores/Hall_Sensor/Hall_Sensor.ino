int hall_din=10;
int hall_ain=A0;
int ad_value;
int i = 0;
int currentI = 0;

// for calculate the distance and velocity
#define PI 3.1415926535897932384626433832795
#define updateRate 1000 // time between each amount of time that we capture the magnet presence in milliseconds (just values multiple of 10)
#define radius 0.37 // m
float distance = 2*PI*radius;; // m
float lstTime; // last time that the sensor notice the magnet
float interval; // interval between the first and the last time that the magnet passed by the sensor in milliseconds
float velS = 0; // m/s
float vel = 0; //km/h
int RPXS = 0; // rotation per x seconds


void setup()
{
  Serial.begin(115200);
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

void loop(){
  interval = millis() - lstTime;
  if(interval >= updateRate){
    lstTime = millis();
    RPXS = i - currentI;
    currentI = i;
    velS = (RPXS*distance/updateRate);
    velS = velS/(updateRate/1000);
    vel = velS*3600;
  }
  Serial.println(vel);
  delay(1000);
}