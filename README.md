# Bike Trainer Game Controller

This needs a better name. It is not, itself, a game. Recommendations are appreciated.

This project turns a "dumb" bike trainer into a joystick/throttle controller that can be used to play games. Pedal speed is a USB throttle; a Wii nunchuck serves as a controller. You can fly a flight sim (fun), play a driving game (hard), or even map it to an FPS (Untested). It works with linux, mac, windows, consoles that support USB joysticks, and mobile devices that support both HID Joysticks and USB-OTG connections. 

# Hardware

This requires an ATMega32U4 device, such as an Arduino Leonardo or Micro. It very well could work on a Teensy - if you try it, let me know. Just be sure to use a 3.3v microcontroller (or a level shifter), so you don't fry the nunchuck.

I used [this](http://a.co/gge61I9) bike trainer. Remove the 3 screws and plastic panel covering the rotor. Add two small permanent magnets to the inside of the rotor. Put a digital Hall effect sensor through the slots in the rotor's plastic cover. These should be cemented in place once they are verified to be functional, since it spins at a few thousand RPM. TODO: pictures.

A direct-drive bike trainer would reduce slippage and wear of the rear tire, but it's perfectly usable as-is.

The nunchuck works most comfortably when riding with aero bars, since the rider's hands don't have to operate independently. Aero bars with a "loop" design or a connecting bridge are also a nice place to mount a display or mobile device that won't mess with the riding position.

# Software

You should just read it, it's pretty simple. There are two interrupts: one timer, and a rising edge trigger on the Hall sensor. Every time the timer fires, it sends the RPM as a joystick update.

The nunchuck's joystick is mapped to the HID joystick's X and Y axes, and polled. "Flicking" the nunchuck will send an 8-position hat switch command via the accelerometer, when the timer fires (a poor man's rate limiter). 

# Future work
- A potentiometer to scale throttle range, adjusting difficulty of achieving 100% throttle
- A rotary-encoded turntable under the front wheel riser to sense steering position, for 2D games (like driving)
- A second Hall sensor for pedal cadence, which can be divided by wheel RPM to determine gear ratio
- A linear potentiometer could also determine derailleur and brake positions. Braking input would be useful in driving games.
- A generator to add variable resistance (and power!), with credit to www.genesgreenmachine.com. This may make RPM measurement nicer by eliminating those magnets and correlating voltage to throttle position. Force feedback, I understand, is harder. I would have to abandon the HID Joystick approach (since PID FF drivers are dead, right?) and instead emulate a device with FF support in its driver. 
- Logging data to fitness platforms (Strava, Garmin, whatever). The current design makes this a big step, involving non-HID drivers, SD cards, wifi-enabled microcontrollers, or something else. If you are reading this, please chime in with ideas.

# External Dependencies
- Matthew Heironimus's [ArduinoJoystick](https://github.com/MHeironimus/ArduinoJoystickLibrary) library, to appear as a USB HID Joystick.
- Gabriel Bianconi's [arduino-nunchuck](https://github.com/GabrielBianconi/arduino-nunchuk) library, to demux the i2c data from the nunchuck.
- Paul Stoffregen's [modified TimerOne](https://github.com/PaulStoffregen/TimerOne) library. I'm unable to make this work with my boards so far, but I would like to, so that timers work across different boards.
