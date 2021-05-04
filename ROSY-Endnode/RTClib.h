#ifndef _RTCLIB_H_
#define _RTCLIB_H_

#include <Arduino.h>
class TimeSpan;

#define PCF8523_ADDRESS 0x68      
#define PCF8523_CLKOUTCONTROL 0x0F 
#define PCF8523_CONTROL_1 0x00     
#define PCF8523_CONTROL_2 0x01     
#define PCF8523_CONTROL_3 0x02     
#define PCF8523_TIMER_B_FRCTL 0x12 
#define PCF8523_TIMER_B_VALUE 0x13 
#define PCF8523_OFFSET 0x0E       
#define PCF8523_STATUSREG 0x03     

#define DS1307_ADDRESS 0x68 
#define DS1307_CONTROL 0x07 
#define DS1307_NVRAM 0x08   

#define DS3231_ADDRESS 0x68   
#define DS3231_TIME 0x00      
#define DS3231_ALARM1 0x07   
#define DS3231_ALARM2 0x0B   
#define DS3231_CONTROL 0x0E   
#define DS3231_STATUSREG 0x0F 
#define DS3231_TEMPERATUREREG  0x11
#define SECONDS_PER_DAY 86400L 
#define SECONDS_FROM_1970_TO_2000 946684800

class DateTime {
public:
  DateTime(uint32_t t = SECONDS_FROM_1970_TO_2000);
  DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0,
           uint8_t min = 0, uint8_t sec = 0);
  DateTime(const DateTime &copy);
  DateTime(const char *date, const char *time);
  DateTime(const __FlashStringHelper *date, const __FlashStringHelper *time);
  bool isValid() const;
  char *toString(char *buffer);

  uint16_t year() const { return 2000 + yOff; }

  uint8_t month() const { return m; }

  uint8_t day() const { return d; }

  uint8_t hour() const { return hh; }

  uint8_t twelveHour() const;

  uint8_t isPM() const { return hh >= 12; }

  uint8_t minute() const { return mm; }

  uint8_t second() const { return ss; }

  uint8_t dayOfTheWeek() const;

  uint32_t secondstime() const;

  uint32_t unixtime(void) const;

  enum timestampOpt {
    TIMESTAMP_FULL,
    TIMESTAMP_TIME, 
    TIMESTAMP_DATE  
  };
  String timestamp(timestampOpt opt = TIMESTAMP_FULL);

  DateTime operator+(const TimeSpan &span);
  DateTime operator-(const TimeSpan &span);
  TimeSpan operator-(const DateTime &right);
  bool operator<(const DateTime &right) const;
  bool operator>(const DateTime &right) const { return right < *this; }
  bool operator<=(const DateTime &right) const { return !(*this > right); }

  bool operator>=(const DateTime &right) const { return !(*this < right); }
  bool operator==(const DateTime &right) const;
  bool operator!=(const DateTime &right) const { return !(*this == right); }

protected:
  uint8_t yOff; 
  uint8_t m;    
  uint8_t d;    
  uint8_t hh;   
  uint8_t mm;   
  uint8_t ss;  
};

class TimeSpan {
public:
  TimeSpan(int32_t seconds = 0);
  TimeSpan(int16_t days, int8_t hours, int8_t minutes, int8_t seconds);
  TimeSpan(const TimeSpan &copy);

  int16_t days() const { return _seconds / 86400L; }

  int8_t hours() const { return _seconds / 3600 % 24; }

  int8_t minutes() const { return _seconds / 60 % 60; }

  int8_t seconds() const { return _seconds % 60; }
  int32_t totalseconds() const { return _seconds; }

  TimeSpan operator+(const TimeSpan &right);
  TimeSpan operator-(const TimeSpan &right);

protected:
  int32_t _seconds;
};

enum Ds1307SqwPinMode {
  DS1307_OFF = 0x00,           
  DS1307_ON = 0x80,             
  DS1307_SquareWave1HZ = 0x10,  
  DS1307_SquareWave4kHz = 0x11, 
  DS1307_SquareWave8kHz = 0x12, 
  DS1307_SquareWave32kHz = 0x13 
};

class RTC_DS1307 {
public:
  boolean begin(void);
  static void adjust(const DateTime &dt);
  uint8_t isrunning(void);
  static DateTime now();
  static Ds1307SqwPinMode readSqwPinMode();
  static void writeSqwPinMode(Ds1307SqwPinMode mode);
  uint8_t readnvram(uint8_t address);
  void readnvram(uint8_t *buf, uint8_t size, uint8_t address);
  void writenvram(uint8_t address, uint8_t data);
  void writenvram(uint8_t address, uint8_t *buf, uint8_t size);
};

enum Ds3231SqwPinMode {
  DS3231_OFF = 0x01,           
  DS3231_SquareWave1Hz = 0x00,  
  DS3231_SquareWave1kHz = 0x08, 
  DS3231_SquareWave4kHz = 0x10, 
  DS3231_SquareWave8kHz = 0x18  
};

enum Ds3231Alarm1Mode {
  DS3231_A1_PerSecond = 0x0F,
  DS3231_A1_Second = 0x0E,
  DS3231_A1_Minute = 0x0C,
  DS3231_A1_Hour = 0x08,
  DS3231_A1_Date = 0x00,
  DS3231_A1_Day = 0x10
};

enum Ds3231Alarm2Mode {
  DS3231_A2_PerMinute = 0x7,
  DS3231_A2_Minute = 0x6,
  DS3231_A2_Hour = 0x4,
  DS3231_A2_Date = 0x0,
  DS3231_A2_Day = 0x8
};

class RTC_DS3231 {
public:
  boolean begin(void);
  static void adjust(const DateTime &dt);
  bool lostPower(void);
  static DateTime now();
  static Ds3231SqwPinMode readSqwPinMode();
  static void writeSqwPinMode(Ds3231SqwPinMode mode);
  bool setAlarm1(const DateTime &dt, Ds3231Alarm1Mode alarm_mode);
  bool setAlarm2(const DateTime &dt, Ds3231Alarm2Mode alarm_mode);
  void disableAlarm(uint8_t alarm_num);
  void clearAlarm(uint8_t alarm_num);
  bool alarmFired(uint8_t alarm_num);
  static float getTemperature(); 
};

enum Pcf8523SqwPinMode {
  PCF8523_OFF = 7,             
  PCF8523_SquareWave1HZ = 6,  
  PCF8523_SquareWave32HZ = 5,  
  PCF8523_SquareWave1kHz = 4,  
  PCF8523_SquareWave4kHz = 3,  
  PCF8523_SquareWave8kHz = 2,  
  PCF8523_SquareWave16kHz = 1, 
  PCF8523_SquareWave32kHz = 0  
};

enum PCF8523TimerClockFreq {
  PCF8523_Frequency4kHz = 0,
  PCF8523_Frequency64Hz = 1,
  PCF8523_FrequencySecond = 2,
  PCF8523_FrequencyMinute = 3,
  PCF8523_FrequencyHour = 4,  
};

enum PCF8523TimerIntPulse {
  PCF8523_LowPulse3x64Hz = 0,  
  PCF8523_LowPulse4x64Hz = 1,  
  PCF8523_LowPulse5x64Hz = 2,  
  PCF8523_LowPulse6x64Hz = 3,  
  PCF8523_LowPulse8x64Hz = 4,  
  PCF8523_LowPulse10x64Hz = 5, 
  PCF8523_LowPulse12x64Hz = 6, 
  PCF8523_LowPulse14x64Hz = 7  
};

enum Pcf8523OffsetMode {
  PCF8523_TwoHours = 0x00, 
  PCF8523_OneMinute = 0x80 
};

class RTC_PCF8523 {
public:
  boolean begin(void);
  void adjust(const DateTime &dt);
  boolean lostPower(void);
  boolean initialized(void);
  static DateTime now();
  Pcf8523SqwPinMode readSqwPinMode();
  void writeSqwPinMode(Pcf8523SqwPinMode mode);
  void enableSecondTimer(void);
  void disableSecondTimer(void);
  void enableCountdownTimer(PCF8523TimerClockFreq clkFreq, uint8_t numPeriods,
                            uint8_t lowPulseWidth);
  void enableCountdownTimer(PCF8523TimerClockFreq clkFreq, uint8_t numPeriods);
  void disableCountdownTimer(void);
  void deconfigureAllTimers(void);
  void calibrate(Pcf8523OffsetMode mode, int8_t offset);
};

class RTC_Millis {
public:
  static void begin(const DateTime &dt) { adjust(dt); }
  static void adjust(const DateTime &dt);
  static DateTime now();

protected:
  static uint32_t lastUnix;  
  static uint32_t lastMillis; 
};

class RTC_Micros {
public:
  static void begin(const DateTime &dt) { adjust(dt); }
  static void adjust(const DateTime &dt);
  static void adjustDrift(int ppm);
  static DateTime now();

protected:
  static uint32_t microsPerSecond; 
  static uint32_t lastUnix;   
  static uint32_t lastMicros; 
};

#endif
