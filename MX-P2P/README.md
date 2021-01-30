# Introduction

The modules `MX-05V` (tx) and `MX-FS-03V` (rx) are simple 433 Mhz transmission modules in range up to 30m.

## Libraries

Please install the listed libraries below. Download ZIP file of each library and add them to the libraries as described below

Download ZIP files | Content
-------------------|-----------
[RFTransmitter](https://github.com/zeitgeist87/RFTransmitter) | Transmitter Code
[RFReceiver](https://github.com/zeitgeist87/RFReceiver) | Receiver Code
[PinChangeInterruptHandler](https://github.com/zeitgeist87/PinChangeInterruptHandler) | Handler for Interrupts

Please use "Add Zip File Library..." to add the downloaded files to the project.

## Patches

The following patches are needed to have no warning and less error.  
(Both are not mandatory for the function used; but may cause problems in the future.)

### Remove Blocker

Patch | Value
------|------
`Category` | blocker : **remove code stall**
`File:Line` | Arduino\libraries\RFTransmitter-master\RFTransmitter.h:136
`Original` | for (byte i = sizeof(printBuf) - 1; i >= 0; --i, ++len) {
`Patched`|  for (int i = sizeof(printBuf) - 1; i >= 0; --i, ++len) {

### Remove Warning

Patch | Value
------|------
`Category` | cosmetic : removes warning
`File:Line` | Arduino\libraries\RFReceiver-master\RFReceiver.cpp:75
`Original` | void RFReceiver::handlePCInterrupt(int8_t pcIntNum, bool state) {
`Patched`|  void RFReceiver::handlePCInterrupt(int8_t /*pcIntNum*/, bool /*state*/) {
