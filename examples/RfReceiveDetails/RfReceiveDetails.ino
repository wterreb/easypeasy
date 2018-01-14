#include <EasyPeasy.h>


// RfReceiveDetails -    Shows detailed information about data received  via RF receiver.   Usefull when analizing an unknown transmitter device  
//                       Requires one EasyPeasy433 boards.   


EasyPeasy easy = EasyPeasy();


void setup() {
     Serial.begin(115200);
     easy.begin();
}

void loop() { 
     uint32_t rxData = 0;
     if (easy.RfReceiveDataDetails(&rxData, 1000)) {   // Monitor the radio receiver for 1000 milliseconds for incoming data
         easy.SetLedColor(COLOR_GREEN, 20); 
     }
     else {
        easy.SetLedColor(COLOR_OFF, 20);
     }
}
