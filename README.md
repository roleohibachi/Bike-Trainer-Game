# Bike Trainer Game

This project needs a better name. It is not, itself, a game. Recommendations are appreciated.

I have a bike trainer. It was cheap on Amazon. http://a.co/gge61I9

This project monitors RPM using a hall sensor. It requires an ATMega32U4 device, such as an Arduino Leonardo or Micro. With some refactoring, it should also work on a Teensy.

It sends that RPM as a HID Joystick's throttle input, using the ArduinoJoystick library created by Matthew Heironimus, linked below, not included. You can fly a flight sim (fun), play a driving game (hard), or even map it to an FPS (Untested). It works with linux, mac, windows, consoles that support USB HID Joysticks, and mobile devices that support both HID Joysticks and USB-OTG connections.

It also uses a wii nunchuck library created by Gabriel Bianconi, linked below, not included. The nunchuck is a great form factor for controlling a game while riding, especially with aero bars. It speaks i2c. Be sure to use a 3.3v microcontroller (or a level shifter), so you don't fry the 'chuck.

To set up my specific model of trainer, I added two small permanent magnets to the inside of the rotor. I put a digital hall effect sensor through the slots in the rotor's plastic cover. These should be cemented in place once they are verified to be functional, since it spins at a few thousand RPM. TODO: pictures.

Further work may include:
- A potentiometer to scale throttle range, adjusting difficulty of achieving 100% throttle
- A rotary-encoded turntable under the front wheel to sense steering position, for 2D games (like driving)
- A second hall sensor for pedaling cadence, which can be divided by wheel RPM to determine gear ratio
- A generator to add variable resistance (and power!), with credit to www.genesgreenmachine.com. This may make RPM measurement nicer by eliminating extra magnets and correlating generator output to HID throttle position.
- Finding a use for the nunchuck's accelerometer
- Logging of game-workout data to fitness platforms (Strava, Garmin, whatever). 

# External Dependencies
https://github.com/MHeironimus/ArduinoJoystickLibrary
https://github.com/GabrielBianconi/arduino-nunchuk
