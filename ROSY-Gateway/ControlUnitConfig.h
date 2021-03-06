#ifndef _CONTROLUNITCONFIG_H_
#define _CONTROLUNITCONFIG_H_ 

#define ENABLE_RESET_EEPROM false
#define ENABLE_READ_ALL_EEPROM false

#define ENABLE_DEBUG_MODE true
#define ENABLE_I2CSCANNER true
#define AUTO_TIME_ADJUST false
#define ENABLE_DS3231 true

#define ENABLE_INA219 false
#define ENABLE_OPCN3 false
#define ENABLE_AM2315 false
#define ENABLE_BME280 false
#define ENABLE_SENSOR_1 false
#define ENABLE_SENSOR_2 false
#define ENABLE_SENSOR_3 false
#define ENABLE_SENSOR_4 false
#define ENABLE_SENSOR_5 false
#define ENABLE_SENSOR_6 false
#define ENABLE_LSM9DS0 false
#define ENABLE_SEN0186 false


#define __ROSY_ADDRESS_ADS1115_1__		0x48
#define __ROSY_ADDRESS_ADS1115_2__		0x49
#define __ROSY_ADDRESS_ADS1115_3__		NAN
#define __ROSY_ADDRESS_INA219__			0x40
#define __ROSY_ADDRESS_OPCN3__			53			
#define __ROSY_ADDRESS_AM2315__			0x5C
#define __ROSY_ADDRESS_BME280__			0x77	
#define __ROSY_ADDRESS_BME280_ALTERNATIVE__	0x76
#define __ROSY_ADDRESS_LSM9DS1_ACCELGYRO__ 0x6B
#define __ROSY_ADDRESS_LSM9DS1_MAG__ 0x1E

#define ADS1115_CONVERSION_mV 0.1875F
#define MPH_TO_MS 2.2369362920544

#define MIN_TEMPERATURE_BME280 -40
#define MAX_TEMPERATURE_BME280 85
#define MIN_MSD_TEMPERATURE_BME280 0
#define MAX_MSD_TEMPERATURE_BME280 62.5

#define MIN_HUMIDITY_BME280 0
#define MAX_HUMIDITY_BME280 100
#define MIN_MSD_HUMIDITY_BME280 0
#define MAX_MSD_HUMIDITY_BME280 50

#define MIN_PRESSURE_BME280 300
#define MAX_PRESSURE_BME280 1100
#define MIN_MSD_PRESSURE_BME280 0
#define MAX_MSD_PRESSURE_BME280 400

#define MIN_TEMPERATURE_AM2315 -40
#define MAX_TEMPERATURE_AM2315 125
#define MIN_MSD_TEMPERATURE_AM2315 0
#define MAX_MSD_TEMPERATURE_AM2315 82.5

#define MIN_HUMIDITY_AM2315 0
#define MAX_HUMIDITY_AM2315 100
#define MIN_MSD_HUMIDITY_AM2315 0
#define MAX_MSD_HUMIDITY_AM2315 50

#define MIN_VOLTAGE_ADS1115_1_mV 0
#define MAX_VOLTAGE_ADS1115_1_mV 6144
#define MIN_MSD_VOLTAGE_ADS1115_1_mV 0
#define MAX_MSD_VOLTAGE_ADS1115_1_mV 3072

#define MIN_VOLTAGE_ADS1115_2_mV 0
#define MAX_VOLTAGE_ADS1115_2_mV 6144
#define MIN_MSD_VOLTAGE_ADS1115_2_mV 0
#define MAX_MSD_VOLTAGE_ADS1115_2_mV 3072

#define MIN_VOLTAGE_ADS1115_3_mV 0
#define MAX_VOLTAGE_ADS1115_3_mV 6144
#define MIN_MSD_VOLTAGE_ADS1115_3_mV 0
#define MAX_MSD_VOLTAGE_ADS1115_3_mV 3072

#define MIN_LSM9DS0_ACCELRANGE_2G (-2*9.81)
#define MAX_LSM9DS0_ACCELRANGE_2G (2*9.81)
#define MIN_MSD_LSM9DS0_ACCELRANGE_2G (0*9.81)
#define MAX_MSD_LSM9DS0_ACCELRANGE_2G (2*9.81)

#define MIN_LSM9DS0_ACCELRANGE_4G (-4*9.81)
#define MAX_LSM9DS0_ACCELRANGE_4G (4*9.81)
#define MIN_MSD_LSM9DS0_ACCELRANGE_4G (0*9.81)
#define MAX_MSD_LSM9DS0_ACCELRANGE_4G (4*9.81)

#define MIN_LSM9DS0_ACCELRANGE_6G (-6*9.81)
#define MAX_LSM9DS0_ACCELRANGE_6G (6*9.81)
#define MIN_MSD_LSM9DS0_ACCELRANGE_6G (0*9.81)
#define MAX_MSD_LSM9DS0_ACCELRANGE_6G (6*9.81)

#define MIN_LSM9DS0_ACCELRANGE_8G (-8*9.81)
#define MAX_LSM9DS0_ACCELRANGE_8G (8*9.81)
#define MIN_MSD_LSM9DS0_ACCELRANGE_8G (0*9.81)
#define MAX_MSD_LSM9DS0_ACCELRANGE_8G (8*9.81)

#define MIN_LSM9DS0_ACCELRANGE_16G (-16*9.81)
#define MAX_LSM9DS0_ACCELRANGE_16G (16*9.81)
#define MIN_MSD_LSM9DS0_ACCELRANGE_16G (0*9.81)
#define MAX_MSD_LSM9DS0_ACCELRANGE_16G (16*9.81)

#define MIN_LSM9DS0_MAGGAIN_2GAUSS -2000
#define MAX_LSM9DS0_MAGGAIN_2GAUSS 2000
#define MIN_MSD_LSM9DS0_MAGGAIN_2GAUSS 0
#define MAX_MSD_LSM9DS0_MAGGAIN_2GAUSS 2000


#define MIN_LSM9DS0_MAGGAIN_4GAUSS -4000
#define MAX_LSM9DS0_MAGGAIN_4GAUSS 4000
#define MIN_MSD_LSM9DS0_MAGGAIN_4GAUSS 0
#define MAX_MSD_LSM9DS0_MAGGAIN_4GAUSS 4000

#define MIN_LSM9DS0_MAGGAIN_8GAUSS -8000
#define MAX_LSM9DS0_MAGGAIN_8GAUSS 8000
#define MIN_MSD_LSM9DS0_MAGGAIN_8GAUSS 0
#define MAX_MSD_LSM9DS0_MAGGAIN_8GAUSS 8000

#define MIN_LSM9DS0_MAGGAIN_12GAUSS -12000
#define MAX_LSM9DS0_MAGGAIN_12GAUSS 12000
#define MIN_MSD_LSM9DS0_MAGGAIN_12GAUSS 0
#define MAX_MSD_LSM9DS0_MAGGAIN_12GAUSS 12000

#define MIN_LSM9DS0_GYROSCALE_245DPS ((float)(-245/180)*PI)
#define MAX_LSM9DS0_GYROSCALE_245DPS ((float)(245/180)*PI)
#define MIN_MSD_LSM9DS0_GYROSCALE_245DPS ((float)(0/180)*PI)
#define MAX_MSD_LSM9DS0_GYROSCALE_245DPS ((float)(245/180)*PI)

#define MIN_LSM9DS0_GYROSCALE_500DPS ((float)(-500/180)*PI)
#define MAX_LSM9DS0_GYROSCALE_500DPS ((float)(500/180)*PI)
#define MIN_MSD_LSM9DS0_GYROSCALE_500DPS ((float)(0/180)*PI)
#define MAX_MSD_LSM9DS0_GYROSCALE_500DPS ((float)(500/180)*PI)

#define MIN_LSM9DS0_GYROSCALE_2000DPS ((float)(-2000/180)*PI)
#define MAX_LSM9DS0_GYROSCALE_2000DPS ((float)(2000/180)*PI)
#define MIN_MSD_LSM9DS0_GYROSCALE_2000DPS ((float)(0/180)*PI)
#define MAX_MSD_LSM9DS0_GYROSCALE_2000DPS ((float)(2000/180)*PI)

#define MIN_SEN0186_WIND_DIRECTION 0
#define MAX_SEN0186_WIND_DIRECTION 360
#define MIN_MSD_SEN0186_WIND_DIRECTION 0
#define MAX_MSD_SEN0186_WIND_DIRECTION 180

#define MIN_SEN0186_WIND_SPEED 0 
#define MAX_SEN0186_WIND_SPEED 99.9 * MPH_TO_MS
#define MIN_MSD_SEN0186_WIND_SPEED 0 
#define MAX_MSD_SEN0186_WIND_SPEED 49.95 * MPH_TO_MS

#define MIN_SEN0186_RAIN_FALL_ONE_HOURS 0
#define MAX_SEN0186_RAIN_FALL_ONE_HOURS 9.99 
#define MIN_MSD_SEN0186_RAIN_FALL_ONE_HOUR 0 
#define MAX_MSD_SEN0186_RAIN_FALL_ONE_HOUR 4.995 

#define MIN_PM1_OPCN3 0
#define MAX_PM1_OPCN3 2000
#define MIN_MSD_PM1_OPCN3 0
#define MAX_MSD_PM1_OPCN3 1000

#define MIN_PM2_5_OPCN3 0 
#define MAX_PM2_5_OPCN3 2000
#define MIN_MSD_PM2_5_OPCN3 0
#define MAX_MSD_PM2_5_OPCN3 1000

#define MIN_PM10_OPCN3 0
#define MAX_PM10_OPCN3 2000
#define MIN_MSD_PM10_OPCN3 0
#define MAX_MSD_PM10_OPCN3 1000

#define MIN_BATTERY_VOLTAGE_V -20
#define MAX_BATTERY_VOLTAGE_V 20

#define MIN_BATTERY_CURRENT_mA -500
#define MAX_BATTERY_CURRENT_mA +500

#define MIN_LoRa_RSSI -148
#define MAX_LoRa_RSSI 20

#define MIN_LoRa_SN 0
#define MAX_LoRa_SN 10
#endif
