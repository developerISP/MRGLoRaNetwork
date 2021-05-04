#include "Compressor.h"

uint16_t ROSY::Compressor::floatToIntEncode(float value, float min, float max)
{
	if (value > max || value < min || isnan(value)) return 65535;
	else return (value - min) * 65534 / (max - min);
}

float  ROSY::Compressor::IntToFloatDecode(uint16_t value, float min, float max)
{
	if (value == 65535)	return NAN;
	else return ((float)value) * (max - min) / 65534 + min;
}
