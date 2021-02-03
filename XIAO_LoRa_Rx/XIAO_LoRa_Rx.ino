#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <U8x8lib.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

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
  u8x8.begin();
  u8x8.setPowerSave(0);
  delay(100);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(4, 0, "LoRa-Rx");
  LoRa.setPins(1, 3, 2);
  if (!LoRa.begin(433700000)) {     // 433.775 kHz
    u8x8.drawString(2, 5, "LoRa Module Fail !");
    while (1);
  }
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125000);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(0x12);
}

void loop() {
  u8x8.setCursor(0, 2);
  u8x8.print("Wert: ");
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    while (LoRa.available()) {
      u8x8.write((char)LoRa.read());
    }
    u8x8.setCursor(0, 4);
    u8x8.print("RSSI: ");
    u8x8.print(LoRa.packetRssi());
    u8x8.setCursor(0, 6);
    u8x8.print("SNR: ");
    u8x8.print(LoRa.packetSnr());
  }
}
