#include <EasyPeasy.h>

// PingPong -  A usefull program to test all the hardware of an EasyPeasy board.  Every time you press the button, the EasyPeasy beeps, and takes a temperature measurement.   
//                  At the same time, it also transmits a PING (RF code) and flashes the BLUE LED.  
//                  When the button is released the EasyPeasy turns on its RF receiver and wait to receive a signal.   
//                  If it receives a PING, then it flashed the GREEN LED, wait one second and and then transmit a PONG and also measure the temperature and send it to the remote bord.
//                  If it receives anything else than a valid ping or a valid temperature measurement, then it flashes the RED LED and beeps.
//                  In other words by powering up a second EasyPeasy board running this same program, we can test all the functionality of an EasyPeasy board.  You
//                  simply press the button, check the temperature on the display and also observe that LEDS and beeps.  Because the second board will respond to our signal.
//                  we test both the receiver and the transmitter as well as button, buzzer and temperature sensor.  It is also usefull for range testing where one board remains stationary and you walk around and check that
//                  you keep getting PONG responses to the PINGS you send out..

#define SCALE_FACTOR   50
#define PING        0x00ABC000
#define PONG        0x00DEF000
#define DATA        0x00123000
       
bool flagTransmitPing = false; 
bool flagTransmitPong = false;
bool flagReadTemperature = false;
       
EasyPeasy easy = EasyPeasy();

void setup() {
     Serial.begin(115200);
     easy.begin();
     easy.TemperaturePower(ON);                                   // Power up the temperature sensor
}


void TransmitSensorData(float value)
{
     float tempval = value * SCALE_FACTOR;
     uint32_t thistemp = (uint32_t)tempval &0x0000FFF;   // Turn into 12 bits integer number
     thistemp = thistemp | DATA;  
     easy.RfTransmitData(thistemp);
}

void DisplayTemperature(uint32_t remotetemp)
{
   remotetemp = remotetemp & 0xFFF;   // The lower 12 bits contains the actual temperature data
   float remote_temperature = (float)remotetemp / SCALE_FACTOR;
   Serial.print("Remote temperature : ");
   Serial.println(remote_temperature);
}

void loop() { 
  if (easy.IsPushButtonPressed()) 
  { 
      flagTransmitPing = true;
  }
  else {
     uint32_t rxData = 0;
     if (easy.RfReceiveData(&rxData, 100))                       // Monitor the radio receiver for 1000 milliseconds for incoming data
     {
         easy.SetLedColor(COLOR_GREEN, 250);                     // Turn the LED to GREEN to show we received something
         Serial.print("RECEIVED : ");
         easy.BuzzerOn();                                        // Beep
         if (rxData == PING)
         { 
             Serial.println("PING");
             delay(100);  
             flagTransmitPong = true;
         }
         else {                                
             if (rxData == PONG) {
                 Serial.println("PONG");
                 flagReadTemperature = true;
             }
             else {
                 if ((rxData & 0x00FFF000) == DATA) {
                    DisplayTemperature(rxData);
                 }
                 else {
                   easy.SetLedColor(COLOR_RED, 250);              // Turn the LED to RED to show we received something unexpected
                   delay(100);  
                   Serial.println(rxData, HEX);                    // Not sure what we received, so just display the actual received data
                 }
             }
         }    
     }
  }
  if ( flagTransmitPing ) {
     flagTransmitPing = false;
     easy.SetLedColor(COLOR_BLUE, 250);                       // Turn the LED to BLUE to show the transmitter is on
     Serial.println("TRANSMITTING : PING");
     easy.RfTransmitData(PING);                               // Transmit a ping to the other EasyPeasy board 
  }
  if ( flagTransmitPong ) {
      flagTransmitPong = false;
      delay(100);
      easy.SetLedColor(COLOR_BLUE, 250);                       // TTurn the LED to BLUE to show the transmitter is on
      delay(100);
      Serial.println("TRANSMITTING : PONG");
      easy.RfTransmitData(PONG);   
      flagReadTemperature = true;

  }
  if ( flagReadTemperature ) {
      flagReadTemperature = false;
      float temperature = easy.GetTemperatureCelcuis();        // Measure the temperature
      Serial.print("Transmitting local temperature : ");
      Serial.println(temperature);
      easy.SetLedColor(COLOR_BLUE, 20);
      TransmitSensorData(temperature);                         // Send measure temperature value to remote device    
  }
  
  easy.BuzzerOff();
  easy.SetLedColor(COLOR_OFF, 250);
}
  

