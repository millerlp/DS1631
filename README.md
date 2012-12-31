DS1631 library for Arduino

Save the contents of this repository into a folder
named DS1631 inside your Arduino/libraries directory.
If you don't have a libraries folder inside your 
Arduino sketchbook folder, go ahead and make one. 

This library contains function to talk to and read from
a DS1631 temperature sensor over I2C (Two-Wire) serial
interface.

The DS1631 from Maxim Integrated  is a digital thermometer 
with up to 12-bit resolution (0.0625 degrees C) that 
can sample continuously or in one-shot mode, with a 
low-power idle state enabled between readings in one-shot 
mode. It can read from -55 to +125 degrees Celsius.

This library was written under Arduino 1.0.2 and 
tested on a Duemilanove (ATmega 328P) clone.

Originally found at
http://github.com/millerlp/DS1631
