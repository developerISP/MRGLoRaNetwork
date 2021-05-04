#ifndef _ROSY_SLEEP_H_
#define _ROSY_SLEEP_H_

#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <SPI.h>
#include "ROSY_Config.h"
#include "ControlUnitConfig.h"

#ifdef HAVE_HWSERIAL0
#define HS0baudRate 9600
#define HS0enable true
#if HS0enable
#if HS0baudRate == 0
#error Choose the baud rate for HS0
#endif
#endif
#endif
#ifdef HAVE_HWSERIAL1
#define HS1baudRate  9600
#define HS1enable ENABLE_SEN0186
#if HS1enable
#if HS1baudRate == 0
#error Choose the baud rate for HS1
#endif
#endif
#endif
#ifdef HAVE_HWSERIAL2
#define HS2baudRate  9600
#define HS2enable false
#if HS2enable
#if HS2baudRate == 0
#error Choose the baud rate for HS2
#endif
#endif
#endif
#ifdef HAVE_HWSERIAL3
#define HS3baudRate  9600
#define HS3enable false
#if HS3enable
#if HS3baudRate == 0
#error Choose the baud rate for HS3
#endif
#endif
#endif

class ROSY_Sleep {
private:
	static void(*SensorPowerDisable)(void);
	static void(*SensorPowerEnable)(void);
	static void(*serialEnabler)(bool, bool, bool);
	static bool(*UpdateInSleep)(void);
	static void(*logger)(uint8_t);
	static int isSerialON;
	static int isSleeping;
public:
	static void def();

	static void setUpdater(void(*PwEn)(void), void(*PwDis)(void), void(*SeEn)(bool, bool, bool), bool(*UpSleep)(void), void(*logCB)(uint8_t));

	static uint16_t rtc2let(uint8_t hour, uint8_t minute, uint8_t second);
	static uint32_t rtcSYNCsleep(uint16_t let, uint8_t hour, uint8_t minute, uint8_t second);

	static void SerialOFF(bool flush);

	static void SerialON();

	static void sleepMode(uint16_t t);
};

#endif