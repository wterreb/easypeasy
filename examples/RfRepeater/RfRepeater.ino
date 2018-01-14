#include <EasyPeasy.h>

// RfRepeater -  Example of how to implement a simple RF repeater that can be used to boost signal range.
//               Every time a valid radio signal is received it be re-transmitted.
//               If this repeater is placed midway between a transmitter and a receiver, then it can effectively double the range.
//               The repeater also shows some details of what is happening by changing the colour of the LED
//               Requires the EasyPeasy433 board 


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
        // Something was received, so display it and retransmit the same code again
        Serial.println(rxData, HEX);
        easy.SetLedColor(COLOR_GREEN, ledBrightness);
        delay(100);
        easy.SetLedColor(COLOR_BLUE, ledBrightness);
        easy.RfTransmitData(rxData);
        easy.SetLedColor(COLOR_RED, ledBrightness);
        delay(3000);  // Lay low for a while to avoid ping-pong effect if there is multiple repeaters in the area
        easy.SetLedColor(COLOR_OFF, ledBrightness);
     }
}



