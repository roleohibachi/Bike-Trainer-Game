#include <Joystick.h>
#include <Arduinonunchuck.h>

int throttleMax = 50; //starter value, roughly observed
const int hatMax = 1024;//???

const int hallPin = 3; //must be interruptable. 2 or 3 for Uno; 0, 1, 2, 3, 7 for Micro

int lastRev = 0;
int revs = 0;

int xmax = 0;
int zmax = 0;
int hat = JOYSTICK_HATSWITCH_RELEASE;

Arduinonunchuck nunchuck = Arduinonunchuck();

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
	attachInterrupt(digitalPinToInterrupt(hallPin), throttleISR, RISING);

	msecTimer(250);

	Serial.begin(115200);

	Serial.print("throttle\t"); 
	Serial.print("nunZ\tnunC\t"); 
	Serial.print("nunX\tnunY"); 
	Serial.print("accX\taccY\taccZ"); 

	Joystick.setThrottleRange(0, throttleMax);
	Joystick.setXAxisRange(-127, 127);//TODO: no idea
	Joystick.setYAxisRange(-127, 127);//TODO: no idea
	Joystick.begin(false); //do not auto-update; I'll send updates via setThrottle(value) etc

	nunchuck.init();

	sei();//allow interrupts
}

void loop() {

	//throttle
	//counted in throttleISR
	//lastrev updated in timer ISR
	Serial.print(lastRev);
	Serial.print("\t");
	Joystick.setThrottle(lastRev);

	//nunchuck
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

	Joystick.setButton(0, nunchuck.zButton);
	Joystick.setButton(1, nunchuck.cButton);
	Joystick.setXAxis(nunchuck.analogX);
	Joystick.setYAxis(nunchuck.analogY);

	//accelerometer hat
	//poll for max value
	//update value in timer ISR
	xmax = (abs(nunchuck.accelX) > abs(xmax))? nunchuck.accelX : xmax;
	zmax = (abs(nunchuck.accelZ) > abs(zmax))? nunchuck.accelZ : zmax;
	Joystick.setHatSwitch(hatSwitch, hat);

	Joystick.sendState()
}


ISR(TIMER1_COMPA_vect) { //timer1 interrupt
	cli(); //so this interrupt doesn't get interrupted mid-write by the other one
	lastRev = revs;
	revs = 0;

	//TODO: test this, it's sloppy

	int hatx = xmax/hatMax;//should result in -1, 0 or 1 horizontal twitch
	int hatz = ((zmax-1)/hatMax)*4;//should result in -4, 0, 4  vertical twitch

	#define HATLEFT		-1
	#define HATUP		-4
	#define HATDOWN		4
	#define HATRIGHT	1
	#define NOHAT		0

	switch(hatx+hatz) {
	 case NOHAT: 
		hat = JOYSTICK_HATSWITCH_RELEASE);
		break; 
	 case HATUP:
		hat = 0;
		break; 
	 case HATUP+HATRIGHT:
		hat = 45;
		break; 
	 case HATRIGHT: 
		hat = 90;
		break; 
	 case HATDOWN+HATRIGHT: 
		hat = 135;
		break; 
	 case HATDOWN:
		hat = 180;
		break; 
	 case HATDOWN+HATLEFT:
		hat = 225;
		break; 
	 case HATLEFT: 
		hat = 270;
		break; 
	 case HATUP+HATLEFT: 
		hat = 315;
		break; 
	 default :
	   Serial.print("unexpected hat value. Condition: ");
	   Serial.print(hatx+hatz);   
	   hat = JOYSTICK_HATSWITCH_RELEASE;
	}

	sei(); //play ball
}

void throttleISR(){
revs++;
}

void msecTimer(int msecs){
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
