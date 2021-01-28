#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>

int counter = 0;

/*----------------
   LoRa         UNO         XIAO
   3,3 V ...... 3,3 V  ....  3,3 V
   Gnd   ...... Gnd    ....  Gnd
   NSS   ...... D10    ....  D1
   DIO0  ...... D2     ....  D2
   SCK   ...... D13    ....  SCK
   MISO  ...... D12    ....  MISO
   MOSI  ...... D11    ....  MOSI
   RST   ...... D9     ....  D3
*/

void setup(void)
{
  Serial.begin(9600);
  Serial.println("LoRa Tx:");
  if (!LoRa.begin(433700000)) {     // 433775 kHz
    Serial.println("LoRa Init failed !");
    while (1);
  }
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  LoRa.setCodingRate4(8);
  LoRa.setSyncWord(0x12);
  LoRa.setTxPower(20);
}

void loop()
{
  Serial.print("Packet Tx: ");
  Serial.println(counter);
  LoRa.beginPacket();
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  delay(2000);
}
