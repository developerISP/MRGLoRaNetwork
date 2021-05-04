#define CONTROL_UNIT_ID 1
#include "Arduino.h"
#include "ROSY_Config.h"
#include "ROSY_LoRa.h"
#include "ROSY_Sleep.h"
#include "ROSY_EEPROM.h"
#include "Compressor.h"
#include "SendDecompressedPacket.h"
#include "Statistics.h"
#include "ControlUnit.h"
#include "ControlUnitConfig.h"
#include "ROSY_Config.h"

ControlUnit cu;
uint8_t PKT_sent = 0;

void serialCallBack(bool enable, bool flush, bool isRCV)
{
	if (enable) ROSY_Sleep::SerialON();
	else if (!isRCV) ROSY_Sleep::SerialOFF(flush);
	return;
}

void loggerCallback(uint8_t act)
{
#if ENABLE_DEBUG_MODE
	Serial.println("Logger --> "+String(act));
#endif
	if (act==30)
	{
		uint32_t dat = cu.ntime.unixtime();
		ROSY_EEPROM::LOG_addActivity(act, dat);
		return;
	}
	else
	{
		uint32_t dat = cu.DS3231getDateTime().unixtime();
		ROSY_EEPROM::LOG_addActivity(act, dat);
		return;
	}
}
bool update()
{
	loggerCallback(LOGACT_update);
	if (ROSY_Lora_End::pktRX != NULL)
	{
		if (ROSY_Lora_End::pktRX->pkt_rcv == PKT_type_RQST) 
		{
			if(ROSY_Lora_End::received_timestamp > 0) cu.DS3231setDateTime(ROSY_Lora_End::received_timestamp);
			ROSY_Lora_End::received_timestamp = 0;
#if ENABLE_DEBUG_MODE
			Serial.println("RQST_A " + String(ROSY_EEPROM::LET_getQueueLength()));
#endif
			loggerCallback(LOGACT_lora_rqst);
			uint8_t buff[DIM_EEPROM_BUFF - 1];
			uint8_t status = 0;
			PKT_sent = 0;
			if (ROSY_EEPROM::LET_getBufferFromQueue(0, buff))
			{
				loggerCallback(LOGACT_lora_let1);
				ROSY_Lora_End::PKT_setData(0, buff);
				status |= INF_LET_1;
				PKT_sent++;
			}
			if (ROSY_EEPROM::LET_getBufferFromQueue(1, buff))
			{
				loggerCallback(LOGACT_lora_let2);
				ROSY_Lora_End::PKT_setData(1, buff);
				status |= INF_LET_2;
				PKT_sent++;
			}
			if (ROSY_EEPROM::LET_getQueueLength() >= 3)
			{
				loggerCallback(LOGACT_lora_more_data);
				status |= INF_MORE_DATA;
#if ENABLE_DEBUG_MODE
				Serial.println("MORE DATA");
#endif
			}
#if ENABLE_DEBUG_MODE
			Serial.println("RQST_B " + String(PKT_sent));
#endif
			float RSSI;
			float SN;
			ROSY_Lora_End::LoRa_getINFO(&SN, &RSSI);
			ROSY_Lora_End::PKT_setStatus(
				status,
				ROSY::Compressor::floatToIntEncode(SN, MIN_LoRa_SN, MAX_LoRa_SN),
				ROSY::Compressor::floatToIntEncode(RSSI, MIN_LoRa_RSSI, MAX_LoRa_RSSI),
				ROSY::Compressor::floatToIntEncode(cu.getBatteryVoltage(), MIN_BATTERY_VOLTAGE_V, MAX_BATTERY_VOLTAGE_V),
				ROSY::Compressor::floatToIntEncode(cu.getBatteryCurrent(), MIN_BATTERY_CURRENT_mA, MAX_BATTERY_CURRENT_mA)
			);
			ROSY_Lora_End::PKT_send(PKT_sent);
		}
		else if (ROSY_Lora_End::pktRX->pkt_rcv == PKT_type_ACK)
		{
			loggerCallback(LOGACT_lora_ack);
#if ENABLE_DEBUG_MODE
			Serial.println("ACK_A " + String(PKT_sent) + " " + String(ROSY_EEPROM::LET_getQueueLength()));
#endif
			for (uint8_t i = 0; i < PKT_sent; i++) ROSY_EEPROM::LET_delBufferFromQueue();
			PKT_sent = 0;
#if ENABLE_DEBUG_MODE
			Serial.println("ACK_B " + String(ROSY_EEPROM::LET_getQueueLength()));
#endif
		}
		ROSY_Lora_End::PKT_RCV* pktDEL = ROSY_Lora_End::pktRX;
		ROSY_Lora_End::pktRX = ROSY_Lora_End::pktRX->next;
		delete pktDEL;
	}
	return false;
}
void callbackPowerOnSensor()
{
	return;
}
void callbackPowerOffSensor()
{
	return;
}

int main()
{
	init();
#if SLEEP_ENABLE
	ROSY_Sleep::SerialON();
#else
	Wire.begin();
	_delay_ms(SERIAL_TIME);
#ifdef HAVE_HWSERIAL0
#if HS0enable
	if (HS0baudRate != 0) 
	{
		Serial.begin(HS0baudRate);
		_delay_ms(SERIAL_TIME);
	}
#endif
#endif
#ifdef HAVE_HWSERIAL1
#if HS1enable
	if (HS1baudRate != 0) 
	{
		Serial1.begin(HS1baudRate);
		_delay_ms(SERIAL_TIME);
	}
#endif
#endif
#ifdef HAVE_HWSERIAL2
#if HS2enable
	if (HS2baudRate != 0) {
		Serial2.begin(HS2baudRate);
		_delay_ms(SERIAL_TIME);
	}
#endif
#endif
#ifdef HAVE_HWSERIAL3
#if HS3enable
	if (HS3baudRate != 0) 
	{
		Serial3.begin(HS3baudRate);
		_delay_ms(SERIAL_TIME);
	}
#endif
#endif
	SPI.begin();
	_delay_ms(SERIAL_TIME);
#endif
	_delay_ms(100);
	Serial.println();
	Serial.println("//////////////////////////////////////////////////////////////");
	Serial.print("************  ");
	Serial.print(" ______   _______ _______ __   __ ");
	Serial.println("  ************");
	Serial.print("************  ");
	Serial.print("|    _ | |       |       |  | |  |");
	Serial.println("  ************");
	Serial.print("************  ");
	Serial.print("|   | || |   _   |  _____|  |_|  |");
	Serial.println("  ************");
	Serial.print("************  ");
	Serial.print("|   |_||_|  | |  | |_____|       |");
	Serial.println("  ************");
	Serial.print("************  ");
	Serial.print("|    __  |  |_|  |_____  |_     _|");
	Serial.println("  ************");
	Serial.print("************  ");
	Serial.print("|   |  | |       |_____| | |   |  ");
	Serial.println("  ************");
	Serial.print("************  ");
	Serial.print("|___|  |_|_______|_______| |___|  ");
	Serial.println("  ************");
	Serial.println("Project realised by ITIS MAX PLANCK of Treviso and ISP-CNR");
	Serial.println("Software Developed and Coded by: ");
	Serial.println("\t->\tBaseggio Nicolo");
	Serial.println("\t->\tGranziol Massimiliano");
	Serial.println("\t->\tDal Bo Davide");
	Serial.println("\t->\tDe Marchi Andrea");

	Serial.println("//////////////////////////////////////////////////////////////");
	for (int i = 0; i < 4; i++)
	{
		_delay_ms(1000);
		Serial.println();
	}
	Serial.print("Process Starting ");
	for (int i = 0; i < 30; i++)
	{
		Serial.print("#");
		_delay_ms(50);
	}
	Serial.println();

#if ENABLE_RESET_EEPROM
	Serial.println("Start EEPROM RESET");
	uint32_t timestartErase = millis();
	ROSY_EEPROM::resetEEPROM();
	for(int i = 0; i < 10; i++) Serial.print("#");
	Serial.println();
	Serial.println("EEPROM RESET DONE");
	for(int i = 0; i < 10; i++) Serial.print("#");
	Serial.println();
	Serial.println("The erasing of the EEPROM has used: " + String(millis() - timestartErase) + " ms");
#endif

	ROSY_EEPROM::setLogger(loggerCallback);
	ROSY_EEPROM::LOG_init();
	ROSY_EEPROM::LET_init();
	ROSY_EEPROM::INF_init();
	ROSY_EEPROM::LOG_addActivity(LOGACT_start_system, DateTime(1, 1, 1, 1, 1, 1).unixtime());
	cu.ControlUnitInit(update, loggerCallback);
	ROSY_Lora_End::LoRaInit(CONTROL_UNIT_ID, 53, 11, 18, serialCallBack, loggerCallback);
	ROSY_Sleep::setUpdater(callbackPowerOnSensor, callbackPowerOffSensor, serialCallBack, update, loggerCallback);
	loggerCallback(LOGACT_setup_system_done);
	for (;;)
	{
		DateTime dt = cu.DS3231getDateTime();
		uint16_t cLet;
		uint16_t t;
		if (cu.getDS3231status())
		{
			cLet = ROSY_Sleep::rtc2let(dt.hour(), dt.minute(), dt.second());
			t = ROSY_Sleep::rtcSYNCsleep(cLet, dt.hour(), dt.minute(), dt.second());
		}
		else
		{
			t = SLEEP_TIME;
			cLet = 65535;
		}
#if ENABLE_DEBUG_MODE
		Serial.println("sleep" + String(t) + " " + String(cLet));
#endif
		while (t > 0)
		{
			ROSY_Sleep::sleepMode(t);
			dt = cu.DS3231getDateTime();
			_delay_ms(1000);
			if (cLet != 65535)
			{
				if (cu.getDS3231status())
				{
					_delay_ms(1000);
					t = ROSY_Sleep::rtcSYNCsleep(cLet, dt.hour(), dt.minute(), dt.second());
				}
				else
				{
					t = 0;
				}
			}
			Serial.println(t);
		}
		update();
		ROSY_Lora_End::dropReceived=true;
		cu.readSensor();
		ROSY_Lora_End::dropReceived=false;
	}
	return 1;
}