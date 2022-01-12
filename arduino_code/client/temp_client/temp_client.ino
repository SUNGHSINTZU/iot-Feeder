// for 台灣大學講課 LoraWAN
#include <Wire.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// for  Dragino Shield + UNO
#define RFM95_CS 10           // SS pin D10
#define RFM95_RST 4
#define RFM95_INT 2            //(interrupt 0 pin D2)

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT); // RH_RF95 rf95(10, 2); // Dragino with RFM95
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0    //頻率915.0MHz

#define ONE_WIRE_BUS 5    // 告訴 OneWire library DQ 接在那隻腳上 
OneWire oneWire(ONE_WIRE_BUS); // 建立 OneWire 物件
DallasTemperature DS18B20(&oneWire); // 建立 DS18B20 物件

int counter=0;
//=========================
int delayTime=280;
int delayTime2=40;
float offTime=9680;
//=========================
void setup() {
   Serial.begin(115200);
   DS18B20.begin();
   if (!rf95.init())
   {
    Serial.println("init failed");
   }
}
void loop()
{
    float temperature; //注意，溫度讀值帶小數，要用 float

    DS18B20.requestTemperatures();  //下指令開始轉換
    
    temperature = DS18B20.getTempCByIndex(0);  //讀取第一顆 DS18B20 的溫度
    Serial.print("T-");
    Serial.println(temperature);
    delay(1000);
    
  //================
    Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  //================
      uint8_t data[48] ;
      String total_countString = String("")+ "T-"+ temperature ;
      for (int i=0 ; i< 48; i++)
           {
             data[i] = total_countString.charAt(i);
            }
  //==============
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();
    // Now wait for a reply
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.waitAvailableTimeout(3000))
    {  
      if (rf95.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  }
  delay(4000);
  counter++;
}
