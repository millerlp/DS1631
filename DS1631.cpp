//
//  DS1631.cpp
//  Functions to write and read to the DS1631 
//  temperature sensor over I2C (two-wire) 
//  interface. Requires the Wire library. 
//
//  Created by Luke Miller on 12/30/12.
//  
//
//  Released into the public domain.
//  http://github.com/millerlp/DS1631

#include <Wire.h>
#include <DS1631.h>

// Initialize Class Variables //////////////////////////////////////////////////
int _address;
uint8_t MSByte;
uint8_t LSByte;

// The constructor. Supply the address for the
// DS1631. The base address is 1001 A2 A1 A0
// Available options should be 0 through 7, with
// 0 being the standard option when A2, A1, and A0 are
// tied to ground.
// Call with something like:
// DS1631 TempValue(0); // address 0 = A2,A1,A0 tied to ground

DS1631::DS1631(int Addr){
    _address=Addr + 72;  // Ground Addr 72 for DS1631  
}

//*****************************************************
// Public Methods 

// Tell the DS1631 to stop making temperature readings.
void DS1631::stopConversion(){
    Wire.beginTransmission(_address);
    Wire.write(0x22); // Stop conversion
    Wire.endTransmission();
}

// Tell the DS1631 to begin making temperature readings
// If 1SHOT = True, only one temperature reading is taken.
// If 1SHOT = False, the DS1631 is in continuous mode and
// will keep making temperature readings until told to 
// stop.
void DS1631::startConversion(){
    Wire.beginTransmission(_address);
    Wire.write(0x51); // Start conversion
    Wire.endTransmission();
    
}

// Write to the configuration registers
// You'll primarily use this to set the 
// conversion resolution (which affects 
// the time taken for a reading) and whether
// the DS1631 takes continuous readings or
// goes into a low-power idle state after
// taking 1 reading (1-shot mode).
// 13   = 12 bit, 1-shot mode
// 9    = 11 bit, 1-shot mode
// 5    = 10 bit, 1-shot mode
// 1    = 9  bit, 1-shot mode

// 12   = 12 bit, continuous mode
// 8    = 11 bit, continuous mode
// 4    = 10 bit, continuous mode
// 0    = 9  bit, continuous mode
void DS1631::writeConfig(uint8_t _data){
    stopConversion(); 
    Wire.beginTransmission(_address);
    Wire.write(0xAC);        // AC : Access Config
    Wire.write(_data);
    Wire.endTransmission();
    startConversion();
}

// Read the configuration registers
uint8_t DS1631::readConfig(){
    uint8_t _data;
    Wire.beginTransmission(_address);
    Wire.write(0xAC);        // AC : Access Config
    Wire.endTransmission();
    Wire.requestFrom(_address, 1);
    if(Wire.available()) {
        _data = Wire.read();
    }
    return _data;
}

// Request a temperature reading from the DS1631
// The high and low bytes are saved
void DS1631::readT(){
    Wire.beginTransmission(_address);
    Wire.write(0xAA); // AA : Request Temperature
    Wire.endTransmission();
    Wire.requestFrom(_address,2); // READ 2 bytes
    Wire.available(); // 1st byte
    MSByte = Wire.read(); // read a byte
    Wire.available(); // 2nd byte
    LSByte = Wire.read(); // read a byte
}

// Read the temperature and return a floating point
// temperature value, in degrees Celsius
float DS1631::readTempF(){
    int T_dec;
    double T;
    readT();
    // T° processing. Shift the LSByte right 4 positions
    // The resulting binary value, converted to base-10 
    // and multiplied by 0.0625, is the decimal part of 
    // the temperature. 
    LSByte = LSByte>>4;
    
    // The MSByte (8 bits), converted to base-10, 
    // represents the whole number portion of the 
    // temperature. When the left-most bit is 1,
    // this represents the special case of a 
    // negative temperature value, so you must 
    // subtract off 256 to get the whole number
    // value.
    if(MSByte>=0x80){ //if sign bit is set
        MSByte = MSByte - 256;
    }
    // Combine the whole number and fractional
    // parts of the temperature
    T = (float) MSByte + (float) LSByte*0.0625;
    return T;
}

// Read the temperature in 1-shot mode, with a 
// wait built in so that the temperature
// has time to update. This isn't necessary when
// using the continuous measuring mode.
float DS1631::readTempOneShot(){
    long lastMillis = millis();
    float T;
    // Send command to start taking temperature reading
    startConversion();
    // Now wait for the configuration register's 
    // most significant bit to be returned as 1,
    // indicating that the reading is done. A 12-bit
    // reading can take up to 750 milliseconds
    while ( !conversionDone() ) {
        // Wait a little while before checking
        // the configuration register again
        while (millis() - lastMillis < 50) {};
    }
    // Once the temperature conversion is done,
    // read the value from the DS1631
    T = readTempF();
    // After reading the temperature, put the
    // DS1631 back into low-power idle state.
    stopConversion();
    return T;
}

// Read the temperature in 1-shot mode and 
// return an integer composed of the MSByte
// and LSByte returned from the DS1631, stored
// in two's complement format. The integer
// would be the most compact way of storing the 
// temperature data, since it can be stored in
// two bytes.
uint16_t DS1631::readTempOneShotInt(){
    long lastMillis = millis();
    uint16_t T;
    startConversion();
    while ( !conversionDone() ){
        while (millis() - lastMillis < 50) {};
    }
    readT(); // Get MSByte and LSByte

    T = word(MSByte,LSByte);
    return T;
    // If you take the integer and split it
    // back into its highByte and lowByte using
    // the highByte() and lowByte() functions of
    // Arduino, you can quickly calculate the
    // temperature using the rules outlined above
    // in the readTempF() function.
}

// Read the temperature and return a double value
// If you divide the returned double value by 16, 
// you get the temperature value in °C
int32_t DS1631::readTempD(){ // 1/16°C = 12 Bit accuracy 0.0625°C
    int T_dec;
    int32_t T; 
    readT();
    
    T=((int32_t)MSByte << 8) + LSByte;
    // T° processing
    if(T >= 0x8000){   // If sign bit is set, then temp is negative
        T = T - 0xFFFF;
    }
    T = T >>4;
    return T;
}

// Check if the temperature reading (Conversion) is 
// finished. 12-bit readings take up to 750ms.
bool DS1631::conversionDone(){  // if Conversion Done = Boolean = True
    uint8_t _data = readConfig();
    // This OR's the _data value with 127 (b01111111)
    // If the most significant bit is 1, the result 
    // is 255 (b11111111)
    if ((_data | 127)==255)
        return true;
    else 
        return false;
}