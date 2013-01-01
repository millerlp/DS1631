//
//  DS1631.h
//  
//
//  Created by Luke Miller on 12/30/12.
//  Released into the public domain.
//  http://github.com/millerlp/DS1631

#ifndef DS1631_h
#define DS1631_h


#include <Arduino.h>
#include <inttypes.h>

class DS1631
{
public:
    DS1631(int); // Constructor, supply with I2C address (0-7)
    void stopConversion();
    void startConversion();
    void writeConfig(uint8_t);
    uint8_t readConfig();
    float readTempF();  // Floating point (i.e. 24.125)
    int32_t readTempD();  // Long * 1/16 = °C
    bool conversionDone();
    float readTempOneShot(); // Read in 1-shot mode, return float
    
    uint16_t readTempOneShotInt(); // Return 16-bit int
    // The 16-bit integer returned by readTempOneShotInt()
    // can be split into its high byte and low byte for
    // conversion to a temperature. The low byte simply
    // needs to be multiplied by 0.0625 to get the 
    // fractional component of the temperature. The
    // high byte can be directly converted to the 
    // whole-number part of the temperature if it is
    // less than 0x80 (128 in base-10). If it is greater
    // than 128, the temperature was negative. If it is
    // a negative temperature, subtract
    // 256 from the value, the result of which is the
    // whole part of the temperature. You must then add
    // the fractional part to this negative value to get
    // the true temperature value, as follows:
    // (float)(highByte - 256) + (lowByte * 0.0625)
    
    
private:
    void readT(); 
    int _address;
    uint8_t MSByte;
    uint8_t LSByte;
};


#endif
