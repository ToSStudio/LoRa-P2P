// MX_rx.ino

// need to be set, if Serial is used with the intention to work with PC-USB
#define USBCON 

#include <PinChangeInterruptHandler.h>
#include <RFReceiver.h>

#define PIN_RX  2
#define PIN_LED 13

RFReceiver receiver( PIN_RX );

int ledLevel = 1;

void setup() 
{
    // setup LED as output
    pinMode( PIN_LED, OUTPUT); 
    digitalWrite(13, ledLevel ? HIGH : LOW );  

    // setup communication to PC 
    Serial.begin(9600);
    receiver.begin();
}

void loop() 
{
    // toggle LED value
    ledLevel = ledLevel ? 0 : 1;
    digitalWrite(13, ledLevel ? HIGH : LOW );  
  
    // try to receive transmission
    char msg[MAX_PACKAGE_SIZE];
    byte senderId = 0;
    byte packageId = 0;
    int len = (int) receiver.recvPackage((byte *)msg, &senderId, &packageId);
  
    Serial.println("");
    Serial.print("id: "); Serial.println(packageId);
    Serial.print("tx: "); Serial.println(senderId);
    Serial.print("ln: "); Serial.println(len);
    Serial.print("mx: "); Serial.println(msg);
}
