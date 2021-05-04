#include "ROSY_AM2315.h"

ROSY_AM2315::ROSY_AM2315(TwoWire* theI2C) : _i2c(theI2C)
{
	this->lastreading = 0;
	this->temperature = NAN;
	this->humidity = NAN;
}

bool ROSY_AM2315::begin(void)
{
	this->_i2c->begin();
	return this->readData();
}

bool ROSY_AM2315::readData(void)
{
	this->temperature = NAN;
	this->humidity = NAN;
	uint8_t reply[10];
	if (this->lastreading)
	{
		if (millis() > this->lastreading)
		{
			if ((millis() - this->lastreading) < 2000)
				return false;
		}
		else
		{
			this->lastreading = millis();
			return false;
		}
	}
	this->lastreading = millis();

	this->_i2c->clearWriteError();
	this->_i2c->beginTransmission(AM2315_I2CADDR);
	delayMicroseconds(1000 * 2);
	if (this->_i2c->getWriteError()) return false;
	if (this->_i2c->endTransmission()) return false;
	
	this->_i2c->clearWriteError();
	this->_i2c->beginTransmission(AM2315_I2CADDR);
	this->_i2c->write(AM2315_READREG);
	this->_i2c->write(0x00);
	this->_i2c->write(4);
	if (this->_i2c->getWriteError()) return false;
	if (this->_i2c->endTransmission()) return false;

	delayMicroseconds(1000 * 10);
	if (this->_i2c->requestFrom(AM2315_I2CADDR, 8) <= 0) return false;
	for (uint8_t i = 0; i < 8; i++) reply[i] = _i2c->read();
	if (reply[0] != AM2315_READREG) return false;
	if (reply[1] != 4) return false;
	this->humidity = ((reply[2] * 256) + reply[3]) / 10;
	this->temperature = (((reply[4] & 0x7F) * 256) + reply[5]) / 10;
	if (reply[4] >> 7) this->temperature = -this->temperature;
	return true;
}

float ROSY_AM2315::getTemperature(void)
{
	float temp = temperature;
	this->temperature = NAN;
	return temp;
}

float ROSY_AM2315::getHumidity(void)
{
	float temp = humidity;
	this->humidity = NAN;
	return temp;
}
