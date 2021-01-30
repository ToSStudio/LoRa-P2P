// MX-tx.ino

// need to be set, if Serial is used with the intention to work with PC-USB
#define USBCON 

#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptHandler.h>

#include <RFTransmitter.h>

#define PIN_TX  11
#define PIN_LED 13

#define NODE_ID 1

// Send on digital pin 11 and identify as node 1
RFTransmitter transmitter( PIN_TX, NODE_ID );

int ledLevel = 1;

void setup() 
{
    // setup LED as output
    pinMode( PIN_LED, OUTPUT); 
    digitalWrite(13, ledLevel ? HIGH : LOW );  
}

int msgCounter = 1;

void loop() 
{ 
    // toggle LED value
    ledLevel = ledLevel ? 0 : 1;
    digitalWrite(13, ledLevel ? HIGH : LOW );  

    char msg[80];
    const int len = sprintf( msg, "msg#%d", msgCounter++ ) + 1;
    transmitter.send( (byte *)msg, len );

    delay(500);

    transmitter.resend( (byte *)msg, len );
}
