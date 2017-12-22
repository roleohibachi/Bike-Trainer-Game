# Bike Trainer Game Controller

This needs a better name. It is not, itself, a game. Recommendations are appreciated.

This project creates a bike trainer that can be used to play games. Pedal speed is a USB throttle; a Wii nunchuck serves as a controller. You can fly a flight sim (fun), play a driving game (hard), or even map it to an FPS (Untested). It works with linux, mac, windows, consoles that support USB joysticks, and mobile devices that support both HID Joysticks and USB-OTG connections. The nunchuck works most comfortably when riding with aero bars.

# Hardware

This requires an ATMega32U4 device, such as an Arduino Leonardo or Micro. Eventually, it should also work on a Teensy. Be sure to use a 3.3v microcontroller (or a level shifter), so you don't fry the nunchuck.

I used [this](http://a.co/gge61I9) bike trainer. Remove the 3 screws and plastic panel covering the rotor. Add two small permanent magnets to the inside of the rotor. Put a digital Hall effect sensor through the slots in the rotor's plastic cover. These should be cemented in place once they are verified to be functional, since it spins at a few thousand RPM. TODO: pictures.

# Software

You should just read it, it's pretty simple. There's two interrupts: one timer, and a rising edge trigger on the Hall sensor. Every time the timer fires, it sends the RPM as a joystick update.

The nunchuck's joystick is mapped to the HID joystick's X and Y axes, and polled. "Flicking" the nunchuck will send an 8-position hat switch command via the accelerometer, when the timer fires (a poor man's rate limiter). 

# Future work
- A potentiometer to scale throttle range, adjusting difficulty of achieving 100% throttle
- A rotary-encoded turntable under the front wheel to sense steering position, for 2D games (like driving)
- A second Hall sensor for pedaling cadence, which can be divided by wheel RPM to determine gear ratio
- A generator to add variable resistance (and power!), with credit to www.genesgreenmachine.com. This may make RPM measurement nicer by eliminating those magnets and correlating voltage to throttle position.
- Logging of game-workout data to fitness platforms (Strava, Garmin, whatever). 

# External Dependencies
- Matthew Heironimus's [ArduinoJoystick](https://github.com/MHeironimus/ArduinoJoystickLibrary) library, to appear as a USB HID Joystick.
- Gabriel Bianconi's [arduino-nunchuck](https://github.com/GabrielBianconi/arduino-nunchuk) library, to demux the i2c data from the nunchuck.
