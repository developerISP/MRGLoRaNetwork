from dataclasses import dataclass
from datetime import datetime
from typing import List
import re

@dataclass
class Reading():
    time: datetime
    temp_ext: float
    temp_int: float
    temp_ext_sd: float
    temp_int_sd: float
    rh_ext: float
    rh_int: float
    rh_ext_sd: float
    rh_int_sd: float
    PM1: float
    PM25: float
    PM10: float
    PM1_sd: float
    PM25_sd: float
    PM10_sd: float
    OZO_11: float
    OZO_12: float
    OZO_21: float
    OZO_22: float
    OZO_31: float
    OZO_32: float
    OZO_11_sd: float
    OZO_12_sd: float
    OZO_21_sd: float
    OZO_22_sd: float
    OZO_31_sd: float
    OZO_32_sd: float
    accX: float
    accY: float
    accZ: float
    accX_sd: float
    accY_sd: float
    accZ_sd: float
    magX: float
    magY: float
    magZ: float
    magX_sd: float
    magY_sd: float
    magZ_sd: float
    gyrX: float
    gyrY: float
    gyrZ: float
    gyrX_sd: float
    gyrY_sd: float
    gyrZ_sd: float
    windD: float
    windS: float
    windD_sd: float
    windS_sd: float

    # converts data to a CSV string
    def to_csv(self) -> str:
        # matches all numbers (both integers, float and in scientific notation (e.g 2e-7))
        # and strings (with "" or '' included)
        # preceded by an equal sign
        # (attributes are guaranteed to be ordered by their definition)
        res = re.findall("(?<==)((((\\+|-)?\\d*\\.?\\d*e?(\\+|-)?\d*)|((\"|')?[^\"\']*('|\")?)))", self.__repr__())

        # date and time are formatted separately,
        # in order to format it in dd/mm/yyyy hh:mm:ss
        date = f"{self.time.day}, {self.time.month}, {self.time.year}, {self.time.hour}, {self.time.minute}, {self.time.second}, "
        # returns the values separated by a comma
        return date + ', '.join([g[0] for g in res if g[0] != ''])

@dataclass
class StationData():
    time: datetime
    addr: str
    reads: int
    tension: float
    amps: float
    sn_lora: str
    rssi_lora: str

    def to_csv(self) -> str:
        return f"{self.addr}, {self.reads}, {self.tension}, {self.amps}, {self.sn_lora}, {self.rssi_lora}"

@dataclass
class Message():
    sd: StationData
    reads: List[Reading]

    def to_csv(self) -> str:
        res = "sp, "+ self.sd.to_csv()

        for i, r in enumerate(self.reads):
            res += ", sr" + str(i) + ", "
            res += r.to_csv()
            res += ", er" + str(i)

        res += ", ep"
        return res
        
    # # fp = open("msg.txt", "r").read().split('\n')
