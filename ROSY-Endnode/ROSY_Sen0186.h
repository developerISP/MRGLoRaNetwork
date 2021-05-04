
#ifndef _SEN0186_H_
#define _SEN0186_H_

#define __BUFFER_SIZE__ 35
#include "Arduino.h"

class ROSY_Sen0186
{
    private: bool isRead;
    public: bool getIsRead();
    private: char databuffer[__BUFFER_SIZE__];
    public: bool begin();
    public: bool read();
    private: int transCharToInt(char*,int ,int );  
    public: float WindDirection();
    public: float WindSpeedAverage();
    public: float WindSpeedMax();
    public: float Temperature();
    public: float RainfallOneHour();
    public: float RainfallOneDay();
    public: float Humidity();
    public: float BarPressure();
};
#endif

