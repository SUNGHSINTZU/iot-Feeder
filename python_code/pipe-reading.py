import serial                                                                                          
import RPi.GPIO as GPIO                                                                                
import time                                                                                            
from multiprocessing import Process,Pipe
import os
from auto_capture_getarea_module import camera

# RPi Serial port                                                         
ser=serial.Serial("/dev/ttyACM0",115200)           
ser.baudrate=115200                                                                                      
# RPi Serial port to controller arduino 
arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=.3)
arduino.flush()
# recieve data of another python program
main_conn , sub_conn = Pipe()  

PIC_PERIOD=10.0
START_TIME = time.time()
count = 0

def write_read(x):
    writebyte = bytes(x, 'utf-8')
    arduino.write(writebyte)
#     time.sleep(0.05)
#     data = arduino.readline().decode('utf-8').rstrip()
#     return data


while True:
        delay = PIC_PERIOD - ((time.time() - START_TIME) % PIC_PERIOD)
        if delay > PIC_PERIOD-0.1:
                print("-------------")
                print("count",count)
                
                # grep serial data (temperature)
                serialmessage = ser.readline()
                serialmessage = serialmessage[:7]
                serialmessage = serialmessage.decode('utf-8')
        
                # grep python sub connection data (area)
                proc = Process(target=camera,args=(sub_conn,))
                proc.start()
                subconnmessage = main_conn.recv()
                writemessage = serialmessage + ", "+ subconnmessage
                write_read(writemessage)
                print("write message is " + writemessage)
                print("read message is ")
                count += 1

        data = arduino.readline().decode('utf-8').rstrip()
        if data != "": 
                print(data)
