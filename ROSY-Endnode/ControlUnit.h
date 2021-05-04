#ifndef _CONTROLUNIT_H_
#define _CONTROLUNIT_H_

#include "ROSY_EEPROM.h"
#include "ROSY_LoRa.h"
#include "Compressor.h"
#include "Statistics.h"
#include "ROSY_BME280.h"
#include "ROSY_OPCN3.h"
#include "ROSY_AM2315.h"
#include "ROSY_ADS1115.h" 
#include "Adafruit_LSM9DS1.h"
#include "RTClib.h"
#include "ROSY_Sen0186.h"
#include "Adafruit_INA219.h"
#include "ControlUnitConfig.h"
#include "SendDecompressedPacket.h"
#include "ROSY_i2cScanner.h"

#define NUMBER_OF_READ_FOR_STEP 1

class ControlUnit
{
#if ENABLE_BME280
private: ROSY_BME280 BME280 = ROSY_BME280();
private: bool enableBME280;
private: bool statusBME280;
#endif
#if ENABLE_AM2315
private: ROSY_AM2315 AM2315 = ROSY_AM2315();
private: bool enableAM2315;
private: bool statusAM2315;
#endif
#if ENABLE_OPCN3
private: OPCN3 OPC = OPCN3(53, 500000);
private: HistogramData OPChist;
private: bool enableOPC;
private: bool statusOPC;
#endif
#if ENABLE_SENSOR_1 || ENABLE_SENSOR_2
private: ROSY_ADS1115 ADS1115_1 = ROSY_ADS1115(0x48);
private: bool enableADS1115_1;
private: bool statusADS1115_1;
#endif
#if ENABLE_SENSOR_3 || ENABLE_SENSOR_4
private: ROSY_ADS1115 ADS1115_2 = ROSY_ADS1115(0x49);
private: bool enableADS1115_2;
private: bool statusADS1115_2;
#endif
#if ENABLE_SENSOR_5 || ENABLE_SENSOR_6
private: ROSY_ADS1115 ADS1115_3 = ROSY_ADS1115(0x50);
private: bool enableADS1115_3;
private: bool statusADS1115_3;
#endif

#if ENABLE_LSM9DS0
private: Adafruit_LSM9DS1 LSM9DS0 = Adafruit_LSM9DS1();
private: sensors_event_t accel;
private: sensors_event_t mag;
private: sensors_event_t gyro;
private: sensors_event_t temp;
private: bool enableLSM9DS0;
private: bool statusLSM9DS0;
#endif
#if ENABLE_DS3231
private: RTC_DS3231 DS3231 = RTC_DS3231();
private: bool enableDS3231;
private: bool statusDS3231;
#endif
#if ENABLE_INA219
private: Adafruit_INA219 INA219 = Adafruit_INA219();
private: bool enableINA219;
private: bool statusINA219;
#endif
#if ENABLE_SEN0186 
private: ROSY_Sen0186 Sen0186 = ROSY_Sen0186(); 
private: bool enableSen0186;
private: bool statusSen0186;
#endif

private: bool(*update)();
private: void(*logger)(uint8_t);

public: DateTime ntime;

public:void ControlUnitInit(bool(*updateCB)(), void(*loggerCallback)(uint8_t));
private: void INA219Init();
public: float getBatteryCurrent();
public: float getBatteryVoltage();
public: bool getINA219status();

private: void BME280Init();
public: float BME280ReadTemperature();
public: float BME280ReadHumidity();
public: float BME280ReadPressure();
public: bool getBME280status();

private: void AM2315Init();
public: void AM2315Read();
public: float AM2315GetTemperature();
public: float AM2315GetHumidity();
public: bool getAM2315status();

private: void OPCN3Init();
public: void  OPCN3SetHistogramData();
public: float OPCN3PM1();
public: float OPCN3PM2_5();
public: float OPCN3PM10();
public: bool getOPCN3status();

private: void initADS1115_1();
public: bool getADS1115_1status();
private: void initADS1115_2();
public: bool getADS1115_2status();
private: void initADS1115_3();
public: bool getADS1115_3status();

public: float readSensor1_1();
public: float readSensor1_2();
public: float readSensor1_3();
public: float readSensor1_4();
public: float readSensor2_1();
public: float readSensor2_2();
public: float readSensor2_3();
public: float readSensor2_4();
public: float readSensor3_1();
public: float readSensor3_2();
public: float readSensor3_3();
public: float readSensor3_4();

private: void initLSM9DS0();
public: void LSM9DS0Read();
public: float LSM9DS0GetAccelX();
public: float LSM9DS0GetAccelY();
public: float LSM9DS0GetAccelZ();
public: float LSM9DS0GetMagX();
public: float LSM9DS0GetMagY();
public: float LSM9DS0GetMagZ();
public: float LSM9DS0GetGyroX();
public: float LSM9DS0GetGyroY();
public: float LSM9DS0GetGyroZ();
public: bool getLSM9DS0status();

private: void initDS3231();
public: DateTime DS3231getDateTime();
public: void DS3231setDateTime(uint32_t datetime);
public: bool getDS3231status();

public: void initSen0186(); 
public: bool getSen0186status();
public: void Sen0186Read(); 
public: float Sen0186WindDirection();
public: float Sen0186WindSpeedAverage();
public: float Sen0186Temperature();
public: float Sen0186RainfallOneHour();
public: float Sen0186Humidity();
public: float Sen0186BarPressure();

public: void readSensor();

};
#endif
