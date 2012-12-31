/*  DS1631_test
  Basic code to get temperature readings over I2C
  from a DS1631 temperature sensor
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
*/

#include <Wire.h>
#include <DS1631.h>


DS1631 Temp1(0);  // initialize DS1631 object, with bus address
// A bus address of 0 means pins A2,A1,A0 are all tied to 
// ground.

#define LEDpin 13 // Arduino pin with LED on it.
boolean LEDflag = false;

// setup loop
void setup(){
    
  // Setup Serial connection
  Serial.begin(9600); 
  Serial.println("DS1631 test: Temp. sensor"); 
  
  Wire.begin();             // join I2C bus
  
  // Read the current configuration registers 
  // off of the DS1631
  int config = Temp1.readConfig();
  Serial.print("Config settings before: ");
  Serial.println(config, BIN);
  
  // Write to configuration registers of DS1631
  Temp1.writeConfig(13);  // Set to 12-bit, 1-shot mode

  // Read the new configuration registers off
  // of the DS1631
  config = Temp1.readConfig();
  Serial.print("Config settings after: ");
  Serial.println(config, BIN);
  
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, LEDflag = !LEDflag);

  
}

// Main Loop
void loop(){
  
  // Take a temperature reading. In 1-shot mode, a
  // reading may take up to 750ms, depending on the
  // resolution you set. The readTempOneShot() 
  // function integrates a wait cycle until the
  // new reading is done. 
  float temp = Temp1.readTempOneShot(); 
//  Temp1.startConversion();
//  float temp = Temp1.readTempF();
//  Temp1.stopConversion();
  
  Serial.print("Temperature: ");
  Serial.print(temp, 4);
  Serial.println(" degC");
  digitalWrite(LEDpin, LEDflag = !LEDflag);
  // Wait 1s before restart
//  delay(1000);
}

