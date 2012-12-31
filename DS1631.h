//
//  DS1631.h
//  
//
//  Created by Luke Miller on 12/30/12.
//  
//

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
    float readTempOneShot();
    
private:
    void readT(); 
    int _address;
    uint8_t highByte;
    uint8_t lowByte;
};


#endif
