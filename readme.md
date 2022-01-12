## IOT PROJECT : Feeder

### 1. SETUP

#### a. Temperature sensor

##### DS18B20

* [DS18B20 spec](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf)

* [DS18B20 tutorial](https://shop.mirotek.com.tw/arduino/arduino-adv-1/)

1.  Connect [Dragino LoRa Shield](https://wiki.dragino.com/index.php?title=Lora_Shield#Pin_Mapping_and_Unused_Pins) to Arduino

2. connecting the wires, and make the data line connect to PIN5 (not PIN2!!)

![https://shop.mirotek.com.tw/wp-content/uploads/2021/03/A100013-2.png](https://shop.mirotek.com.tw/wp-content/uploads/2021/03/A100013-2.png)



3. open "arduino", then open file "temp_client"

4.  click "Tools"--> "Manage Libraries"

5. search for "onewire" and "dallastemperature" and click "install"
6. upload the code!

#### b. Camera

##### ESP32 CAM

* [ESP32CAM spec](https://loboris.eu/ESP32/ESP32-CAM%20Product%20Specification.pdf)
* [ESP32CAM tutorial](https://youtu.be/DdybJZ58mlI)
* wire connection

​	Arduino RESET --- Arduino GND

​	ESP32CAM GND --- Arduino GND

​	ESP32CAM 5V --- Arduino 5V

​	ESP32CAM U0R--- Arduino 0(RX)

​	ESP32CAM U0T --- Arduino 1(TX)

​	ESP32CAM IO0--- ESP32CAM GND

* Arduino IDE 

  [esp32camera document](https://github.com/espressif/esp32-camera)
  
  "Tools"--> 

<img src="C:\Users\glori\AppData\Roaming\Typora\typora-user-images\image-20220108115418903.png" alt="image-20220108115418903" style="zoom:80%;" />



**Reference**

(not used)Arduino : [auto take picture](https://gist.github.com/youjunjer/b20f63fcf909862c7943dd955583916e#file-gistfile1-txt) , [auto take pic blog](https://youyouyou.pixnet.net/blog/post/119645859)

(not used)MQTT: [AWS IoT | ESP32CAM working as a publisher on MQTT](https://www.youtube.com/watch?v=7_3qbou_keg) , [esp32cam w. mqtt](https://www.reddit.com/r/esp32/comments/ghvmmy/i_made_a_simple_esp32cam_fully_controlled_by_mqtt/)

OpenCV : [video capture tutorial](https://blog.gtwang.org/programming/opencv-webcam-video-capture-and-file-write-tutorial/), [video capture official tutorial](https://docs.opencv.org/4.x/dd/d43/tutorial_py_video_display.html)

#### c. Python Environment Setup (Windows)

use PYTHON and ANACONDA ENV!

* tutorial:  [anaconda virtual env ](https://medium.com/python4u/%E7%94%A8conda%E5%BB%BA%E7%AB%8B%E5%8F%8A%E7%AE%A1%E7%90%86python%E8%99%9B%E6%93%AC%E7%92%B0%E5%A2%83-b61fd2a76566) 

* In anaconda prompt: 

create and activate a virtual env

```
conda create --name [your env name] python=[your python version]
```

```
activate  [your env name]
```

check installed module in virtual env

```
conda e list
```

install python module (for example)

```
python -m pip install -U pip
pip install -U pip
pip install -U numpy 
```

deactivate
```
deactivate
```

#### d. Image Processing

##### Object detection (not used)

* [YOLOV4](https://pjreddie.com/darknet/yolo/) 

* [Training Custom Object Detector](https://tensorflow-object-detection-api-tutorial.readthedocs.io/en/latest/training.html)

  I0110 09:39:11.612815 15168 model_lib_v2.py:708] 

  {'Loss/classification_loss': 0.14004116,
   'Loss/localization_loss': 0.09383143,
   'Loss/regularization_loss': 0.2444374,
   'Loss/total_loss': 0.47831,
   'learning_rate': 0.014666351}

  * Tried but gave up, because it needs to label every pellet in each picture, and it needs GPU to accelerate the training, and it takes too much time to get a great model.

Reference

[VS Installation](https://www.scivision.dev/python-windows-visual-c-14-required)

##### OpenCV contour

* open a new terminal or anaconda prompt, install module:

``` 
pip install pyserial
pip install matplotlib
```

* ```cd``` to the directory of file "auto_capture_getarea.py", and run it with ``` python auto_capture_getarea.py```.

Reference

[opencv contour tutorial](https://docs.opencv.org/4.x/d4/d73/tutorial_py_contours_begin.html)

[finding-a-tight-contour-around-a-blurry-object-with-python](https://stackoverflow.com/questions/60642827/finding-a-tight-contour-around-a-blurry-object-with-python-opencv)

#### e. RPI setup

* install opencv and pyserial

```
sudo apt-get update && sudo apt-get upgrade
pip3 install numpy
pip3 install opencv-python
pip3 install pyinstaller
sudo apt-get install libatlas-base-dev
sudopip3 install pyserial
```
* install MQTT

```
sudo apt-get update
sudo apt-get install mosquitto
sudo apt-get install mosquitto-clients

sudo service mosquitto start
service mosquitto status # check status
mosquitto_sub -h 192.168.50.110 -d -t pond/temp0 # subscriber
mosquito_pub -h 192.168.50.110 -d -t pond/temp0 -m "T-18.17" # publisher 
# -h: rpi ip address, -d:debug message, -t topic, -m message
```
* show serial port data

```
dmesg | grep tty 
cat /dev/ttyACM0
stty -F /dev/ttyACM0 [baudrate] #指定serialport的baud rate
```
* install Node-red

```
sudo apt-get install npm
sudo npm install -g n
sudo npm install -g --unsafe-perm node-red
sudo apt-get update
sudo apt-get install npm
sudo n 12.13 # node v12
node-red # run node-red
```

Reference

[Building wheel for opencv-python (PEP 517) ... - runs forever](https://stackoverflow.com/questions/63669752/building-wheel-for-opencv-python-pep-517-runs-forever)

### 2. RUN CODE

first, ```git clone https://github.com/SUNGHSINTZU/iot-Feeder.git```

then run the code,

* Arduino client side:

  in ```\arduino_code\client\``` :

  upload the code in ```temp_client``` to Arduino+LoRa board

  upload the code in ```camera_client``` to Arduino board

* Arduino server side:

  in ```\arduino_code\server\``` :

  upload the code in ```rf95_server``` to Arduino+LoRa board

  upload the code in ```car_server``` to Arduino board

* RPi 

  always check the serial port name in Terminal by ```dmesg|gret tty``` before you run the python code. After port name like "/dev/ttyACM0" or "/dev/ttyUSB1" is confirmed, change them in ```pipe-reading.py```. Also, change the camera IP address in ```auto_capture_getarea_module.py```.
  
  ```
  cd \python_code
  python pipe-reading.py
  ```
  
  