# bike

This project needs a better name.

I have a bike trainer. It was cheap on Amazon. http://a.co/gge61I9

This project monitors RPM using a hall sensor. 

It sends that RPM as a HID Joystick's throttle input, using the ArduinoJoystick library by Matthew Heironimus. You can fly a flight sim (fun), play a driving game (hard), or even map it to an FPS (Untested). It works with linux, mac, windows, consoles that support USB HID Joysticks, and mobile devices that support both HID Joysticks and USB-OTG connections.

It also includes a wii nunchuck library by Gabriel Bianconi. The nunchuck is a great form factor for controlling a game while riding, especially with aero bars.

To set up my specific model of trainer, I added two small permanent magnets to the inside of the rotor. I put a digital hall effect sensor through the slots in the rotor's plastic cover. These should be cemented in place once they are verified to be functional, since it spins very quickly. TODO: pictures.

Further work may include:
- A potentiometer to scale throttle range, affecting gameplay difficulty
- A turntable under the front wheel to sense steering position
- A second hall sensor for pedaling cadence, which can be divided by wheel RPM to determine gear ratio.
- A small generator to vary difficulty, with credit to www.genesgreenmachine.com
- Finding a use for the nunchuck's accelerometer

https://github.com/MHeironimus/ArduinoJoystickLibrary
https://github.com/GabrielBianconi/arduino-nunchuk
