/*  DS1631_test
  Basic code use the DS1631 as a thermostat
  Requires the DS1631 library
  
  Copyright Luke Miller December 30, 2012
  
  Released into the public domain. 
  
  DS1631 pinout:
        1      8
  SDA  --|----|--  Vdd
  SCL  --|    |--  A0
  Tout --|    |--  A1 
  GND  --|____|--  A2
        4      5
        
  Connect SDA to Arduino pin A4 (SDA)
  Connect SCL to Arduino pin A5 (SCL)

  /!\ To have a standalone thermostat, use DS1631A chip instead of a DS1631. (The final A of the chip name is important)
  A DS1631A chip starts in continuous mode by it self.
*/

#include <Wire.h>
#include <DS1631.h>


DS1631 Temp1(0);  // initialize DS1631 object, with bus address
// A bus address of 0 means pins A2,A1,A0 are all tied to 
// ground.

#define LEDpin 13 // Arduino pin with LED on it.
boolean LEDflag = false;

// Setup Loop
void setup(){
    
  // Setup Serial connection
  Serial.begin(9600); 
  Serial.println("DS1631 test: Temp. sensor"); 
  
  Wire.begin();             // join I2C bus

  //Setup the thermostat
  Temp1.setActiveHigh(true);
  Temp1.setOneShotMode(false);
  Temp1.setResolution(12);

  // Setup the TH threshold and TL threshold
  // Once the temperature reaches TH, the Tout pin will go high
  // Once the temperature is under the TL threshold, the Tout pin will go low
  Temp1.writeTH(35);
  Temp1.writeTL(30);

  Serial.print("TH : ");
  Serial.println(Temp1.readTH());
  Serial.print("TL : ");
  Serial.println(Temp1.readTL());
}

// Main Loop
void loop(){

}
