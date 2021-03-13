import serial

ser = serial.Serial('/dev/ttyACM0',115200)

with open('dataFile.txt','a') as f:
    while 1 :
        f.write(ser.readline())