#include "Arduino.h"
#include <Wire.h>
#include "ROSY_ADS1115.h"

static bool writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value)
{
	Wire.clearWriteError();
	Wire.beginTransmission(i2cAddress);
	Wire.write((uint8_t)reg);
	Wire.write((uint8_t)(value >> 8));
	Wire.write((uint8_t)(value & 0xFF));
	if (Wire.getWriteError()) return false;
	return Wire.endTransmission() ? false : true;
}

static bool readRegister(uint8_t i2cAddress, uint8_t reg, uint16_t* value = 0)
{
	Wire.clearWriteError();
	Wire.beginTransmission(i2cAddress);
	Wire.write(reg);
	if (Wire.getWriteError()) return false;
	if (Wire.endTransmission()) return false;
	if (Wire.requestFrom(i2cAddress, (uint8_t)2) <= 0) return false;
	if (value != 0)
	{
		*value = ((Wire.read() << 8) | Wire.read());
		return true;
	}
	else
	{
		Wire.read();
		Wire.read();
		return true;
	}
}

ROSY_ADS1115::ROSY_ADS1115(uint8_t i2cAddress)
{
	m_i2cAddress = i2cAddress;
	m_conversionDelay = ADS1115_CONVERSIONDELAY;
	m_bitShift = 0;
	m_gain = GAIN_TWOTHIRDS;
}

bool ROSY_ADS1115::begin()
{
	Wire.begin();
	return (this->readADC_SingleEnded((uint8_t)0) && this->readADC_SingleEnded((uint8_t)1) && this->readADC_SingleEnded((uint8_t)2) && this->readADC_SingleEnded((uint8_t)3));
}

void ROSY_ADS1115::setGain(adsGain_t gain)
{
	m_gain = gain;
}

adsGain_t ROSY_ADS1115::getGain()
{
	return m_gain;
}

bool ROSY_ADS1115::readADC_SingleEnded(uint8_t channel, uint16_t* _readVal = 0)
{
	if (channel > 3) return false;
	uint16_t config =
		ADS1015_REG_CONFIG_CQUE_NONE |   
		ADS1015_REG_CONFIG_CLAT_NONLAT |  
		ADS1015_REG_CONFIG_CPOL_ACTVLOW |
		ADS1015_REG_CONFIG_CMODE_TRAD |  
		ADS1015_REG_CONFIG_DR_1600SPS |  
		ADS1015_REG_CONFIG_MODE_SINGLE;  
	config |= m_gain;
	switch (channel)
	{
	case (0):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
		break;
	case (1):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
		break;
	case (2):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
		break;
	case (3):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
		break;
	}
	config |= ADS1015_REG_CONFIG_OS_SINGLE;
	if (!writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config)) return false;
	_delay_ms(ADS1115_CONVERSIONDELAY);
	if (!readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT, _readVal)) return false;
	if (_readVal != 0) *_readVal = (*_readVal) >> m_bitShift;
	return true;
}
