
const int hallPin = 3; //must be interruptable. 2 or 3 for Uno; 0, 1, 2, 3, 7 for Micro

int lastRev = 0;
int revs = 0;
boolean output = true;

//Joystick_ Joystick;

void setup() {
  cli();//disable interrupts

  pinMode(hallPin, INPUT);
  pinMode(btnPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(hallPin), revISR, RISING);

  setTimer(250);

  Serial.begin(115200);
  
  //Serial.print("Time");
  //Serial.print("\t");
  Serial.println("RPM"); 

  
  sei();//allow interrupts
}

void loop() {
  if(output){
    Serial.println(lastRev*60); 
    output=false;
  }
}


ISR(TIMER1_COMPA_vect) { //timer1 interrupt
  lastRev = revs;
  revs = 0;
  output=true;
}

void revISR(){
  revs++;
}

void setTimer(int msecs){
  //set timer1 interrupt (roughly)
  
  int compMatch = (int) (msecs * 15.624);
  
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B

  // turn on CTC mode
  TCCR1B |= (1 << WGM12); //can I just (1 << CTC1) ? It would make more sense.
  
  //OCR1A is compared to TCNT1.
  TCNT1  = 0;//initialize counter value to 0
  
  // Set CS12 and CS10 bits for 1024 prescaler (max)
  TCCR1B |= (1 << CS12) | (1 << CS10);
  
  // set compare match register for increments
  //OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536) (this gives 1sec)
  //OCR1A = 3906;// = (16*10^6) / (0.25*1024) - 1 (must be <65536) (this gives 250msec)
  OCR1A = (compMatch < 65536)? compMatch : 65536;
  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}
