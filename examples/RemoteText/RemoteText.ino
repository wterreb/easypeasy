#include <EasyPeasy.h>

// RemoteText - This example uses two EasyPeasy board.  One transmits ascii text characters and the other receives and display it

// IMPORTANT:  If the following line is commented out, this code will run as the receiver part
//			   To turn this into the transmitter, you have to remove the comments
//#define __IS_TRANSMITTER 1 
#define DATA_ID        0x00ABCD00

EasyPeasy easy = EasyPeasy();

String inData;  // Buffer to store incoming commands from serial port


void setup() {
     Serial.begin(115200);
     easy.begin();
}


void loop() { 
#ifdef      __IS_TRANSMITTER
    while (Serial.available() > 0)
    {
        char recieved = Serial.read();
        inData += recieved; 

        // Start transmission when new line is recieved
       if ( recieved == '\n' )
        {
            easy.SetLedColor(COLOR_BLUE, 20);                      // Turn LED on to show we are transmitting
            for (int i=0; i<inData.length(); i++)
            {
              Serial.print(inData[i]); 
              uint32_t thisdata = (uint32_t)inData[i] & 0x00000FF;   // Turn into 8 bits integer number
              thisdata = thisdata | DATA_ID;                         // Add the ID   
              easy.RfTransmitText(thisdata);                         // Transmit one character at a time
            }
            inData = ""; // Transmission complete, so clear recieved buffer
        }
        else {
           easy.SetLedColor(COLOR_OFF, 20);
        }
    }
#else      
      // Receiver section       
       uint32_t rxData = 0;
       if (easy.RfReceiveData(&rxData, 100))    // Monitor the radio receiver for 100 milliseconds for incoming data
       {
          // Something was received
          if ((rxData & 0x00FFFF00) == DATA_ID)   // Check if this came from the tranmitter unit
          {  
             easy.SetLedColor(COLOR_GREEN, 20);         // Turn the LED Green to show valid data was received
             char remotechar = (char)(rxData & 0xFF);   // Only the lower 8 bits contains the actual text data
             Serial.print(remotechar);                  // Display the received character on  
          }
          else {     
             easy.SetLedColor(COLOR_RED, 20);           // Unrecognised data received, so show RED LED
             Serial.print("*");                         // Display * to show we received something unexpected
          }
       }
       else {
           easy.SetLedColor(COLOR_OFF, 20);
       }
#endif
     
}

