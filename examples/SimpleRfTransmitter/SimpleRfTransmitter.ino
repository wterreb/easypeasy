#include <EasyPeasy.h>

// SimpleRfTx -  Example of how transmit a radio signal via the onboard 433 Mhz transmitter
//               While the push button is pressed the board will repeatedly transmit a 32 bit number while
//               at the same time also switch on a blue LED and a short blip on the onboard buzzer
//               Requires the EasyPeasy433 board 


EasyPeasy easy = EasyPeasy();

void setup() {
     Serial.begin(115200);
     easy.begin();
}

void loop() {
     int ledBrightness = 20;
     
     if (easy.IsPushButtonPressed()) 
     {
        easy.BuzzerOn(); 
        easy.SetLedColor(COLOR_BLUE, ledBrightness);
        easy.RfTransmitData(0xABCDEF);    // Transmit a dummy byte via the RF transmitter
        easy.BuzzerOff();
        easy.SetLedColor(COLOR_OFF, ledBrightness);
     }

}



