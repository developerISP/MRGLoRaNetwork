#ifndef _ROSY_STATUS_H_
#define _ROSY_STATUS_H_

#define DIM_PKT_ADDR 1
#define DIM_PKT_TYPE 1
#define DIM_PKT_STATUS 9
#define DIM_PKT_TIME 4
#define DIM_PKT_TEMPERATURE (2*4)
#define DIM_PKT_HUMIDITY (2*4)
#define DIM_PKT_PRESSURE (2*2)
#define DIM_PKT_PM (2*6)
#define DIM_PKT_OZONE (2*16)
#define DIM_PKT_ACCEL (2*6)
#define DIM_PKT_MAG (2*6)
#define DIM_PKT_GYRO (2*6)
#define DIM_PKT_WINDRAIN (2*6)

#define ADDR_START 3
#define ADDR_STOP 192
#define ADDR_TRUNC 12
#define ADDR_NULL 48 

#define EEPROM_RAM false
#define EEPROM_I2C_ADDRESS 0x50
#define EEPROM_TIME_WRITE 20
#define EEPROM_TIME_READ 10
#define EEPROM_MAX_FAILED_READ 5
#define EEPROM_MAX_FAILED_WRITE 5

#define __EEPROM_LENGTH__ 32768

#define LET_PER_READ 1
#define LOG_PER_READ 30
#define INF_PER_READ 1

#define DIM_EEPROM_BUFF (DIM_PKT_TIME + DIM_PKT_TEMPERATURE + DIM_PKT_HUMIDITY + DIM_PKT_PRESSURE + DIM_PKT_PM+DIM_PKT_OZONE + DIM_PKT_ACCEL + DIM_PKT_MAG + DIM_PKT_GYRO + DIM_PKT_WINDRAIN + 1) 
#define DIM_EEPROM_INF 13 
#define DIM_EEPROM_LOG 6 

#define WEIGHT_LET (LET_PER_READ / (float)(LET_PER_READ + INF_PER_READ + LOG_PER_READ))
#define QUEUE_EEPROM_TOT_DATA (((int)(__EEPROM_LENGTH__ * WEIGHT_LET)/ DIM_EEPROM_BUFF ))
#define FREE_SPACE_AFTER_LET (__EEPROM_LENGTH__ - QUEUE_EEPROM_TOT_DATA * DIM_EEPROM_BUFF)

#define WEIGHT_INF (INF_PER_READ/ (float)(INF_PER_READ + LOG_PER_READ))
#define INF_EEPROM_TOT_DATA ((int)((int)(FREE_SPACE_AFTER_LET * WEIGHT_INF) / (int)DIM_EEPROM_INF ))
#define FREE_SPACE_AFTER_LOG (FREE_SPACE_AFTER_LET - INF_EEPROM_TOT_DATA * DIM_EEPROM_INF)

#define WEIGHT_LOG (LOG_PER_READ / (float)LOG_PER_READ)
#define LOG_EEPROM_TOT_DATA ((int)((int)(FREE_SPACE_AFTER_LOG * WEIGHT_LOG) / (int)DIM_EEPROM_LOG ))
#define FREE_SPACE_AFTER_INF (FREE_SPACE_AFTER_LOG - LOG_EEPROM_TOT_DATA * DIM_EEPROM_LOG)

#define QUEUE_EEPROM_OFFSET 0
#define INF_EEPROM_OFFSET (DIM_EEPROM_BUFF*QUEUE_EEPROM_TOT_DATA)
#define LOG_EEPROM_OFFSET (DIM_EEPROM_BUFF*QUEUE_EEPROM_TOT_DATA+DIM_EEPROM_INF*INF_EEPROM_TOT_DATA)

#define LOGACT_start_system                     0
#define LOGACT_setup_system_done                (LOGACT_start_system + 1)
#define LOGACT_update                           (LOGACT_setup_system_done + 1)

#define LOGACT_serial_on                       10
#define LOGACT_serial_off                      (LOGACT_serial_on + 1 )

#define LOGACT_start_sleep                     20     
#define LOGACT_stop_sleep                      (LOGACT_start_sleep+ 1)
#define LOGACT_sensor_on                       (LOGACT_stop_sleep+ 1)
#define LOGACT_sensor_off                      (LOGACT_sensor_on + 1)

#define LOGACT_lora_rx                         30    
#define LOGACT_lora_tx                         (LOGACT_lora_rx + 1)   
#define LOGACT_lora_ack                        (LOGACT_lora_tx + 1)
#define LOGACT_lora_rqst                       (LOGACT_lora_ack + 1)
#define LOGACT_lora_let1                       (LOGACT_lora_rqst + 1)    
#define LOGACT_lora_let2                       (LOGACT_lora_let1 + 1)   
#define LOGACT_lora_let3                       (LOGACT_lora_let2 + 1) 
#define LOGACT_lora_more_data                  (LOGACT_lora_let3 + 1)     

#define LOGACT_init_AM2315                     40
#define LOGACT_init_BME280                     (LOGACT_init_AM2315+1)
#define LOGACT_init_OPCN3                      (LOGACT_init_BME280+1)  
#define LOGACT_init_ADS1115_1                  (LOGACT_init_OPCN3+1)
#define LOGACT_init_ADS1115_2                  (LOGACT_init_ADS1115_1+1)
#define LOGACT_init_ADS1115_3                  (LOGACT_init_ADS1115_2+1)
#define LOGACT_init_DS3231                     (LOGACT_init_ADS1115_3+1)
#define LOGACT_init_Sen0186                    (LOGACT_init_DS3231+1)
#define LOGACT_init_LSM9DS0                    (LOGACT_init_Sen0186+1)
#define LOGACT_init_INA219                     (LOGACT_init_LSM9DS0+1)
#define LOGACT_init_LoRa                       (LOGACT_init_INA219+1)

#define LOGACT_eeprom_buff_app                 60
#define LOGACT_eeprom_buff_del                 (LOGACT_eeprom_buff_app+1)
#define LOGACT_eeprom_buff_get                 (LOGACT_eeprom_buff_del+1)

#define LOGACT_eeprom_log_get                  (LOGACT_eeprom_buff_get+1)

#define LOGACT_eeprom_inf_app                  (LOGACT_eeprom_log_get+1)
#define LOGACT_eeprom_inf_get                  (LOGACT_eeprom_inf_app+1)

#define LOGACT_let                             90  
#define LOGACT_read_AM2315                     (LOGACT_let+1) 
#define LOGACT_read_BME280_temp                (LOGACT_read_AM2315+1) 
#define LOGACT_read_BME280_hum                 (LOGACT_read_BME280_temp+1)
#define LOGACT_read_BME280_pressure            (LOGACT_read_BME280_hum+1) 
#define LOGACT_read_OPCN3                      (LOGACT_read_BME280_pressure+1)
#define LOGACT_read_ADS1115_1_1                (LOGACT_read_OPCN3+1) 
#define LOGACT_read_ADS1115_1_2                (LOGACT_read_ADS1115_1_1+1) 
#define LOGACT_read_ADS1115_1_3                (LOGACT_read_ADS1115_1_2+1)  
#define LOGACT_read_ADS1115_1_4                (LOGACT_read_ADS1115_1_3+1) 
#define LOGACT_read_ADS1115_2_1                (LOGACT_read_ADS1115_1_4+1) 
#define LOGACT_read_ADS1115_2_2                (LOGACT_read_ADS1115_2_1+1) 
#define LOGACT_read_ADS1115_2_3                (LOGACT_read_ADS1115_2_2+1)  
#define LOGACT_read_ADS1115_2_4                (LOGACT_read_ADS1115_2_3+1) 
#define LOGACT_read_ADS1115_3_1                (LOGACT_read_ADS1115_2_4+1)  
#define LOGACT_read_ADS1115_3_2                (LOGACT_read_ADS1115_3_1+1) 
#define LOGACT_read_ADS1115_3_3                (LOGACT_read_ADS1115_3_2+1) 
#define LOGACT_read_ADS1115_3_4                (LOGACT_read_ADS1115_3_3+1) 
#define LOGACT_read_LSM9DS0                    (LOGACT_read_ADS1115_3_4+1) 
#define LOGACT_read_SEN0186					   (LOGACT_read_LSM9DS0+1)
#define LOGACT_read_wind_speed                 (LOGACT_read_SEN0186+1)
#define LOGACT_read_wind_direction             (LOGACT_read_wind_speed+1)
#define LOGACT_read_rain_fall                  (LOGACT_read_wind_direction+1) 
#define LOGACT_read_bat_mA                     (LOGACT_read_rain_fall+1)
#define LOGACT_read_bat_V                      (LOGACT_read_bat_mA+1)

#define LOGACT_get_int_temp                   120 
#define LOGACT_get_int_hum                    (LOGACT_get_int_temp+1) 
#define LOGACT_get_ext_temp                   (LOGACT_get_int_hum+1) 
#define LOGACT_get_ext_hum                    (LOGACT_get_ext_temp+1) 
#define LOGACT_get_int_pressure				  (LOGACT_get_ext_hum+1)
#define LOGACT_get_pm_1                       (LOGACT_get_int_pressure+1) 
#define LOGACT_get_pm_2_5                     (LOGACT_get_pm_1+1) 
#define LOGACT_get_pm_10                      (LOGACT_get_pm_2_5+1) 
#define LOGACT_get_o3_1_1                     (LOGACT_get_pm_10+1) 
#define LOGACT_get_o3_1_2                     (LOGACT_get_o3_1_1+1) 
#define LOGACT_get_o3_1_3                     (LOGACT_get_o3_1_2+1) 
#define LOGACT_get_o3_1_4                     (LOGACT_get_o3_1_3+1) 
#define LOGACT_get_o3_2_1                     (LOGACT_get_o3_1_4+1) 
#define LOGACT_get_o3_2_2                     (LOGACT_get_o3_2_1+1) 
#define LOGACT_get_o3_2_3                     (LOGACT_get_o3_2_2+1) 
#define LOGACT_get_o3_2_4                     (LOGACT_get_o3_2_3+1) 
#define LOGACT_get_o3_3_1                     (LOGACT_get_o3_2_4+1) 
#define LOGACT_get_o3_3_2                     (LOGACT_get_o3_3_1+1) 
#define LOGACT_get_o3_3_3                     (LOGACT_get_o3_3_2+1)
#define LOGACT_get_o3_3_4                     (LOGACT_get_o3_3_3+1)
#define LOGACT_get_acc_x                      (LOGACT_get_o3_3_4+1) 
#define LOGACT_get_acc_y                      (LOGACT_get_acc_x+1) 
#define LOGACT_get_acc_z                      (LOGACT_get_acc_y+1) 
#define LOGACT_get_mag_x                      (LOGACT_get_acc_z+1) 
#define LOGACT_get_mag_y                      (LOGACT_get_mag_x+1) 
#define LOGACT_get_mag_z                      (LOGACT_get_mag_y+1) 
#define LOGACT_get_gyro_x                     (LOGACT_get_mag_z+1) 
#define LOGACT_get_gyro_y                     (LOGACT_get_gyro_x+1) 
#define LOGACT_get_gyro_z                     (LOGACT_get_gyro_y+1) 
#define LOGACT_get_wind_speed                 (LOGACT_get_gyro_z+1) 
#define LOGACT_get_wind_direction             (LOGACT_get_wind_speed+1) 
#define LOGACT_get_rain_fall                  (LOGACT_get_wind_direction+1)
#define LOGACT_get_bat_mA                     (LOGACT_get_rain_fall+1)
#define LOGACT_get_bat_V                      (LOGACT_get_bat_mA+1)

#define LOGACT_error_init_AM2315              160
#define LOGACT_error_init_BME280              (LOGACT_error_init_AM2315+1)
#define LOGACT_error_init_OPCN3               (LOGACT_error_init_BME280+1)
#define LOGACT_error_init_ADS1115_1           (LOGACT_error_init_OPCN3+1)
#define LOGACT_error_init_ADS1115_2           (LOGACT_error_init_ADS1115_1+1)
#define LOGACT_error_init_ADS1115_3           (LOGACT_error_init_ADS1115_2+1)
#define LOGACT_error_init_DS3231              (LOGACT_error_init_ADS1115_3+1)
#define LOGACT_error_init_Sen0186             (LOGACT_error_init_DS3231+1)
#define LOGACT_error_init_LSM9DS0             (LOGACT_error_init_Sen0186+1)
#define LOGACT_error_init_INA219              (LOGACT_error_init_LSM9DS0+1)

#define LOGACT_error_read_AM2315              180 
#define LOGACT_error_read_BME280_temp         (LOGACT_error_read_AM2315+1)
#define LOGACT_error_read_BME280_hum          (LOGACT_error_read_BME280_temp+1)
#define LOGACT_error_read_BME280_pressure     (LOGACT_error_read_BME280_hum+1)    
#define LOGACT_error_read_OPCN3               (LOGACT_error_read_BME280_pressure+1)
#define LOGACT_error_read_ADS1115_1_1         (LOGACT_error_read_OPCN3+1) 
#define LOGACT_error_read_ADS1115_1_2         (LOGACT_error_read_ADS1115_1_1+1)
#define LOGACT_error_read_ADS1115_1_3         (LOGACT_error_read_ADS1115_1_2+1) 
#define LOGACT_error_read_ADS1115_1_4         (LOGACT_error_read_ADS1115_1_3+1) 
#define LOGACT_error_read_ADS1115_2_1         (LOGACT_error_read_ADS1115_1_4+1)
#define LOGACT_error_read_ADS1115_2_2         (LOGACT_error_read_ADS1115_2_1+1)
#define LOGACT_error_read_ADS1115_2_3         (LOGACT_error_read_ADS1115_2_2+1) 
#define LOGACT_error_read_ADS1115_2_4         (LOGACT_error_read_ADS1115_2_3+1)
#define LOGACT_error_read_ADS1115_3_1         (LOGACT_error_read_ADS1115_2_4+1)
#define LOGACT_error_read_ADS1115_3_2         (LOGACT_error_read_ADS1115_3_1+1) 
#define LOGACT_error_read_ADS1115_3_3         (LOGACT_error_read_ADS1115_3_2+1)
#define LOGACT_error_read_ADS1115_3_4         (LOGACT_error_read_ADS1115_3_3+1)
#define LOGACT_error_read_LSM9DS0             (LOGACT_error_read_ADS1115_3_4+1)
#define LOGACT_error_read_SEN0186			  (LOGACT_error_read_LSM9DS0+1)
#define LOGACT_error_read_wind_speed          (LOGACT_error_read_SEN0186+1)
#define LOGACT_error_read_wind_direction      (LOGACT_error_read_wind_speed+1)
#define LOGACT_error_read_rain_fall           (LOGACT_error_read_wind_direction+1)
#define LOGACT_error_read_bat_mA              (LOGACT_error_read_rain_fall+1)
#define LOGACT_error_read_bat_V               (LOGACT_error_read_bat_mA+1)

#define LOGACT_error_get_int_temp             210 
#define LOGACT_error_get_int_hum              (LOGACT_error_get_int_temp+1) 
#define LOGACT_error_get_ext_temp             (LOGACT_error_get_int_hum+1) 
#define LOGACT_error_get_ext_hum              (LOGACT_error_get_ext_temp+1) 
#define LOGACT_error_get_int_pressure         (LOGACT_error_get_ext_hum+1)
#define LOGACT_error_get_pm_1                 (LOGACT_error_get_int_pressure+1) 
#define LOGACT_error_get_pm_2_5               (LOGACT_error_get_pm_1+1) 
#define LOGACT_error_get_pm_10                (LOGACT_error_get_pm_2_5+1) 
#define LOGACT_error_get_o3_1_1               (LOGACT_error_get_pm_10+1) 
#define LOGACT_error_get_o3_1_2               (LOGACT_error_get_o3_1_1+1) 
#define LOGACT_error_get_o3_1_3               (LOGACT_error_get_o3_1_2+1) 
#define LOGACT_error_get_o3_1_4               (LOGACT_error_get_o3_1_3+1) 
#define LOGACT_error_get_o3_2_1               (LOGACT_error_get_o3_1_4+1) 
#define LOGACT_error_get_o3_2_2               (LOGACT_error_get_o3_2_1+1) 
#define LOGACT_error_get_o3_2_3               (LOGACT_error_get_o3_2_2+1) 
#define LOGACT_error_get_o3_2_4               (LOGACT_error_get_o3_2_3+1) 
#define LOGACT_error_get_o3_3_1               (LOGACT_error_get_o3_2_4+1) 
#define LOGACT_error_get_o3_3_2               (LOGACT_error_get_o3_3_1+1) 
#define LOGACT_error_get_o3_3_3               (LOGACT_error_get_o3_3_2+1)
#define LOGACT_error_get_o3_3_4               (LOGACT_error_get_o3_3_3+1)
#define LOGACT_error_get_acc_x                (LOGACT_error_get_o3_3_4+1) 
#define LOGACT_error_get_acc_y                (LOGACT_error_get_acc_x+1) 
#define LOGACT_error_get_acc_z                (LOGACT_error_get_acc_y+1) 
#define LOGACT_error_get_mag_x                (LOGACT_error_get_acc_z+1)
#define LOGACT_error_get_mag_y                (LOGACT_error_get_mag_x+1) 
#define LOGACT_error_get_mag_z                (LOGACT_error_get_mag_y+1) 
#define LOGACT_error_get_gyro_x               (LOGACT_error_get_mag_z+1) 
#define LOGACT_error_get_gyro_y               (LOGACT_error_get_gyro_x+1) 
#define LOGACT_error_get_gyro_z               (LOGACT_error_get_gyro_y+1) 
#define LOGACT_error_get_wind_speed           (LOGACT_error_get_gyro_z+1)
#define LOGACT_error_get_wind_direction       (LOGACT_error_get_wind_speed+1)
#define LOGACT_error_get_rain_fall            (LOGACT_error_get_wind_direction+1)
#define LOGACT_error_get_bat_mA               (LOGACT_error_get_rain_fall+1)
#define LOGACT_error_get_bat_V                (LOGACT_error_get_bat_mA+1)

#define LoRa_gateway false
#define LoRa_NetID 0x12

#define LoRa_time_ACK 4000
#define LoRa_time_RQST 10000

#define LoRa_time_MORE_DATA 15000
#define LoRa_time_NEXT_RQST 60000

#define LoRa_maxRQST 3
#define LoRa_maxRQST_MOREDATA 3

#define PKT_type_RQST 129
#define PKT_type_ACK 66
#define PKT_type_LET 36

#define INF_LET_1 3
#define INF_LET_2 12
#define INF_LET_3 48
#define INF_MORE_DATA 192

#define OFFSET_PKT_NEW (DIM_PKT_ADDR + DIM_PKT_TYPE + DIM_PKT_STATUS)
#define OFFSET_PKT_STATUS 2
#define OFFSET_PKT_TIME (OFFSET_PKT_STATUS + DIM_PKT_STATUS)
#define OFFSET_PKT_TEMPERATURE (OFFSET_PKT_TIME + DIM_PKT_TIME)
#define OFFSET_PKT_HUMIDITY (OFFSET_PKT_TEMPERATURE + DIM_PKT_TEMPERATURE)
#define OFFSET_PKT_PRESSURE (OFFSET_PKT_HUMIDITY + DIM_PKT_HUMIDITY)
#define OFFSET_PKT_PM (OFFSET_PKT_PRESSURE + DIM_PKT_PRESSURE)
#define OFFSET_PKT_OZONE (OFFSET_PKT_PM + DIM_PKT_PM)
#define OFFSET_PKT_ACCEL (OFFSET_PKT_OZONE + DIM_PKT_OZONE)
#define OFFSET_PKT_MAG (OFFSET_PKT_ACCEL + DIM_PKT_ACCEL)
#define OFFSET_PKT_GYRO (OFFSET_PKT_MAG + DIM_PKT_MAG)
#define OFFSET_PKT_WIND (OFFSET_PKT_GYRO + DIM_PKT_GYRO)
#define OFFSET_PKT_END (OFFSET_PKT_WIND + DIM_PKT_WINDRAIN)

#define TOT_LET_PER_PKT 2

#if TOT_LET_PER_PKT >= 3
#error packet can contain max 2 LET
#endif

#define SLEEP_ENABLE true
#define SENSOR_TIME 300
#define tot_TIME 1800
#define SLEEP_TIME (tot_TIME-3*SENSOR_TIME)/3
#define SERIAL_TIME 750
#define SLEEP_CYCLE_INCLUDED 4

#if SLEEP_TIME<SENSOR_TIME
#error SLEEP_TIME must be greater than SENSOR_TIME
#endif

#if (60*60*24)%tot_TIME!=0
#error a day must be divisible by tot_TIME
#endif

#endif
