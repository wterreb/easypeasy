// SimpleRfReceiver -  Example of how to receive a radio signal via the onboard 433 Mhz transmitter
//                     Every time a valid radio signal is received it will be displayd in hexadecimal format and
//                     at the same time also switch on a green LED and emit a short blip on the onboard buzzer
//                     Requires an EasyPeasy433 board 
#include <EasyPeasy.h>

EasyPeasy easy = EasyPeasy();

void setup() {
     Serial.begin(115200);
     easy.begin();
}

void loop() {
     int ledBrightness = 20;
     uint32_t rxData = 0;

     if (easy.RfReceiveData(&rxData, 1000))    // Monitor the radio receiver for 1000 milliseconds for incoming data
     {
        // Something was received, so display it and make some sound and light
        Serial.println(rxData, HEX);
        easy.BuzzerOn(); 
        easy.SetLedColor(COLOR_GREEN, ledBrightness);
        delay(100);
        easy.BuzzerOff();
        easy.SetLedColor(COLOR_OFF, ledBrightness);
     }
}



