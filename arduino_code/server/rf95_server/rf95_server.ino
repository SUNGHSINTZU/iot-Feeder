#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;

void setup()
{
  Serial.begin(115200);
  while (!Serial) ;
  if (!rf95.init())
    Serial.println("init failed");
}
void loop()
{
  if (rf95.available())
  {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      Serial.println((char*)buf);

      // Send a reply
      uint8_t data[] = "server got data";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}
