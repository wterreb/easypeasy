#include "EasyPeasy.h"


EasyPeasy::EasyPeasy() {
}

EasyPeasy::~EasyPeasy(void) {
}

void EasyPeasy::begin(void)
{
   pinMode(BUZZER, OUTPUT); 
   digitalWrite(BUZZER, LOW);   // Ensure the Buzzer is off to begin with
   SetLedColor(COLOR_OFF, 0);   // Ensure the LED is off to begin with
#if defined(ARDUINO)
   pinMode(DISABLE_RPI_PIN, OUTPUT);   
   digitalWrite(DISABLE_RPI_PIN, HIGH);  // When using Arduino, ensure the RPi is off
#endif
}

//  --------------  Adafruit Neopixel LED code

void EasyPeasy::SetLedColors(uint8_t red, uint8_t green, uint8_t blue)
{
     // Convert separate R,G,B into packed 32-bit RGB color.
   // Packed format is always RGB, regardless of LED strand color order. 
   ledColor =  ((uint32_t)green << 16) | ((uint32_t)red <<  8) | (uint32_t)blue;
   UpdateLED();
}

void EasyPeasy::SetLedColor(byte colourId, byte intensity)
{
   uint8_t r; uint8_t g; uint8_t b; 
   switch (colourId)
   {
     case COLOR_GREEN: r=0, g=intensity, b=0; break;
     case COLOR_YELLOW: r=intensity, g=intensity, b=0; break;
     case COLOR_BLUE: r=0, g=0, b=intensity; break;
     case COLOR_CYAN: r=0, g=intensity, b=intensity; break;
     case COLOR_WHITE: r=intensity, g=intensity, b=intensity; break;
     case COLOR_MAGENTA: r=intensity, g=0, b=intensity; break;
     case COLOR_RED: r=intensity, g=0, b=0; break;
     default :  r=0, g=0, b=0; break;
   }
   // Convert separate R,G,B into packed 32-bit RGB color.
   // Packed format is always RGB, regardless of LED strand color order.
   ledColor =  ((uint32_t)g << 16) | ((uint32_t)r <<  8) | b;    
   UpdateLED();
}

void EasyPeasy::UpdateLED(void)
{
  #if defined(ARDUINO)
  // When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
  strip.begin(); // This initializes the NeoPixel library.
  strip.setPixelColor(0, ledColor);
  strip.show(); // This sends the updated pixel color to the hardware.
  #endif
}

//  --------------  DALLAS Temperature chip code

void EasyPeasy::TemperaturePower(bool state)
{
  pinMode(TEMPERATURE_POWER, state);      // Set the Transmitter power pin to output mode
  digitalWrite(TEMPERATURE_POWER, state);  // Turn on/off power to the RF Transmitter 
  pinMode(TEMPERATURE_POWER, state);       // Turn pin back to an input when state is 0
}


float EasyPeasy::GetTemperatureCelcuis()
{
  bool alldone = false;
  OneWire  ds(TEMPERATURE_PIN);
  byte i;
  byte present = 0;
  byte type_s;
  float celsius, fahrenheit;
  
  TemperaturePower(ON);   // Power up the Dallas temperature sensor on
 
  if ( !ds.search(addr)) {
    Serial.println("Dallas DS18B20 chip not detected.");
    Serial.println();
    ds.reset_search();
    delay(250);
    alldone = true;
  }
  else {
    if (OneWire::crc8(addr, 7) != addr[7]) {
        Serial.println("CRC is not valid!");
        alldone = true;
    }
    if (alldone == false)
    {
      if (addr[0] != 0x28)
      {
          Serial.println("Device is not a DS18B20 family device.");
          alldone = true;
      }
      if (alldone == false)
      {
        ds.reset();
        ds.select(addr);
        ds.write(0x44, 1);        // start conversion, with parasite power on at the end
        
        delay(500);     // maybe 750ms is enough, maybe not
        // we might do a ds.depower() here, but the reset will take care of it. 
        present = ds.reset();
        ds.select(addr);    
        ds.write(0xBE);         // Read Scratchpad
      
        for ( i = 0; i < 9; i++) {           // we need 9 bytes
          data[i] = ds.read();
        }
      
        // Convert the data to actual temperature
        // because the result is a 16 bit signed integer, it should
        // be stored to an "int16_t" type, which is always 16 bits
        // even when compiled on a 32 bit processor.
        int16_t raw = (data[1] << 8) | data[0];
        if (type_s) {
          raw = raw << 3; // 9 bit resolution default
          if (data[7] == 0x10) {
            // "count remain" gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
          }
        } else {
          byte cfg = (data[4] & 0x60);
          // at lower res, the low bits are undefined, so let's zero them
          if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
          else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
          else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
          //// default is 12 bit resolution, 750 ms conversion time
        }
        celsius = (float)raw / 16.0;
      }
    }
  }
  return celsius;
}


float EasyPeasy::GetTemperatureFahrenheit()
{
  float celsius = GetTemperatureCelcuis();
  float fahrenheit = celsius * 1.8 + 32.0;
  return fahrenheit;
}

uint32_t EasyPeasy::GetIdPart0()
{
   uint32_t retVal =  ((uint32_t)addr[0] << 24) | ((uint32_t)addr[1] <<  16) | ((uint32_t)addr[2] <<  8) | ((uint32_t)addr[3] <<  0);
   return retVal;
}

uint32_t EasyPeasy::GetIdPart1()
{
   uint32_t retVal =  ((uint32_t)addr[4] << 24) | ((uint32_t)addr[5] <<  16) | ((uint32_t)addr[6] <<  8) | ((uint32_t)addr[7] <<  0);
   return retVal;
}

//  --------------  Buzzer and Pusbutton     
void EasyPeasy::BuzzerOn()
{
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH); 
}

void EasyPeasy::BuzzerOff()
{
   pinMode(BUZZER, OUTPUT);
   digitalWrite(BUZZER, LOW); 
}

bool EasyPeasy::IsPushButtonPressed()
{
   pinMode(PUSH_BUTTON, INPUT);
   int buttonState = digitalRead(PUSH_BUTTON);
   return (buttonState == 1);
}

//  --------------  433 Mhz Transmitter code

void EasyPeasy::RfTransmitterPower(bool state)
{
  
  pinMode(RF_TX_POWER_PIN, state);      // Set the Transmitter power pin to output mode
  digitalWrite(RF_TX_POWER_PIN, state);  // Turn on/off power to the RF Transmitter 
  pinMode(RF_TX_POWER_PIN, state);       // Turn pin back to an input when state is 0
}

void EasyPeasy::RfTransmitData(uint32_t dataByte)
{
   RfTransmitterPower(ON);               // Turn on power to the RF transmitter
   RCSwitch rc = RCSwitch(); 
   rc.enableTransmit(RF_TX_DATA_PIN);    // Transmitter is connected to Arduino Pin #8 
   rc.send(dataByte, 24);
}

void EasyPeasy::RfTransmitText(uint32_t dataByte)
{
   RfTransmitterPower(ON);               // Turn on power to the RF transmitter
   RCSwitch rc = RCSwitch(); 
   rc.setRepeatTransmit(2);              // Do not repeast transmissions in the case of text
   rc.enableTransmit(RF_TX_DATA_PIN);    // Transmitter is connected to Arduino Pin #8 
   rc.send(dataByte, 24);
}    

void EasyPeasy::RfReceiverPower(bool state)
{
  pinMode(RF_RX_POWER_PIN, state);      // Set the Receiver power pin to output mode, or input if state is 0
  digitalWrite(RF_RX_POWER_PIN, state);  // Turn on/off power to the RF Receiver 
  pinMode(RF_RX_POWER_PIN, state);      // Set the Receiver power pin to output mode, or input if state is 0
  pinMode(RF_RX_CS_PIN, state);         // Set the Receiver Chip Select pin to output mode
  digitalWrite(RF_RX_CS_PIN, state); // Turn on/off CS to the RF Receiver
  pinMode(RF_RX_CS_PIN, state);       // Turn pin back to an input when state is 0
}

bool EasyPeasy::RfReceiveData(uint32_t* rxData, uint32_t timeout)
{
    bool retValue = false;
    RCSwitch rc = RCSwitch(); 
    rc.enableReceive(1);  // Receiver on interrupt 1 => that is pin #2 on Arduino Micro board
    RfReceiverPower(ON);
    
    unsigned long endtime = millis() + timeout;
    do {
      if (rc.available()) {   
         *rxData = rc.getReceivedValue();
         retValue = true;
      }
    } while ( (retValue==false) && (millis() < endtime) ); 
    //RfReceiverPower(OFF);  // Turn off power to the RF receiver 
    return retValue;  
}

bool EasyPeasy::RfReceiveDataDetails(uint32_t* rxData, uint32_t timeout)
{
  bool retValue = false;
    RCSwitch rc = RCSwitch(); 
#if defined(__AVR_ATmega328P__) 
    rc.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2 on Arduino Uno board
#else
    rc.enableReceive(1);  // Receiver on inerrupt 1 => that is pin #2 on Arduino Micro board
#endif
    pinMode(RF_RX_POWER_PIN, OUTPUT);     // Set the recevier power pin to output mode
    digitalWrite(RF_RX_POWER_PIN, HIGH);  // Turn on power to the RF receiver 
    
    unsigned long endtime = millis() + timeout;
    do {
      if (rc.available()) {   
         *rxData = rc.getReceivedValue();
          if (*rxData == 0) {
              Serial.print("Unknown encoding");
          } else {
              retValue = true;
              Serial.print("Received ");
              Serial.print( rc.getReceivedValue() );
              Serial.print(" ( 0x");
              Serial.print(*rxData, HEX);
              Serial.print(" )");
              Serial.print(" / ");
              Serial.print( rc.getReceivedBitlength() );

              Serial.print(" Bit, Protocol : ");
              Serial.print(rc.getReceivedProtocol() );
              Serial.print(", Delay = " );
              Serial.print( rc.getReceivedDelay() );
              Serial.println(" ms" );
          }
          rc.resetAvailable();
      }
    } while ( (retValue==false) && (millis() < endtime) ); 
    digitalWrite(RF_RX_POWER_PIN, LOW);  // Turn off power to the RF receiver 
    return retValue;  
}

