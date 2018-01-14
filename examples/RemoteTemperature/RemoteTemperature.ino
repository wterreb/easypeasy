#include <EasyPeasy.h>


// RemoteTemperature -  One EasyPeasy433 board measure temperatue and sends it to the other one via RF link.  
//                       Both temperatures are on the serial port on transmitter and receiver side and the LED colors gives insight as to what is happening.
//                       Requires two EasyPeasy433 boards.  On for transmitter and one for receiver.  
//                       Note the   __IS_TRANSMITTER #define that needs to commented to make it a receiver board

// IMPORTANT:  If the following line is commented out, this code will run as the receiver part
//			   To turn this into the transmitter, you have to remove the comments
//#define __IS_TRANSMITTER 1 
#define SCALE_FACTOR   50
#define DATA_ID        0x00ABC000
       
EasyPeasy easy = EasyPeasy();

void setup() {
     Serial.begin(115200);
     easy.begin();
}

void loop() { 
#ifdef      __IS_TRANSMITTER
      // Transmitter section
      unsigned long timeNow = millis();
      static unsigned long timeNext = 0;
      if (timeNow > timeNext)
      {
          timeNext = timeNow + 1000;
          // Measure Local temperature
          float local_temperature = easy.GetTemperatureCelcuis();
          Serial.print("Measured local temperature : ");
          Serial.println(local_temperature);
          
          // Transmit local temperature to remote device
         float tempval = local_temperature * SCALE_FACTOR;
         uint32_t thistemp = (uint32_t)tempval &0x0000FFF;   // Turn into 12 bits integer number
         thistemp = thistemp | DATA_ID;
         easy.SetLedColor(COLOR_BLUE, 20);
         easy.RfTransmitData(thistemp);
      }
#else      
      // Receiver section
       uint32_t rxData = 0;
       if (easy.RfReceiveData(&rxData, 1100))    // Monitor the radio receiver for 1000 milliseconds for incoming data
       {
          // Something was received
          if ((rxData & 0x00FFF000) == DATA_ID)   // Check if this came from the tranmitter unit
          {  
             uint32_t remotetemp = rxData & 0xFFF;   // The lower 12 bits contains the actual temperature data
             float remote_temperature = (float)remotetemp / SCALE_FACTOR;
             Serial.print("Received remote temperature : ");
             Serial.println(remote_temperature);
             easy.SetLedColor(COLOR_GREEN, 20); 
             delay(100);     
          }
          else {     
             Serial.print("Unrecognised data recevieved : ");
             Serial.println(rxData, HEX);
             easy.SetLedColor(COLOR_RED, 20); 
             delay(100);
          }
       }
       else {
             Serial.println("No remote data received");
       }
#endif
      easy.SetLedColor(COLOR_OFF, 20);
}



