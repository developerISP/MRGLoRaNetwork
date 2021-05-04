#include "Arduino.h"
#include "SendDecompressedPacket.h"
#include "ROSY_Config.h"
#include "ControlUnitConfig.h"
#include "Compressor.h"
#include "time.h"

template <typename T>
T uint8_array_to_uintN(uint8_t array[], int startPosition)
{
	T temp = 0;
	for (int i = 0; i < sizeof(T); i++)
		temp |= (T)array[startPosition + i] << (8 * i);
	return temp;
}

void ROSY::_send_decompressed_packet(uint8_t packet[])
{
#if ENABLE_DEBUG_MODE
	Serial.print("start packet: ");
#endif
	Serial.print("sp");
	Serial.println();
	int n = 0;
	if ((packet[1] & INF_LET_1) == INF_LET_1) {
		n++;
		if ((packet[1] & INF_LET_2) == INF_LET_2) {
			n++;
			if ((packet[1] & INF_LET_3) == INF_LET_3) {
				n++;
			}
		}
	}
#if ENABLE_DEBUG_MODE
	Serial.print("CU ADDRESS: ");
#endif
	Serial.print(packet[0]);
	Serial.println();
#if ENABLE_DEBUG_MODE
	Serial.print("READ NUMBER: ");
#endif
	Serial.print(n);
	Serial.println();
#if ENABLE_DEBUG_MODE
	Serial.print("BATTERY VOLTAGE: ");
#endif
	Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, OFFSET_PKT_STATUS + 1), MIN_BATTERY_VOLTAGE_V, MAX_BATTERY_VOLTAGE_V));
	Serial.println();
#if ENABLE_DEBUG_MODE
	Serial.print("BATTERY CURRENT: ");
#endif
	Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, OFFSET_PKT_STATUS + 3), MIN_BATTERY_CURRENT_mA, MAX_BATTERY_CURRENT_mA));
	Serial.println();
#if ENABLE_DEBUG_MODE
	Serial.print("SN: ");
#endif
	Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, OFFSET_PKT_STATUS + 5), MIN_LoRa_SN, MAX_LoRa_SN));
	Serial.println();
#if ENABLE_DEBUG_MODE
	Serial.print("RSSI: ");
#endif
	Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, OFFSET_PKT_STATUS + 7), MIN_LoRa_RSSI, MAX_LoRa_RSSI));
	Serial.println();
	for (int k = 0; k < n; k++)
	{
#if ENABLE_DEBUG_MODE
		Serial.print(" start read: ");
#endif
		Serial.print("sr" + String(k));
		Serial.println();
		DateTime dt = DateTime(uint8_array_to_uintN <uint32_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_TIME));
#if ENABLE_DEBUG_MODE
		Serial.print("Date: ");
#endif
		Serial.print(dt.day());
#if ENABLE_DEBUG_MODE
		Serial.print(" / ");
#else 
		Serial.println();
#endif
		Serial.print(dt.month());
#if ENABLE_DEBUG_MODE
		Serial.print(" / ");
#else 
		Serial.println();
#endif
		Serial.print(dt.year());
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("Time: ");
#endif
		Serial.print(dt.hour());
#if ENABLE_DEBUG_MODE
		Serial.print(" : ");
#else 
		Serial.println();
#endif
		Serial.print(dt.minute());
#if ENABLE_DEBUG_MODE
		Serial.print(" : ");
#else 
		Serial.println();
#endif
		Serial.print(dt.second());
		Serial.println();

#if ENABLE_DEBUG_MODE
		Serial.print("EXT TEMP MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_TEMPERATURE + 0), MIN_TEMPERATURE_AM2315, MAX_TEMPERATURE_AM2315));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("INT TEMPERATURE MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_TEMPERATURE + 2), MIN_TEMPERATURE_BME280, MAX_TEMPERATURE_BME280));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("EXT TEMP MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_TEMPERATURE + 4), MIN_MSD_TEMPERATURE_AM2315, MAX_MSD_TEMPERATURE_AM2315));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("INT TEMP MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_TEMPERATURE + 6), MIN_MSD_TEMPERATURE_BME280, MIN_MSD_TEMPERATURE_BME280));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("EXT HUMIDITY MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_HUMIDITY + 0), MIN_HUMIDITY_AM2315, MAX_HUMIDITY_AM2315));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("INT HUMIDITY MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_HUMIDITY + 2), MIN_HUMIDITY_BME280, MAX_HUMIDITY_BME280));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("EXT HUMUDITY MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_HUMIDITY + 4), MIN_MSD_HUMIDITY_AM2315, MAX_MSD_HUMIDITY_AM2315));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("INT HUMIDITY MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_HUMIDITY + 6), MIN_MSD_HUMIDITY_BME280, MAX_MSD_HUMIDITY_BME280));
		Serial.println();

#if ENABLE_DEBUG_MODE
		Serial.print("INT PRESSURE MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_PRESSURE + 0), MIN_PRESSURE_BME280, MAX_PRESSURE_BME280));
		Serial.println();

#if ENABLE_DEBUG_MODE
		Serial.print("INT PRESSURE MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_PRESSURE + 2), MIN_MSD_PRESSURE_BME280, MAX_MSD_PRESSURE_BME280));
		Serial.println();

#if ENABLE_DEBUG_MODE
		Serial.print("PM1 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_PM + 0), MIN_PM1_OPCN3, MAX_PM1_OPCN3));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("PM2_5 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_PM + 2), MIN_PM2_5_OPCN3, MAX_PM2_5_OPCN3));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("PM10 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_PM + 4), MIN_PM10_OPCN3, MAX_PM10_OPCN3));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("PM1 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_PM + 6), MIN_MSD_PM1_OPCN3, MAX_MSD_PM1_OPCN3));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("PM2_5 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_PM + 8), MIN_MSD_PM2_5_OPCN3, MAX_MSD_PM2_5_OPCN3));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("PM10 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_PM + 10), MIN_MSD_PM10_OPCN3, MAX_MSD_PM10_OPCN3));
		Serial.println();


#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 1_1 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 0), MIN_VOLTAGE_ADS1115_1_mV, MAX_VOLTAGE_ADS1115_1_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 1_2 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 2), MIN_VOLTAGE_ADS1115_1_mV, MAX_VOLTAGE_ADS1115_1_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 2_1 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 4), MIN_VOLTAGE_ADS1115_1_mV, MAX_VOLTAGE_ADS1115_1_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 2_2 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 6), MIN_VOLTAGE_ADS1115_1_mV, MAX_VOLTAGE_ADS1115_1_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 3_1 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 8), MIN_VOLTAGE_ADS1115_2_mV, MAX_VOLTAGE_ADS1115_2_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 3_2 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 10), MIN_VOLTAGE_ADS1115_2_mV, MAX_VOLTAGE_ADS1115_2_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 4_1 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 12), MIN_VOLTAGE_ADS1115_2_mV, MAX_VOLTAGE_ADS1115_2_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 4_2 MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 14), MIN_VOLTAGE_ADS1115_2_mV, MAX_VOLTAGE_ADS1115_2_mV));
		Serial.println();

#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 1_1 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 16), MIN_MSD_VOLTAGE_ADS1115_1_mV, MAX_MSD_VOLTAGE_ADS1115_1_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 1_2 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 18), MIN_MSD_VOLTAGE_ADS1115_1_mV, MAX_MSD_VOLTAGE_ADS1115_1_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 2_1 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 20), MIN_MSD_VOLTAGE_ADS1115_1_mV, MAX_MSD_VOLTAGE_ADS1115_1_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 2_2 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 22), MIN_MSD_VOLTAGE_ADS1115_1_mV, MAX_MSD_VOLTAGE_ADS1115_1_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 3_1 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 24), MIN_MSD_VOLTAGE_ADS1115_2_mV, MAX_MSD_VOLTAGE_ADS1115_2_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 3_2 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 26), MIN_MSD_VOLTAGE_ADS1115_2_mV, MAX_MSD_VOLTAGE_ADS1115_2_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 4_1 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 28), MIN_MSD_VOLTAGE_ADS1115_2_mV, MAX_MSD_VOLTAGE_ADS1115_2_mV));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("OZONE 4_2 MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_OZONE + 30), MIN_MSD_VOLTAGE_ADS1115_2_mV, MAX_MSD_VOLTAGE_ADS1115_2_mV));
		Serial.println();


#if ENABLE_DEBUG_MODE
		Serial.print("ACCEL X MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_ACCEL + 0), MIN_LSM9DS0_ACCELRANGE_2G, MAX_LSM9DS0_ACCELRANGE_2G));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("ACCEL Y MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_ACCEL + 2), MIN_LSM9DS0_ACCELRANGE_2G, MAX_LSM9DS0_ACCELRANGE_2G));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("ACCEL Z MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_ACCEL + 4), MIN_LSM9DS0_ACCELRANGE_2G, MAX_LSM9DS0_ACCELRANGE_2G));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("ACCEL X MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_ACCEL + 6), MIN_MSD_LSM9DS0_ACCELRANGE_2G, MAX_MSD_LSM9DS0_ACCELRANGE_2G));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("ACCEL Y MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_ACCEL + 8), MIN_MSD_LSM9DS0_ACCELRANGE_2G, MAX_MSD_LSM9DS0_ACCELRANGE_2G));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("ACCEL Z MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_ACCEL + 10), MIN_MSD_LSM9DS0_ACCELRANGE_2G, MAX_MSD_LSM9DS0_ACCELRANGE_2G));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("MAG X MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_MAG + 0), MIN_LSM9DS0_MAGGAIN_4GAUSS, MAX_LSM9DS0_MAGGAIN_4GAUSS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("MAG Y MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_MAG + 2), MIN_LSM9DS0_MAGGAIN_4GAUSS, MAX_LSM9DS0_MAGGAIN_4GAUSS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("MAG Z MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_MAG + 4), MIN_LSM9DS0_MAGGAIN_4GAUSS, MAX_LSM9DS0_MAGGAIN_4GAUSS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("MAG X MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_MAG + 6), MIN_MSD_LSM9DS0_MAGGAIN_4GAUSS, MAX_MSD_LSM9DS0_MAGGAIN_4GAUSS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("MAG Y MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_MAG + 8), MIN_MSD_LSM9DS0_MAGGAIN_4GAUSS, MAX_MSD_LSM9DS0_MAGGAIN_4GAUSS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("MAG Z MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_MAG + 10), MIN_MSD_LSM9DS0_MAGGAIN_4GAUSS, MAX_MSD_LSM9DS0_MAGGAIN_4GAUSS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("GYRO X MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_GYRO + 0), MIN_LSM9DS0_GYROSCALE_245DPS, MAX_LSM9DS0_GYROSCALE_245DPS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("GYRO Y MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_GYRO + 2), MIN_LSM9DS0_GYROSCALE_245DPS, MAX_LSM9DS0_GYROSCALE_245DPS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("GYRO Z MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_GYRO + 4), MIN_LSM9DS0_GYROSCALE_245DPS, MAX_LSM9DS0_GYROSCALE_245DPS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("GYRO X MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_GYRO + 6), MIN_MSD_LSM9DS0_GYROSCALE_245DPS, MAX_MSD_LSM9DS0_GYROSCALE_245DPS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("GYRO Y MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_GYRO + 8), MIN_MSD_LSM9DS0_GYROSCALE_245DPS, MAX_MSD_LSM9DS0_GYROSCALE_245DPS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("GYRO Z MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_GYRO + 10), MIN_MSD_LSM9DS0_GYROSCALE_245DPS, MAX_MSD_LSM9DS0_GYROSCALE_245DPS));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("WIND DIRECTION MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_WIND + 0), MIN_SEN0186_WIND_DIRECTION, MAX_SEN0186_WIND_DIRECTION));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("WIND SPEED MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_WIND + 2), MIN_SEN0186_WIND_SPEED, MAX_SEN0186_WIND_SPEED));
		Serial.println();

#if ENABLE_DEBUG_MODE
		Serial.print("RAIN FALL ONE HOUR MD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_WIND + 4), MIN_SEN0186_RAIN_FALL_ONE_HOURS, MAX_SEN0186_RAIN_FALL_ONE_HOURS));
		Serial.println();

#if ENABLE_DEBUG_MODE
		Serial.print("WIND DIRECTION MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_WIND + 6), MIN_MSD_SEN0186_WIND_DIRECTION, MAX_MSD_SEN0186_WIND_DIRECTION));
		Serial.println();
#if ENABLE_DEBUG_MODE
		Serial.print("WIND SPEED MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_WIND + 8), MIN_MSD_SEN0186_WIND_SPEED, MAX_MSD_SEN0186_WIND_SPEED));
		Serial.println();

#if ENABLE_DEBUG_MODE
		Serial.print("RAIN FALL ONE HOUR MSD: ");
#endif
		Serial.print(ROSY::Compressor::IntToFloatDecode(uint8_array_to_uintN <uint16_t>(packet, k * (OFFSET_PKT_END - OFFSET_PKT_NEW) + OFFSET_PKT_WIND + 10), MIN_MSD_SEN0186_RAIN_FALL_ONE_HOUR, MAX_MSD_SEN0186_RAIN_FALL_ONE_HOUR));
		Serial.println();

#if ENABLE_DEBUG_MODE
		Serial.print("end read: ");
#endif
		Serial.print("er" + String(k));
		Serial.println();
	}
#if ENABLE_DEBUG_MODE
	Serial.print("end packet: ");
#endif
	Serial.print("ep");
	Serial.println();
}