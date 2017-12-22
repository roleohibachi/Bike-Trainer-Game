#include <TimerOne.h>
#include <ArduinoNunchuk.h>

const int samplePeriod = 250;  // msec

const int hatMax = 1024;  // TODO: ???
const int hallPin = 3;  // must be interruptable. 0, 1, 2, 3, 7 for Arduino
                        // Micro

int throttleMax = 30;  // mi/hr. TODO:Vary with pot?
const int mphPerRpm = 150; //calculated 197.416, we'll see!
int mph = 0;
int revs = 0;

int xmax = 0;
int zmax = 0;
int hat = -1;

ArduinoNunchuk nunchuck = ArduinoNunchuk();

void setup() {
  //cli();  // disable interrupts

  pinMode(hallPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(hallPin), throttleISR, RISING);
 
  Serial.begin(115200);

  Serial.print("throttle\t");
  Serial.print("nunZ\tnunC\t");
  Serial.print("nunX\tnunY");
  Serial.print("accX\taccY\taccZ");

  nunchuck.init();


  //Timer1.initialize(samplePeriod*1000); //microseconds = 250msec = 0.25sec
  //Timer1.attachInterrupt(timer1ISR); 
  //sei();  // allow interrupts
}

void loop() {
  // throttle
  // counted in throttleISR
  // mph updated in timer ISR
  Serial.print(mph);
  Serial.print("\t");

  // nunchuck
  nunchuck.update();

  Serial.print(nunchuck.zButton, DEC);
  Serial.print("\t");
  Serial.print(nunchuck.cButton, DEC);
  Serial.print("\t");
  Serial.print(nunchuck.analogX, DEC);
  Serial.print("\t");
  Serial.print(nunchuck.analogY, DEC);
  Serial.print("\t");
  Serial.print(nunchuck.accelX, DEC);
  Serial.print("\t");
  Serial.print(nunchuck.accelY, DEC);
  Serial.print("\t");
  Serial.print(nunchuck.accelZ, DEC);
  Serial.println();

  // accelerometer hat
  // poll for max value
  // update value in timer ISR
  xmax = (abs(nunchuck.accelX) > abs(xmax)) ? nunchuck.accelX : xmax;
  zmax = (abs(nunchuck.accelZ) > abs(zmax)) ? nunchuck.accelZ : zmax;
}

void timer1ISR(void) {  // timer1 interrupt
  cli();  // so this interrupt doesn't get interrupted mid-write by the other
          // one
  mph = revs*mphPerRpm;
  revs = 0;

  // TODO: test this, it's sloppy

  int hatx = xmax / hatMax;  // should result in -1, 0 or 1 horizontal twitch
  int hatz =
      ((zmax - 1) / hatMax) * 4;  // should result in -4, 0, 4 vertical twitch

  
  #define HATLEFT -1
  #define HATUP -4
  #define HATDOWN 4
  #define HATRIGHT 1
  #define NOHAT 0

  switch (hatx + hatz) {
    case NOHAT  :
      hat = -1;
      break;
    case HATUP  :
      hat = 0;
      break;
    case HATUP + HATRIGHT  :
      hat = 45;
      break;
    case HATRIGHT  :
      hat = 90;
      break;
    case HATDOWN + HATRIGHT  :
      hat = 135;
      break;
    case HATDOWN  :
      hat = 180;
      break;
    case HATDOWN + HATLEFT  :
      hat = 225;
      break;
    case HATLEFT  :
      hat = 270;
      break;
    case HATUP + HATLEFT  :
      hat = 315;
      break;
    default  :
      Serial.print("unexpected hat value. Condition: ");
      Serial.print(hatx + hatz);
      hat = -1;
  }
  
  sei();  // play ball
}

void throttleISR() { revs++; }


