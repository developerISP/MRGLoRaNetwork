#ifndef _COMPRESSOR_H_
#define _COMPRESSOR_H_
#include "Arduino.h"

namespace ROSY
{
	class Compressor
	{
	public:	 static uint16_t floatToIntEncode(float value, float min, float max);
	public:  static float IntToFloatDecode(uint16_t value, float min, float max);
	};
}
#endif