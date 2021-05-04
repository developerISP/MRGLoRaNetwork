#ifndef _ROSY_EEPROM_H_
#define _ROSY_EEPROM_H_

#include <Arduino.h>
#include "ROSY_Config.h"
#include <Wire.h>

class ROSY_EEPROM {
private:
	class PKT_LOG {
	public:
		uint8_t act;
		uint32_t ts;
		PKT_LOG* next = NULL;
	};
	static PKT_LOG* pktLOG;
	static bool isEEPROMcom;

	static void(*logger)(uint8_t);
	static void def();

	static uint8_t buffer[DIM_EEPROM_BUFF - 1];

	static uint8_t QUEUEtrunc;
	static int16_t QUEUEaddrStart;
	static int16_t QUEUEaddrFinish;

	static uint8_t LOGtrunc;
	static int16_t LOGaddrStart;
	static int16_t LOGaddrFinish;

	static uint8_t INFtrunc;
	static int16_t INFaddrStart;
	static int16_t INFaddrFinish;
#if EEPROM_RAM
	static uint8_t queue[__EEPROM_LENGTH__];
#endif

	class AT24C256
	{
	public:
		static void writeByte(uint16_t memoryAddress, uint8_t data);
		static void writeArray(uint16_t memoryAddress, uint8_t data[], uint16_t arrayLength);
		static uint8_t readByte(uint16_t memoryAddress);
	};
public:

	static bool resetEEPROM();
	static bool readAllEEPROM();

	static void setLogger(void(*logCB)(uint8_t));

	static void LET_init();

	static void LET_appendBufferToQueue();

	static bool LET_getBufferFromQueue(uint8_t n, uint8_t* tmpbuff);

	static void LET_delBufferFromQueue();

	static uint16_t LET_getQueueLength();


	static void BUFF_setTime(uint32_t timestamp);


	static void BUFF_setTemperature_M(uint16_t _est, uint16_t _int);

	static void BUFF_setTemperature_SD(uint16_t _est, uint16_t _int);


	static void BUFF_setHumidity_M(uint16_t _est, uint16_t _int);

	static void BUFF_setHumidity_SD(uint16_t _est, uint16_t _int);

	static void BUFF_setPressure_M(uint16_t P);

	static void BUFF_setPressure_SD(uint16_t P);

	static void BUFF_setPM_M(uint16_t pm1, uint16_t pm25, uint16_t pm10);

	static void BUFF_setPM_SD(uint16_t pm1, uint16_t pm25, uint16_t pm10);


	static void BUFF_setOzone_M(uint16_t S1_1, uint16_t S1_2, uint16_t S2_1, uint16_t S2_2, uint16_t S3_1, uint16_t S3_2, uint16_t S4_1, uint16_t S4_2);

	static void BUFF_setOzone_SD(uint16_t S1_1, uint16_t S1_2, uint16_t S2_1, uint16_t S2_2, uint16_t S3_1, uint16_t S3_2, uint16_t S4_1, uint16_t S4_2);


	static void BUFF_setAccel_M(uint16_t x, uint16_t y, uint16_t z);

	static void BUFF_setAccel_SD(uint16_t x, uint16_t y, uint16_t z);


	static void BUFF_setMag_M(uint16_t x, uint16_t y, uint16_t z);

	static void BUFF_setMag_SD(uint16_t x, uint16_t y, uint16_t z);


	static void BUFF_setGyro_M(uint16_t x, uint16_t y, uint16_t z);

	static void BUFF_setGyro_SD(uint16_t x, uint16_t y, uint16_t z);


	static void BUFF_setWindRain_M(uint16_t speed, uint16_t direction, uint16_t rain);

	static void BUFF_setWindRain_SD(uint16_t speed, uint16_t direction, uint16_t rain);



	static void LOG_init();

	static void LOG_addActivity(uint8_t activity, uint32_t timestamp);

	static uint16_t LOG_getQueueLength();

	static bool LOG_getFromQueue(uint8_t n, uint8_t* tmpbuff);



	static void INF_init();

	static void INF_add(uint16_t BV, uint16_t BA, uint16_t SN, uint16_t RSSI, uint32_t timestamp);

	static uint16_t INF_getQueueLength();

	static bool INF_getFromQueue(uint8_t n, uint8_t* tmpbuff);

};
#endif