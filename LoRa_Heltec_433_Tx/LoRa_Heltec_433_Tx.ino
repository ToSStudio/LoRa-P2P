// I N C L U D E

#include <U8x8lib.h>                // Lib for onboard OLED.Display
#include "BluetoothSerial.h"        // Lib for onboard Bluetooth-tranceiver  [wieso muss das in "" stehen?]
#include <SPI.h>                    // Lib to make use of the SPI--Bus

// H A R D W A R E

/*--------------------------\
| SX1276 (pin) – ESP32 (pin)|
| SCK - GPIO5               |
| MISO - GPIO19             |
| MOSI - GPIO27             |
| CS - GPIO18 (Slave Select)|
| RESET - GPIO14            |
| DIO0 (8) – GPIO26 (15)    |
| DIO1 (9) – GPIO33 (13)    |
| DIO2 (10) – GPIO32 (12)   |
\------------------------- */

const byte lora_PIN_SCK = 5;
const byte lora_PIN_MISO = 19;
const byte lora_PIN_MOSI = 27;
const byte lora_PIN_SS = 18;
const byte lora_PIN_RESET = 14;
const byte lora_PIN_DIO0 = 15;
const byte lora_PIN_DIO1 = 13;
const byte lora_PIN_DIO2 = 12;

/*-----------------------------\
| onboard-LED (white): PIN25   |
\---------------------------- */

const byte pinLED = 25;

// Lora-Register names:
const byte lora_RegFifo = 0x00;
const byte lora_RegOpMode = 0x01;
const byte lora_RegFreqMsb = 0x06;
const byte lora_RegFreqMid = 0x07;
const byte lora_RegFreqLsb = 0x08;
const byte lora_RegPaConfig = 0x09;
const byte lora_RegOcp = 0x0b;
const byte lora_RegLna = 0x0c;
const byte lora_RegModemStat = 0x18;
const byte lora_RegPktSnr = 0x19;              // SNR of last received Packet
const byte lora_RegPktRssi = 0x1a;             // RSSI of last received Packet
const byte lora_RegRssi = 0x1b;                // actual RSSI
const byte lora_RegModemConfig1 = 0x1D;        // default: ExplicitHeaderMode | CodingRate=4/5 | Signal Bandwidth=125kHz
const byte lora_RegModemConfig2 = 0x1E;        // default: Spreading Factor=128chips/symbol | TxContinuous=off | RXPayLoadCRC=disable
const byte lora_RegModemConfig3 = 0x26;        // LowDataRateOptimize, AgcAutoOn
const byte lora_RegPayloadLength = 0x22;       // [I guess this is for Tx and req'ed only in Implicit Header Mode]
const byte lora_RegFifoRxByteAddr = 0x25;      // address of last byte written by LoRa-Receiver
const byte lora_RegFifoAddrPtr = 0x0D;         // default: 0x00
const byte lora_RegFifoTxBaseAddr = 0x0e;      // default: 0x80
const byte lora_RegFifoRxBaseAddr = 0x0f;      // default: 0x00
const byte lora_RegFifoRxCurrentAddr = 0x10;   // start address of last packet received
const byte lora_RegIrqFlags = 0x12;
const byte lora_RegRxBytes = 0x13;             // number of payload bytes of latest received packet


// Initialisations

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16); 	      // initialize OLED-Display on HelTec ESP32 (has OLED | LORA | WiFi | Bluetooth)
BluetoothSerial SerBT;                                    // setup Serial via Bluetooth as "SerBT" [wieso darf das nicht in setup stehen ?]

// ========= Functions: ===========

void lora_Reset()                                // reset all the module-register to default values
{
  digitalWrite(lora_PIN_RESET, LOW);
  delay (20);
  digitalWrite(lora_PIN_RESET, HIGH);
}

void lora_Write(byte lora_Reg, byte lora_Data)   // send a byte-value to a given Lora-register
{
  digitalWrite(lora_PIN_SS, LOW);
  SPI.transfer(lora_Reg | 0x80);
  SPI.transfer(lora_Data);
  digitalWrite(lora_PIN_SS, HIGH);
}

byte lora_Read(byte lora_Reg)                    // read the value of a given Lora-register
{
  byte lora_Data;
  digitalWrite(lora_PIN_SS, LOW);
  SPI.transfer(lora_Reg & 0x7f);
  lora_Data = SPI.transfer(0);                   // [wieso muss ein "0" in den Klammern stehen ?]
  digitalWrite (lora_PIN_SS, HIGH);
  return lora_Data;
}

void lora_SetFreq()
{
  lora_Write(lora_RegFreqMsb, 0x6c);             // ≙ 868.100 MHz (D9 06 66)
  lora_Write(lora_RegFreqMid, 0x6c);             // ≙ 433.700 MHz (6C 6C CC)
  lora_Write(lora_RegFreqLsb, 0xcc);
}

void lora_Setup()                                // set OpMode LORA, continuous rx
{
  lora_Write(lora_RegOpMode, 0x80);              // Sleepmode. necessary before modifying LoRa-mode | LowFrequencyMode
  delay(20);
  lora_Write(lora_RegOpMode, 0x85);              // Bit7 | Bit 0, 2 = LoRamode | RXContinuous
}

void lora_Config()
{
  lora_Write(lora_RegModemConfig1, 104); // BW 62,5 khz |  Coding rate: 4/8  | Explicit mode
  lora_Write(lora_RegModemConfig2, 192); // SF = 12 (4096 Chips / Symbol)
  lora_Write(lora_RegPaConfig, 2);       // set Tx-Power to minimum
  lora_Write(lora_RegOcp, 26);           // Over Current Protection on 
  lora_Write(lora_RegLna, 192);          // LNA minimum gain
}

void lora_tx(byte tx_Data)
{
  lora_Write(lora_RegOpMode, 0x81);               // Bit 7 | Bit 1 = LoRaMode | STDBY req'd mode to fill FIFO
  lora_Write(lora_RegFifoTxBaseAddr, 0x80);
  lora_Write(lora_RegFifoAddrPtr, 0x80);
  // lora_Write(lora_RegFifo, 68);
  // lora_Write(lora_RegFifo, 58);
  // lora_Write(lora_RegFifo, 32);
  lora_Write(lora_RegFifo, tx_Data);
  // lora_Write(lora_RegFifo, 32);
  lora_Write(lora_RegFifo, 32);
  lora_Write(lora_RegFifo, (tx_Data-1));
  lora_Write(lora_RegPayloadLength, 3);            // not always req'd -- but better to have 
  delay(5);
  lora_Write(lora_RegOpMode, 0x83);                // Tx-Mode
  // lora_Write(lora_RegOpMode, 8);                   // sleep mode !! does not work !!
}

// ======= end Functions =======

void setup(void)
{
  pinMode(pinLED, OUTPUT);                   // Onboard White LED
  pinMode(lora_PIN_SS, OUTPUT);              // Slave Select for onboard LORA_Module
  Serial.begin(9600);                        // start seriell-over-USB
  SPI.begin();                               // start SPI
  SerBT.begin("ToS_LORA_Modul");             // attach name to the onboard Bluetooth Module
  u8x8.begin();                              // start the OLED-Driver
  u8x8.setPowerSave(false);                  // wake up the OLED
  u8x8.setFont(u8x8_font_chroma48medium8_r); // use u8x8 Font for OLED
  u8x8.drawString(4, 0, "LORA-Tx");  // write header to OLED once
  u8x8.drawString(0, 2, "QRG:");             // write header to OLED once
  u8x8.drawString(0, 4, "Wert:");       
  digitalWrite(lora_PIN_SS, HIGH);           // unselect SPI device
  lora_Reset();
  lora_Setup();
  lora_Config();                             // set SF, BW,
  lora_SetFreq();
}

void loop(void)
{
  u8x8.setCursor(4, 2);
  u8x8.print((((lora_Read(lora_RegFreqMsb) * 65536) + (lora_Read(lora_RegFreqMid) * 256) + lora_Read(lora_RegFreqLsb)) * 61.03515625) / 1000000, 3);
  u8x8.print(" kHz");
  delay(500);
  for (byte tx_byte = 65; tx_byte < 91; tx_byte++) {
    lora_tx(tx_byte);
    digitalWrite (pinLED, HIGH);
    delay(270);
    u8x8.setCursor(6, 4);
    u8x8.print (tx_byte);
    digitalWrite (pinLED, LOW);
    delay (2500);
  }
}
