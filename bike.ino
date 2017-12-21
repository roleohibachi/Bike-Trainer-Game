int throttleMax = 50; //starter value, roughly observed
const int hallPin = 3; //must be interruptable. 2 or 3 for Uno; 0, 1, 2, 3, 7 for Micro

int lastRev = 0;
int revs = 0;
boolean output = true;

Joystick_ Joystick( //optimized for nunchuck+bike throttle, I think
	JOYSTICK_DEFAULT_REPORT_ID, //	uint8_t hidReportId - Default: 0x03 - Indicates the joystick's HID report ID.
	JOYSTICK_TYPE_JOYSTICK, 	//	uint8_t joystickType - Default: JOYSTICK_TYPE_JOYSTICK or 0x04 - Indicates the HID input device type.
	2, 						//	uint8_t buttonCount - Default: 32 - Indicates how many buttons will be available on the joystick.
	0,							//	uint8_t hatSwitchCount - Default: 2 - Indicates how many hat switches will be available on the joystick. Range: 0 - 2
	true, 						//	bool includeXAxis - Default: true - Indicates if the X Axis is available on the joystick.
	true, 						//	bool includeYAxis - Default: true - Indicates if the Y Axis is available on the joystick.
	false, 						//	bool includeZAxis - Default: true - Indicates if the Z Axis is available on the joystick.
	false, 						//	bool includeRxAxis - Default: true - Indicates if the X Axis Rotation (in some situations this is the right Y Axis) is available on the joystick.
	false, 						//	bool includeRyAxis - Default: true - Indicates if the Y Axis Rotation is available on the joystick.
	false,						//	bool includeRzAxis - Default: true - Indicates if the Z Axis Rotation is available on the joystick.
	false, 						//	bool includeRudder - Default: true - Indicates if the Rudder is available on the joystick.
	true, 						//	bool includeThrottle - Default: true - Indicates if the Throttle is available on the joystick.
	false, 						//	bool includeAccelerator - Default: true - Indicates if the Accelerator is available on the joystick.
	false, 						//	bool includeBrake - Default: true - Indicates if the Brake is available on the joystick.
	false						//	bool includeSteering - Default: true - Indicates if the Steering is available on the joystick.
);
//TODO: to add turntable steering, "includeSteering".
//TODO: to add accelerometer, perhaps as a hat?
//TODO: brake control via actual brake?

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
  
  Joystick.setThrottleRange(0, throttleMax);
  Joystick.begin(false); //do not auto-update; I'll send updates via setThrottle(value) etc
  
  sei();//allow interrupts
}

void loop() {
  if(output){
    Serial.println(lastRev*60); //approx rotor RPM (not wheel rpm or speed. TODO) (max appears ~3000)
    Joystick.setThrottle(lastRev);
    output=false;
  }
}


ISR(TIMER1_COMPA_vect) { //timer1 interrupt
  cli(); //so this interrupt doesn't get interrupted mid-write by the other one
  lastRev = revs;
  revs = 0;
  output=true;
  sei();
}

void revISR(){
  revs++;
}

void setTimer(int msecs){
  //set timer1 interrupt (roughly)
  //timer1 is 16-bit on both uno and leonardo, so this should work.
  //TODO, see "arduino-timerone" code already written. It uses nice tricks to optimize and neaten.
  
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
