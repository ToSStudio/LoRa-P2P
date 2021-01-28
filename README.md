# LoRa-P2P
experimenting with two SX1278 on 433 MHz. One is driven by an Arduino UNO. The Datalines are 5V-tolerant. The SX1278 can be supplied with 3.3 V from the UNO. It can deliver sufficient to run the Module at full power (130 mA).

Full power is rarely necessary due to the great link-budget LoRa does offer.

One SX1278 is connected to a SEEEDUINO XIAO which come natively with 3.3 V. Connections to the Arduino boards are in the programme_headers and:

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

