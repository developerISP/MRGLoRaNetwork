
#include "ROSY_BME280.h"
#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>

ROSY_BME280::ROSY_BME280() {};
ROSY_BME280::~ROSY_BME280() {};

bool ROSY_BME280::begin(uint8_t addr, TwoWire* theWire)
{
	this->_i2caddr = addr;
	this->_wire = theWire;
	return init();
}

bool ROSY_BME280::init()
{
	this->_wire->begin();
	if (!read8(BME280_REGISTER_CHIPID, (uint8_t*)&this->_sensorID)) return false;
	if (_sensorID != 0x60) return false;
	if (!write8(BME280_REGISTER_SOFTRESET, 0xB6)) return false;
	_delay_ms(10);
	while (isReadingCalibration()) _delay_ms(10);
	if (!readCoefficients()) return false;
	if (!setSampling()) return false;
	_delay_ms(100);
	return true;
}

bool ROSY_BME280::setSampling(sensor_mode mode, sensor_sampling tempSampling, sensor_sampling pressSampling, sensor_sampling humSampling, sensor_filter filter, standby_duration duration)
{
	_measReg.mode = mode;
	_measReg.osrs_t = tempSampling;
	_measReg.osrs_p = pressSampling;
	_humReg.osrs_h = humSampling;
	_configReg.filter = filter;
	_configReg.t_sb = duration;
	if (!write8(BME280_REGISTER_CONTROL, MODE_SLEEP)) return false;
	if (!write8(BME280_REGISTER_CONTROLHUMID, _humReg.get())) return false;
	if (!write8(BME280_REGISTER_CONFIG, _configReg.get())) return false;
	if (!write8(BME280_REGISTER_CONTROL, _measReg.get())) return false;
	return true;
}

bool ROSY_BME280::write8(byte reg, byte value)
{
	this->_wire->clearWriteError();
	this->_wire->beginTransmission((uint8_t)_i2caddr);
	this->_wire->write((uint8_t)reg);
	this->_wire->write((uint8_t)value);
	if (this->_wire->getWriteError()) return false;
	if (this->_wire->endTransmission()) return false;
	return true;
}

bool ROSY_BME280::read8(byte reg, uint8_t* value = NULL)
{
	this->_wire->clearWriteError();
	this->_wire->beginTransmission((uint8_t)_i2caddr);
	this->_wire->write((uint8_t)reg);
	if (this->_wire->getWriteError()) return false;
	if (this->_wire->endTransmission()) return false;
	if (_wire->requestFrom((uint8_t)_i2caddr, (byte)1) <= 0) return false;
	if (value != 0) *value = this->_wire->read();
	else this->_wire->read();
	return true;
}

bool ROSY_BME280::read16(byte reg, uint16_t* value = NULL)
{
	this->_wire->clearWriteError();
	_wire->beginTransmission((uint8_t)_i2caddr);
	_wire->write((uint8_t)reg);
	if (this->_wire->getWriteError()) return false;
	if (this->_wire->endTransmission()) return false;
	if (this->_wire->requestFrom((uint8_t)_i2caddr, (byte)2) <= 0) return false;
	if (value != 0) *value = (_wire->read() << 8) | _wire->read();
	else
	{
		this->_wire->read();
		this->_wire->read();
	}
	return true;
}

bool ROSY_BME280::read16_LE(byte reg, uint16_t* value = NULL)
{

	if (value != 0)
	{
		if (read16(reg, value))
		{
			*value = (*value >> 8) | (*value << 8);
			return true;
		}
		else return false;
	}
	else return false;
}

bool ROSY_BME280::readS16(byte reg, int16_t* value = NULL)
{
	if (value != 0)
	{
		if (read16(reg, value)) return true;
		else return false;
	}
	else return false;
}

bool ROSY_BME280::readS16_LE(byte reg, int16_t* value = NULL)
{
	if (value != 0)
	{
		if (read16_LE(reg, value)) return true;
		else return false;
	}
	else return false;
}

bool ROSY_BME280::read24(uint8_t reg, uint32_t* value = NULL)
{
	this->_wire->clearWriteError();
	this->_wire->beginTransmission((uint8_t)_i2caddr);
	this->_wire->write((uint8_t)reg);
	if (this->_wire->getWriteError()) return false;
	if (this->_wire->endTransmission()) return false;
	if (_wire->requestFrom((uint8_t)_i2caddr, (uint8_t)3) <= 0) return false;
	if (value != 0)
	{
		*value = _wire->read();
		*value <<= 8;
		*value |= _wire->read();
		*value <<= 8;
		*value |= _wire->read();
		return true;
	}
	else return false;
}

bool ROSY_BME280::takeForcedMeasurement() {
	if (_measReg.mode == MODE_FORCED)
	{
		if (!write8(BME280_REGISTER_CONTROL, _measReg.get())) return false;
		bool isEnd = false;
		unsigned long int start = millis();
		while (!isEnd)
		{
			if ((start > millis()) ? (millis() + (pow(2, 32) - 1) - start) : (millis() - start) > 2000) return false;
			uint8_t tmp;
			if (read8(BME280_REGISTER_STATUS, &tmp))
			{
				if (tmp & 0x08) isEnd = true;
				else isEnd = false;
			}
			else isEnd = false;
			_delay_ms(1);
		}
		return true;
	}
	else return false;
}

bool ROSY_BME280::readCoefficients(void)
{
	if (!read16_LE(BME280_REGISTER_DIG_T1, &_bme280_calib.dig_T1)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_T2, &_bme280_calib.dig_T2)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_T3, &_bme280_calib.dig_T3)) return false;
	if (!read16_LE(BME280_REGISTER_DIG_P1, &_bme280_calib.dig_P1)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_P2, &_bme280_calib.dig_P2)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_P3, &_bme280_calib.dig_P3)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_P4, &_bme280_calib.dig_P4)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_P5, &_bme280_calib.dig_P5)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_P6, &_bme280_calib.dig_P6)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_P7, &_bme280_calib.dig_P7)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_P8, &_bme280_calib.dig_P8)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_P9, &_bme280_calib.dig_P9)) return false;
	if (!read8(BME280_REGISTER_DIG_H1, &_bme280_calib.dig_H1)) return false;
	if (!readS16_LE(BME280_REGISTER_DIG_H2, &_bme280_calib.dig_H2)) return false;
	if (!read8(BME280_REGISTER_DIG_H3, &_bme280_calib.dig_H3)) return false;
	uint8_t Ltmp = 0;
	uint8_t Htmp = 0;
	if (!read8(BME280_REGISTER_DIG_H4, &Ltmp)) return false;
	if (!read8(BME280_REGISTER_DIG_H4 + 1, &Htmp)) return false;
	_bme280_calib.dig_H4 = (((int8_t)Ltmp) << 4) | (Htmp & 0xF);
	if (!read8(BME280_REGISTER_DIG_H5 + 1, &Ltmp)) return false;
	if (!read8(BME280_REGISTER_DIG_H5, &Htmp)) return false;
	_bme280_calib.dig_H5 = ((int8_t)Ltmp << 4) | (Htmp >> 4);
	if (!read8(BME280_REGISTER_DIG_H6, _bme280_calib.dig_H6)) return false;
	return true;
}

bool ROSY_BME280::isReadingCalibration(void) {
	uint8_t const rStatus;
	if (read8(BME280_REGISTER_STATUS, &rStatus)) return (rStatus & (1 << 0)) != 0;
	else return false;
}

bool ROSY_BME280::readTemperature(float* value = NULL) {
	int32_t var1, var2;

	int32_t adc_T = 0;
	if (!read24(BME280_REGISTER_TEMPDATA, (uint32_t*)&adc_T)) return false;
	if (adc_T == 0x800000) {
		if (value != 0) {
			*value = NAN;
		}
		return false;
	}
	adc_T >>= 4;

	var1 = ((((adc_T >> 3) - ((int32_t)_bme280_calib.dig_T1 << 1))) * ((int32_t)_bme280_calib.dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t)_bme280_calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)_bme280_calib.dig_T1))) >> 12) * ((int32_t)_bme280_calib.dig_T3)) >> 14;
	t_fine = var1 + var2;

	float T = (t_fine * 5 + 128) >> 8;
	if (value != 0) {
		*value = T / 100;
		Serial.println(*value);
	}
	return true;
}

bool ROSY_BME280::readPressure(float* value = NULL) {
	int64_t var1, var2, p;

	if (!readTemperature()) return false;

	int32_t adc_P = 0;
	if (!read24(BME280_REGISTER_PRESSUREDATA, (uint32_t*)&adc_P)) return false;
	if (adc_P == 0x800000) {
		if (value != 0) {
			*value = NAN;
		}
		return false;
	}
	adc_P >>= 4;

	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)_bme280_calib.dig_P6;
	var2 = var2 + ((var1 * (int64_t)_bme280_calib.dig_P5) << 17);
	var2 = var2 + (((int64_t)_bme280_calib.dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t)_bme280_calib.dig_P3) >> 8) +
		((var1 * (int64_t)_bme280_calib.dig_P2) << 12);
	var1 =
		(((((int64_t)1) << 47) + var1)) * ((int64_t)_bme280_calib.dig_P1) >> 33;

	if (var1 == 0) {
		return false;
	}
	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t)_bme280_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)_bme280_calib.dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)_bme280_calib.dig_P7) << 4);
	if (value != 0) {
		*value = (float)p / 256;
	}
	return true;
}

bool ROSY_BME280::readHumidity(float* value = NULL) {
	if (!readTemperature()) return false;

	int32_t adc_H = 0;
	if (!read16(BME280_REGISTER_HUMIDDATA, (uint16_t*)&adc_H)) return false;
	if (adc_H == 0x8000) {
		if (value != 0) {
			*value = NAN;
		}
		return false;
	}

	int32_t v_x1_u32r;

	v_x1_u32r = (t_fine - ((int32_t)76800));

	v_x1_u32r = (((((adc_H << 14) - (((int32_t)_bme280_calib.dig_H4) << 20) -
		(((int32_t)_bme280_calib.dig_H5) * v_x1_u32r)) +
		((int32_t)16384)) >>
		15) *
		(((((((v_x1_u32r * ((int32_t)_bme280_calib.dig_H6)) >> 10) *
			(((v_x1_u32r * ((int32_t)_bme280_calib.dig_H3)) >> 11) +
				((int32_t)32768))) >>
			10) +
			((int32_t)2097152)) *
			((int32_t)_bme280_calib.dig_H2) +
			8192) >>
			14));

	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
		((int32_t)_bme280_calib.dig_H1)) >>
		4));

	v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
	v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
	float h = (v_x1_u32r >> 12);

	if (value != 0) {
		*value = h / 1024.0;
	}

	return true;
}

bool ROSY_BME280::readAltitude(float seaLevel, float* value) 
{
	float atmospheric = 0;
	if (!readPressure(&atmospheric)) return false;
	atmospheric /= 100.0F;
	if (value != 0) {
		*value = 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
	}
	return true;
}

float ROSY_BME280::seaLevelForAltitude(float altitude, float atmospheric) 
{
	return atmospheric / pow(1.0 - (altitude / 44330.0), 5.255);
}


uint32_t ROSY_BME280::sensorID(void) { return _sensorID; }

Adafruit_Sensor* ROSY_BME280::getTemperatureSensor(void) 
{
	if (!temp_sensor) temp_sensor = new ROSY_BME280_Temp(this);
	return temp_sensor;
}

Adafruit_Sensor* ROSY_BME280::getPressureSensor(void) 
{
	if (!pressure_sensor) pressure_sensor = new ROSY_BME280_Pressure(this);
	return pressure_sensor;
}

Adafruit_Sensor* ROSY_BME280::getHumiditySensor(void) {
	if (!humidity_sensor) {
		humidity_sensor = new ROSY_BME280_Humidity(this);
	}
	return humidity_sensor;
}


void ROSY_BME280_Temp::getSensor(sensor_t* sensor) {
	memset(sensor, 0, sizeof(sensor_t));
	strncpy(sensor->name, "BME280", sizeof(sensor->name) - 1);
	sensor->name[sizeof(sensor->name) - 1] = 0;
	sensor->version = 1;
	sensor->sensor_id = _sensorID;
	sensor->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
	sensor->min_delay = 0;
	sensor->min_value = -40.0;
	sensor->max_value = +85.0;
	sensor->resolution = 0.01;
}

bool ROSY_BME280_Temp::getEvent(sensors_event_t* event) {
	memset(event, 0, sizeof(sensors_event_t));
	event->version = sizeof(sensors_event_t);
	event->sensor_id = _sensorID;
	event->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
	event->timestamp = millis();
	if (!_theBME280->readTemperature(&event->temperature)) return false;
	return true;
}


void ROSY_BME280_Pressure::getSensor(sensor_t* sensor) {
	memset(sensor, 0, sizeof(sensor_t));
	strncpy(sensor->name, "BME280", sizeof(sensor->name) - 1);
	sensor->name[sizeof(sensor->name) - 1] = 0;
	sensor->version = 1;
	sensor->sensor_id = _sensorID;
	sensor->type = SENSOR_TYPE_PRESSURE;
	sensor->min_delay = 0;
	sensor->min_value = 300.0;
	sensor->max_value = 1100.0;
	sensor->resolution = 0.012;
}


bool ROSY_BME280_Pressure::getEvent(sensors_event_t* event) {
	memset(event, 0, sizeof(sensors_event_t));
	event->version = sizeof(sensors_event_t);
	event->sensor_id = _sensorID;
	event->type = SENSOR_TYPE_PRESSURE;
	event->timestamp = millis();
	if (!_theBME280->readPressure(&event->pressure)) return false;
	event->pressure /= 100;
	return true;
}


void ROSY_BME280_Humidity::getSensor(sensor_t* sensor) {
	memset(sensor, 0, sizeof(sensor_t));
	strncpy(sensor->name, "BME280", sizeof(sensor->name) - 1);
	sensor->name[sizeof(sensor->name) - 1] = 0;
	sensor->version = 1;
	sensor->sensor_id = _sensorID;
	sensor->type = SENSOR_TYPE_RELATIVE_HUMIDITY;
	sensor->min_delay = 0;
	sensor->min_value = 0;
	sensor->max_value = 100;
	sensor->resolution = 3;
}

bool ROSY_BME280_Humidity::getEvent(sensors_event_t* event) {
	memset(event, 0, sizeof(sensors_event_t));
	event->version = sizeof(sensors_event_t);
	event->sensor_id = _sensorID;
	event->type = SENSOR_TYPE_RELATIVE_HUMIDITY;
	event->timestamp = millis();
	if (!_theBME280->readHumidity(&event->relative_humidity)) return false;
	return true;
}