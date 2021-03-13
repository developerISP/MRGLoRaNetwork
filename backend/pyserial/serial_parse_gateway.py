#!/usr/bin/env python3
import serial
from datetime import datetime
from dataclasses import dataclass
import schedule
from ..db_handler.setup import SetupManager as sm
from ..dataclasses import *

ser = serial.Serial(
    port='/dev/ttyS0',
    baudrate = 115200,
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
# 3 - il voltaggio della batteria, 
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

daily_backup = open(datetime.now().strftime("%Y%m%d") + 'MrgRosyGatewayAll' + '.txt', "a")
def update_file():
    daily_backup.flush()
    daily_backup.close()
    daily_backup = open(datetime.now().strftime("%Y%m%d") + 'MrgRosyGatewayAll' + '.txt', "a")

schedule.every().day.at("00:00:00").do(update_file)

schedule.run_all()

"""with open('dataFile.txt','a') as f:
    while 1:
            data = ser.readline()
            f.write(ser.readline())
            f.flush()
            #
            fp=open("dataFile.txt", "r").read().split('\n')
            fp = fp[:-1]
            #
            message=[] 
            #
            for i in range(len(fp)): 
                message.append(fp[i].replace("\n",""))
            
            # scriviamo i valori in un file di backup giornaliero
            dailyBkFile = datetime.now().strftime("%Y%m%d") + 'MrgRosyGatewayAll' + '.txt'

            with open(dailyBkFile, "a+") as myfile:
                myfile.seek(0)
                data = myfile.read(100)
                if len(data) > 0:
                    myfile.write('\n')
                # Append new line
                myfile.write(', '.join(message))
"""

def parse_data() -> Message:
    if (ser.readline()) == "sp\n":
        # Headers
        sd = StationData(
            time = datetime.now(),
            addr = ser.readline(),
            reads = int(ser.readline()),
            tension = float(ser.readline()),
            amps = float(ser.readline()),
            sn_lora = ser.readline(),
            rssi_lora = ser.readline()
        )

        # Actual data
        reads = []
        for r in range(sd.reads):
            # Start read
            day = int(ser.readline())
            month = int(ser.readline())
            year = int(ser.readline())
            hour = int(ser.readline())
            minute = int(ser.readline())
            second = int(ser.readline())

            ser.readline()
            reads.append(Reading(
                # sd = standard deviation
                # Date and time of this reading
                time = datetime(year, month, day, hour, minute, second),

                # Internal and external temperature
                temp_ext = float(ser.readline()), 
                temp_int = float(ser.readline()),
                temp_ext_sd = int(ser.readline()),
                temp_int_sd = int(ser.readline()),

                # Humidity data
                rh_ext = float(ser.readline()),
                rh_int = float(ser.readline()),
                rh_ext_sd = float(ser.readline()),
                rh_int_sd = float(ser.readline()),

                # PM data
                PM1 = float(ser.readline()),
                PM25 = float(ser.readline()),
                PM10 = float(ser.readline()),
                PM1_sd = float(ser.readline()),
                PM25_sd = float(ser.readline()),
                PM10_sd = float(ser.readline()),

                # Ozone data
                OZO_11 = float(ser.readline()),
                OZO_12 = float(ser.readline()),
                OZO_21 = float(ser.readline()),
                OZO_22 = float(ser.readline()),
                OZO_31 = float(ser.readline()),
                OZO_32 = float(ser.readline()),
                OZO_11_sd = float(ser.readline()),
                OZO_12_sd = float(ser.readline()),
                OZO_21_sd = float(ser.readline()),
                OZO_22_sd = float(ser.readline()),
                OZO_31_sd = float(ser.readline()),
                OZO_32_sd = float(ser.readline()),

                # Accelerometer
                accX = float(ser.readline()),
                accY = float(ser.readline()),
                accZ = float(ser.readline()),
                accX_sd = float(ser.readline()),
                accY_sd = float(ser.readline()),
                accZ_sd = float(ser.readline()),

                # Mag data
                magX = float(ser.readline()),
                magY = float(ser.readline()),
                magZ = float(ser.readline()),
                magX_sd = float(ser.readline()),
                magY_sd = float(ser.readline()),
                magZ_sd = float(ser.readline()),

                # Gyroscope
                gyrX = float(ser.readline()),
                gyrY = float(ser.readline()),
                gyrZ = float(ser.readline()),
                gyrX_sd = float(ser.readline()),
                gyrY_sd = float(ser.readline()),
                gyrZ_sd = float(ser.readline()),

                # Wind data
                windD = float(ser.readline()),
                windS = float(ser.readline()),
                windD_sd = float(ser.readline()),
                windS_sd = float(ser.readline())
            ))
            
            # end of this read
            ser.readline()

        # end message
        ser.readline()
        
        return Message(sd, reads)
    else: 
        return

if __name__ == "__main__":
    db = sm(path="backend/pyserial/db.conf").setupDatabase()
    while True:
        with open("dataFile.txt", "a") as f:
            # Read and parse data
            data = parse_data()

            # Writes data to main and backup file
            f.writelines(data.to_csv())
            daily_backup.writelines(data.to_csv())

            # Loads data in the database
            db.addRow("stations_data", data.sd.__dict__)

            for i, r in enumerate(data.reads):
                row = {"addr": data.sd.addr, "read_number": i + 1, **r.__dict__}
                db.addRow("sensors_data", row)
