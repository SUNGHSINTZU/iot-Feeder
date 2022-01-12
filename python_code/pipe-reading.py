import serial                                                                                          
import RPi.GPIO as GPIO                                                                                
import time                                                                                            

# LedPin = 11    # pin11                                                                                 
ser=serial.Serial("/dev/ttyACM0",115200)  #change ACM number as found from ls /dev/tty/ACM*              
ser.baudrate=115200                                                                                      

from multiprocessing import Process,Pipe
import os
from auto_capture_getarea_v2 import camera

main_conn , sub_conn = Pipe()    #使用Pipe()函數同時建立主進程及自進程兩個通信的物件
while True:                                                                                            
   serialmessage = ser.readline()                                                          
   print("serial message is " + serialmessage) 
   proc = Process(target=camera,args=(sub_conn,))
   proc.start()
   subconnmessage = main_conn.recv()
   print("sub connection message is "+ subconnmessage)

   if serialmessage[0] == "T":                                                                        
      # todo
      break
   if subconnmessage[0] == "A":
      #todo
      break

      
# def setup():                                                                                           
#    GPIO.setmode(GPIO.BOARD)       # Set the board mode to numbers pins by physical location            
#    GPIO.setup(LedPin, GPIO.OUT)   # Set pin mode as output                                             
#    GPIO.output(LedPin, GPIO.HIGH) # Set pin to high(+3.3V) to off the led                              

# def blink():                                                                                           
#            print ('led on')                                                                            
#            GPIO.output(LedPin, GPIO.LOW)   # led on                                                    
#            time.sleep(1.0)                 # wait 1 sec                                                
#            print ('led off')                                                                       
#            GPIO.output(LedPin, GPIO.HIGH)  # led off                                                   
#            time.sleep(1.0)                 # wait 1 sec                                                


# setup()