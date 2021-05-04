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
#define EEPROM_TIME_WRITE 15
#define EEPROM_TIME_READ 5
#define EEPROM_MAX_FAILED_READ 5
#define EEPROM_MAX_FAILED_WRITE 5

#define DIM_EEPROM_BUFF (DIM_PKT_TIME + DIM_PKT_TEMPERATURE + DIM_PKT_HUMIDITY + DIM_PKT_PRESSURE + DIM_PKT_PM+DIM_PKT_OZONE + DIM_PKT_ACCEL + DIM_PKT_MAG + DIM_PKT_GYRO + DIM_PKT_WINDRAIN + 1) 
#define QUEUE_EEPROM_TOT_DATA 6
#define QUEUE_EEPROM_OFFSET 0

#define DIM_EEPROM_LOG 6
#define LOG_EEPROM_TOT_DATA 40
#define LOG_EEPROM_OFFSET 0

#define DIM_EEPROM_INF 13
#define INF_EEPROM_TOT_DATA 40
#define INF_EEPROM_OFFSET 0

#define LOGACT_setup                0
#define LOGACT_let_temp             1
#define LOGACT_let_hum              2
#define LOGACT_let_pm               3
#define LOGACT_let_o3               4
#define LOGACT_let_acc              5
#define LOGACT_let_mag              6
#define LOGACT_let_gyro             7
#define LOGACT_let_wind             8
#define LOGACT_let_bat              9
#define LOGACT_sleep               11
#define LOGACT_lora_rx             12
#define LOGACT_lora_tx             13
#define LOGACT_lora_ack            14
#define LOGACT_lora_rqst           15
#define LOGACT_eeprom_r            16
#define LOGACT_eeprom_w            17
#define LOGACT_buff_temp           18
#define LOGACT_buff_hum            19
#define LOGACT_buff_pm             20
#define LOGACT_buff_o3             21
#define LOGACT_buff_acc            22
#define LOGACT_buff_mag            23
#define LOGACT_buff_gyro           24
#define LOGACT_buff_wind           25
#define LOGACT_eeprom_buff_app     26
#define LOGACT_eeprom_buff_del     27
#define LOGACT_eeprom_buff_get     28
#define LOGACT_eeprom_inf_app      29
#define LOGACT_eeprom_inf_del      30
#define LOGACT_eeprom_inf_get      31
#define LOGACT_lora_status         32
#define LOGACT_lora_let1           33
#define LOGACT_lora_let2           34
#define LOGACT_lora_let3           35

#define LoRa_gateway true
#define LoRa_NetID 0x12

#define LoRa_time_ACK 4000
#define LoRa_time_RQST 10000

#define LoRa_time_MORE_DATA 15
#define LoRa_time_NEXT_RQST 60

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
#define SERIAL_TIME 500
#define SLEEP_CYCLE_INCLUDED 4

#if SLEEP_TIME<SENSOR_TIME
#error SLEEP_TIME must be greater than SENSOR_TIME
#endif

#if (60*60*24)%tot_TIME!=0
#error a day must be divisible by tot_TIME
#endif

#endif