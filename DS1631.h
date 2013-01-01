//
//  DS1631.h
//  Functions to write and read to the DS1631 
//  temperature sensor over I2C (two-wire) 
//  interface. Requires the Wire library.
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
    void stopConversion(); // Enter low-power idle mode
    void startConversion(); // Start new temperature reading
    void writeConfig(uint8_t);  // Write configuration registers
    uint8_t readConfig();   // Read configuration registers
    float readTempF();  // Return floating point temperature 
    int32_t readTempD();  // Return temp as double (Long * 1/16 = °C)
    bool conversionDone();  // Check if new temperature read is done
    float readTempOneShot(); // Read in 1-shot mode, return float
    
    uint16_t readTempOneShotInt(); // Read 1-shot, return 16-bit integer
    // The 16-bit integer returned by readTempOneShotInt()
    // can be split into its high byte and low byte for
    // conversion to a temperature. The low byte should
    // be bit-shifted right by 4 spots, and then
    // needs to be multiplied by 0.0625 to get the 
    // fractional component of the temperature. The
    // high byte can be directly converted to the 
    // whole-number part of the temperature if it is
    // less than 0x80 (128 in base-10). 
    // (float)(highByte) + ((lowByte>>4) * 0.0625)
    // If the high byte is greater than 128 (0x80),
    // the temperature was negative.
    // For a negative temperature, subtract
    // 256 from the high byte, the result of which is the
    // whole part of the temperature. You must then add
    // the fractional part to this negative value to get
    // the true temperature value, as follows:
    // (float)(highByte - 256) + ((lowByte>>4) * 0.0625)
    // From the DS1631 data sheet, the following values
    // should be returned for the given temperatures:
    // Temperature      Binary Output       Hex Output
    //  +125        0111 1101 0000 0000         0x7D00
    //  +25.0625    0001 1001 0001 0000         0x1910
    //  +10.125     0000 1010 0010 0000         0x0A20
    //  +0.5        0000 0000 1000 0000         0x0080
    //  -0.5        1111 1111 1000 0000         0xFF80
    //  -10.125     1111 0101 1110 0000         0xF5E0
    //  -25.0625    1110 0110 1111 0000         0xE6F0
    //  -55         1100 1001 0000 0000         0xC900
    
    
private:
    void readT(); 
    int _address;
    uint8_t MSByte;
    uint8_t LSByte;
};


#endif
