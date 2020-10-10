#!/usr/bin/env python
import time, os, sys
import textparser
import csv
import numpy as np
from textparser import Sequence
from datetime import datetime
import serial

ser = serial.Serial(
        port='/dev/ttyS0',
        baudrate = 9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)

###
### LA STRUTTURA DEL MESSAGGIO
###
# 0 - sp
# 1 - l'indirizzo dell'endnode che ha inviato il pacchetto, 
# 2 - il numero di letture, 
# 3 - il voltaggio della battiera, 
# 4 - la corrente della batteria, 
# 5 - il S/N di LoRa e 
# 6 - RSSI di LoRa.  
# 7 - sr1 # START DEL PRIMO MESSAGGIO
# 8 - sr1_day
# 9 - sr1_month
#10 - sr1_year
#11 - sr1_hour
#12 - sr1_minute
#13 - sr1_second
#14 - sr1_temp_ext
#15 - sr1_temp_int
#16 - sr1_temp_ext_sd
#17 - sr1_temp_int_sd
#18 - sr1_rh_ext
#19 - sr1_rh_int
#20 - sr1_rh_ext_sd
#21 - sr1_rh_int_sd
#22 - sr1_PM1
#23 - sr1_PM25
#24 - sr1_PM10
#25 - sr1_PM1_sd
#26 - sr1_PM25_sd
#27 - sr1_PM10_sd
#28 - sr1_OZO_11
#29 - sr1_OZO_12
#30 - sr1_OZO_21
#31 - sr1_OZO_22
#32 - sr1_OZO_31
#33 - sr1_OZO_32
#34 - sr1_OZO_11_sd
#35 - sr1_OZO_12_sd
#36 - sr1_OZO_21_sd
#37 - sr1_OZO_22_sd
#38 - sr1_OZO_31_sd
#39 - sr1_OZO_32_sd
#40 - sr1_accX
#41 - sr1_accY
#42 - sr1_accZ
#43 - sr1_accX_sd
#44 - sr1_accY_sd
#45 - sr1_accZ_sd
#46 - sr1_magX
#47 - sr1_magY
#48 - sr1_magZ
#49 - sr1_magX_sd
#50 - sr1_magY_sd
#51 - sr1_magZ_sd
#52 - sr1_gyrX
#53 - sr1_gyrY
#54 - sr1_gyrZ
#55 - sr1_gyrX_sd
#56 - sr1_gyrY_sd
#57 - sr1_gyrZ_sd
#58 - sr1_windD
#59 - sr1_windS
#60 - sr1_windD_sd
#61 - sr1_windS_sd
#62 - er1
#...  per sr2 ... er2
#119- ep

# lista delle variabili chiave per il parse:

keywords=['sp', 'sr1', 'er1', 'sr2', 'er2', 'ep']

header=['addr', 'nl', 'volt', 'amp', 's_n', 'RSSI']

variables=['sp', 'addr', 'nl', 'volt', 'amp', 's_n', 'RSSI', 'sr1', 'sr1_day',
        'sr1_month', 'sr1_year', 'sr1_hour', 'sr1_minute', 'sr1_second',
        'sr1_temp_ext', 'sr1_temp_int', 'sr1_temp_ext_sd', 'sr1_temp_int_sd',
        'sr1_rh_ext', 'sr1_rh_int', 'sr1_rh_ext_sd', 'sr1_rh_int_sd', 'sr1_PM1',
        'sr1_PM25', 'sr1_PM10', 'sr1_PM1_sd', 'sr1_PM25_sd', 'sr1_PM10_sd',
        'sr1_OZO_11', 'sr1_OZO_12', 'sr1_OZO_21', 'sr1_OZO_22', 'sr1_OZO_31',
        'sr1_OZO_32', 'sr1_OZO_11_sd', 'sr1_OZO_12_sd', 'sr1_OZO_21_sd',
        'sr1_OZO_22_sd', 'sr1_OZO_31_sd', 'sr1_OZO_32_sd', 'sr1_accX',
        'sr1_accY', 'sr1_accZ', 'sr1_accX_sd', 'sr1_accY_sd', 'sr1_accZ_sd',
        'sr1_magX', 'sr1_magY', 'sr1_magZ', 'sr1_magX_sd', 'sr1_magY_sd',
        'sr1_magZ_sd', 'sr1_gyrX', 'sr1_gyrY', 'sr1_gyrZ', 'sr1_gyrX_sd',
        'sr1_gyrY_sd', 'sr1_gyrZ_sd', 'sr1_windD', 'sr1_windS', 'sr1_windD_sd',
        'sr1_windS_sd', 'er1', 'sr2', 'sr2_day', 'sr2_month', 'sr2_year',
        'sr2_hour', 'sr2_minute', 'sr2_second', 'sr2_temp_ext', 'sr2_temp_int',
        'sr2_temp_ext_sd', 'sr2_temp_int_sd', 'sr2_rh_ext', 'sr2_rh_int',
        'sr2_rh_ext_sd', 'sr2_rh_int_sd', 'sr2_PM1', 'sr2_PM25', 'sr2_PM10',
        'sr2_PM1_sd', 'sr2_PM25_sd', 'sr2_PM10_sd', 'sr2_OZO_11', 'sr2_OZO_12',
        'sr2_OZO_21', 'sr2_OZO_22', 'sr2_OZO_31', 'sr2_OZO_32', 'sr2_OZO_11_sd',
        'sr2_OZO_12_sd', 'sr2_OZO_21_sd', 'sr2_OZO_22_sd', 'sr2_OZO_31_sd',
        'sr2_OZO_32_sd', 'sr2_accX', 'sr2_accY', 'sr2_accZ', 'sr2_accX_sd',
        'sr2_accY_sd', 'sr2_accZ_sd', 'sr2_magX', 'sr2_magY', 'sr2_magZ',
        'sr2_magX_sd', 'sr2_magY_sd', 'sr2_magZ_sd', 'sr2_gyrX', 'sr2_gyrY',
        'sr2_gyrZ', 'sr2_gyrX_sd', 'sr2_gyrY_sd', 'sr2_gyrZ_sd', 'sr2_windD',
        'sr2_windS', 'sr2_windD_sd', 'sr2_windS_sd', 'er2', 'ep' ]

f = open('dataFile.txt','a')

while 1:
        f.write(ser.readline())
        f.close()
        #
        fp=open("dataFile.txt", "r").read().split('\n')
        fp = fp[:-1]
        #
        message=[] 
        #
        for i in range(len(fp)): 
            message.append(fp[i].replace("\\n",""))
        
        # scriviamo i valori in un file di backup giornaliero
        dailyBkFile = datetime.now().strftime("%Y%m%d") + 'MrgRosyGatewayAll' + '.txt'

        with open(dailyBkFile, "a+") as myfile:
            myfile.seek(0)
            data = myfile.read(100)
            if len(data) > 0:
                myfile.write('\n')
            # Append new line
            myfile.write(', '.join(message))
        myfile.close()
        open('dataFile.txt', 'w').close()
        f = open('dataFile.txt','a')
        
        

# # fp = open("msg.txt", "r").read().split('\n')
# # fp = fp[:-1]
# 
# fp = x
# 
# message=[]
# 
# for i in range(len(fp)): 
#     message.append(fp[i].replace("\\n",""))
# 
# # scriviamo i valori in un file di backup giornaliero
# dailyBkFile = datetime.now().strftime("%Y%m%d") + 'MrgRosyGatewayAll' + '.txt'
# 
# with open(dailyBkFile, "a+") as myfile:
#     myfile.seek(0)
#     data = myfile.read(100)
#     if len(data) > 0:
#         myfile.write('\n')
#     # Append new line
#     myfile.write(', '.join(message))
# 
# myfile.close()
