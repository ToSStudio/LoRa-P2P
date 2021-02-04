# Readme #

Using a HELTEC Module with the ESP32 processor with integrated WiFi / Bluetooth and an onboard SX1278 LoRa-Module.\
This program does not make use of an external library for the LoRa-Functions.\
It does use the internal SPI-Bus and Library.\
The OLED is accessed through the famous U8x8 Library.\
I am using V1.0 here. If possible, get a V2.1 Module. It comes with slight improvements; is pin-compatible though.

## Onboard LED ##
There is one white LED onboard which can be accessed through PIN25. A second LED (orange) is attached to the LiIon / LiPO charging unit and cannot be controlled by the µController. 

It does
* blink when powered
* remain on solid while charging
* stay off when fully charged

Wihtout a battery connected the orange LED will flicker permanently. To get rid of that: attach a battery, desolder the LED or put the module behind a masking face.

- - - -

- [ ] softwire PA_Output
- [ ] write Heltec-Rx w/o relying on external lib
