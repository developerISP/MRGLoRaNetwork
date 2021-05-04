#ifndef __BME280_H__
#define __BME280_H__

#include "Arduino.h"

#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>

#define BME280_ADDRESS (0x77)         

#define BME280_ADDRESS_ALTERNATE (0x76) 
 
enum {
	BME280_REGISTER_DIG_T1 = 0x88,
	BME280_REGISTER_DIG_T2 = 0x8A,
	BME280_REGISTER_DIG_T3 = 0x8C,

	BME280_REGISTER_DIG_P1 = 0x8E,
	BME280_REGISTER_DIG_P2 = 0x90,
	BME280_REGISTER_DIG_P3 = 0x92,
	BME280_REGISTER_DIG_P4 = 0x94,
	BME280_REGISTER_DIG_P5 = 0x96,
	BME280_REGISTER_DIG_P6 = 0x98,
	BME280_REGISTER_DIG_P7 = 0x9A,
	BME280_REGISTER_DIG_P8 = 0x9C,
	BME280_REGISTER_DIG_P9 = 0x9E,

	BME280_REGISTER_DIG_H1 = 0xA1,
	BME280_REGISTER_DIG_H2 = 0xE1,
	BME280_REGISTER_DIG_H3 = 0xE3,
	BME280_REGISTER_DIG_H4 = 0xE4,
	BME280_REGISTER_DIG_H5 = 0xE5,
	BME280_REGISTER_DIG_H6 = 0xE7,

	BME280_REGISTER_CHIPID = 0xD0,
	BME280_REGISTER_VERSION = 0xD1,
	BME280_REGISTER_SOFTRESET = 0xE0,

	BME280_REGISTER_CAL26 = 0xE1,  

	BME280_REGISTER_CONTROLHUMID = 0xF2,
	BME280_REGISTER_STATUS = 0XF3,
	BME280_REGISTER_CONTROL = 0xF4,
	BME280_REGISTER_CONFIG = 0xF5,
	BME280_REGISTER_PRESSUREDATA = 0xF7,
	BME280_REGISTER_TEMPDATA = 0xFA,
	BME280_REGISTER_HUMIDDATA = 0xFD
};

typedef struct {
	uint16_t dig_T1; 
	int16_t dig_T2;  
	int16_t dig_T3; 

	uint16_t dig_P1;
	int16_t dig_P2; 
	int16_t dig_P3; 
	int16_t dig_P4; 
	int16_t dig_P5; 
	int16_t dig_P6; 
	int16_t dig_P7; 
	int16_t dig_P8; 
	int16_t dig_P9; 

	uint8_t dig_H1; 
	int16_t dig_H2; 
	uint8_t dig_H3; 
	int16_t dig_H4; 
	int16_t dig_H5; 
	int8_t dig_H6; 
} bme280_calib_data;

class ROSY_BME280;

class ROSY_BME280_Temp : public Adafruit_Sensor {
public:
	ROSY_BME280_Temp(ROSY_BME280* parent) { _theBME280 = parent; }
	bool getEvent(sensors_event_t*);
	void getSensor(sensor_t*);

private:
	int _sensorID = 280;
	ROSY_BME280* _theBME280 = NULL;
};

class ROSY_BME280_Pressure : public Adafruit_Sensor {
public:
	ROSY_BME280_Pressure(ROSY_BME280* parent) { _theBME280 = parent; }
	bool getEvent(sensors_event_t*);
	void getSensor(sensor_t*);

private:
	int _sensorID = 280;
	ROSY_BME280* _theBME280 = NULL;
};

class ROSY_BME280_Humidity : public Adafruit_Sensor {
public:
	ROSY_BME280_Humidity(ROSY_BME280* parent) { _theBME280 = parent; }
	bool getEvent(sensors_event_t*);
	void getSensor(sensor_t*);

private:
	int _sensorID = 280;
	ROSY_BME280* _theBME280 = NULL;
};

class ROSY_BME280 {
public:
	enum sensor_sampling {
		SAMPLING_NONE = 0b000,
		SAMPLING_X1 = 0b001,
		SAMPLING_X2 = 0b010,
		SAMPLING_X4 = 0b011,
		SAMPLING_X8 = 0b100,
		SAMPLING_X16 = 0b101
	};

	enum sensor_mode {
		MODE_SLEEP = 0b00,
		MODE_FORCED = 0b01,
		MODE_NORMAL = 0b11
	};

	enum sensor_filter {
		FILTER_OFF = 0b000,
		FILTER_X2 = 0b001,
		FILTER_X4 = 0b010,
		FILTER_X8 = 0b011,
		FILTER_X16 = 0b100
	};

	enum standby_duration {
		STANDBY_MS_0_5 = 0b000,
		STANDBY_MS_10 = 0b110,
		STANDBY_MS_20 = 0b111,
		STANDBY_MS_62_5 = 0b001,
		STANDBY_MS_125 = 0b010,
		STANDBY_MS_250 = 0b011,
		STANDBY_MS_500 = 0b100,
		STANDBY_MS_1000 = 0b101
	};

	ROSY_BME280();
	~ROSY_BME280(void);
	bool begin(uint8_t addr = BME280_ADDRESS, TwoWire* theWire = &Wire);
	bool init();

	bool setSampling(sensor_mode mode = MODE_NORMAL,
		sensor_sampling tempSampling = SAMPLING_X16,
		sensor_sampling pressSampling = SAMPLING_X16,
		sensor_sampling humSampling = SAMPLING_X16,
		sensor_filter filter = FILTER_OFF,
		standby_duration duration = STANDBY_MS_0_5);

	bool takeForcedMeasurement();
	bool readTemperature(float* value = NULL);
	bool readPressure(float* value = NULL);
	bool readHumidity(float* value = NULL);

	bool readAltitude(float seaLevel, float* value);
	float seaLevelForAltitude(float altitude, float pressure);
	uint32_t sensorID(void);

	Adafruit_Sensor* getTemperatureSensor(void);
	Adafruit_Sensor* getPressureSensor(void);
	Adafruit_Sensor* getHumiditySensor(void);

protected:
	TwoWire* _wire;

	ROSY_BME280_Temp* temp_sensor = NULL;

	ROSY_BME280_Pressure* pressure_sensor = NULL;

	ROSY_BME280_Humidity* humidity_sensor = NULL;

	bool readCoefficients(void);
	bool isReadingCalibration(void);
	uint8_t spixfer(uint8_t x);

	bool write8(byte reg, byte value);
	bool read8(byte reg, uint8_t* value);
	bool read16(byte reg, uint16_t* value);
	bool read24(byte reg, uint32_t* value);
	bool readS16(byte reg, int16_t* value);
	bool read16_LE(byte reg, uint16_t* value); 
	bool readS16_LE(byte reg, int16_t* value); 

	uint8_t _i2caddr;
	int32_t _sensorID;
	int32_t t_fine;
					
	bme280_calib_data _bme280_calib;

	struct config {
		unsigned int t_sb : 3;
		unsigned int filter : 3;

		unsigned int none : 1; 
		unsigned int spi3w_en : 1;

		unsigned int get() { return (t_sb << 5) | (filter << 2) | spi3w_en; }
	};
	config _configReg;

	struct ctrl_meas {
		unsigned int osrs_t : 3;

		unsigned int osrs_p : 3; 
		unsigned int mode : 2;

		unsigned int get() { return (osrs_t << 5) | (osrs_p << 2) | mode; }
	};
	ctrl_meas _measReg;

	struct ctrl_hum {
		unsigned int none : 5;

		unsigned int osrs_h : 3;

		unsigned int get() { return (osrs_h); }
	};
	ctrl_hum _humReg;
};

#endif
