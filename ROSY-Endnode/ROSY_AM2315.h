#ifndef _AM2315_H_
#define _AM2315_H_

#include "Arduino.h"
#include "Wire.h"

#define AM2315_I2CADDR 0x5C
#define AM2315_READREG 0x03
class ROSY_AM2315
{
private:TwoWire* _i2c;
private:float humidity, temperature;
private:uint32_t lastreading;

public:ROSY_AM2315(TwoWire* theI2C = &Wire);
public:bool begin(void);
public:bool readData(void);
public:float getTemperature(void);
public:float getHumidity(void);
};
#endif
