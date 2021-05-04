#include "ROSY_i2cScanner.h"
#include <Wire.h>

int ROSY_i2cScanner::scanner()
{
int nDevices = 0;
   int error = 0;
    for (int address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");
            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0) Serial.println("No I2C devices found\n");
    else Serial.println("done\n");
    return nDevices;
}
