#include <stdint.h>

#if defined(ARDUINO) 
#include "Arduino.h"
#include "OneWire.h"
#include "Adafruit_NeoPixel.h"
#include "OneWire.h"
#include "RCSwitch.h"
#else
#include <wiringPi.h>
#include <string>
#include "RPi_utils\RCSwitch.h"
#define CHANGE 1
#ifdef __cplusplus
extern "C" {
#endif
	typedef uint8_t boolean;
	typedef uint8_t byte;
#if !defined(NULL)
#define NULL  0
#endif
#ifdef __cplusplus
}
#endif
#endif



#define NUMBER_OF_NEOPIXELS 1

#if defined(ARDUINO) && ARDUINO >= 100

#define RF_RX_DATA_PIN      2    //  Arduino ProMicro pin 5
#define TEMPERATURE_PIN     3    //  Arduino ProMicro pin 6
#define TEMPERATURE_POWER   A2   //  Arduino ProMicro pin 19
#define RF_RX_POWER_PIN     4    //  Arduino ProMicro pin 7
#define NEOPIXEL_PIN        5    //  Arduino ProMicro pin 8
#define BUZZER              6    //  Arduino ProMicro pin 9
#define PUSH_BUTTON         7    //  Arduino ProMicro pin 10
#define RF_TX_DATA_PIN      8    //  Arduino ProMicro pin 11
#define RF_TX_POWER_PIN     9    //  Arduino ProMicro pin 12
#define DISABLE_RPI_PIN     A3   //  Arduino ProMicro pin 19 
#define RF_RX_CS_PIN        10   //  Arduino ProMicro pin 13
#else
// see http://wiringpi.com/pins/
// or http://pi.gadgetoid.com/pinout

#define RF_RX_DATA_INT      2    // Receiver on interrupt 0 => that is pin #2
#define RF_RX_DATA_PIN      27   // RPi pin 13 
#define TEMPERATURE_PIN     22   // RPi pin 15
#define TEMPERATURE_POWER   2    //  RPi pin 3
#define RF_RX_POWER_PIN     24   // RPi pin 18
#define NEOPIXEL_PIN        18   // RPi pin 12
#define BUZZER              4    // RPi pin 7 
#define PUSH_BUTTON         25   // RPi pin 22  
#define RF_TX_DATA_PIN      17   // RPi pin 11 
#define RF_TX_POWER_PIN     3    //  RPi pin 5
#define RF_RX_CS_PIN        23   //  RPi pin 16
#endif




#define  OFF            0
#define  ON             1

#define COLOR_OFF      0 
#define COLOR_GREEN    1 
#define COLOR_YELLOW   2 
#define COLOR_BLUE     3
#define COLOR_CYAN     4 
#define COLOR_WHITE    5
#define COLOR_MAGENTA  6 
#define COLOR_RED      7 

class EasyPeasy
{
  friend class OneWire;
public:
	EasyPeasy();
	~EasyPeasy(void);
        void begin(void);
        void SetLedColors(uint8_t red, uint8_t green, uint8_t blue);
        void SetLedColor(byte colourId, byte intensity);
        void TemperaturePower(bool state);   
	float GetTemperatureCelcuis();
        float GetTemperatureFahrenheit();
        uint32_t GetIdPart0();
        uint32_t GetIdPart1();
        void BuzzerOn(void);
        void BuzzerOff(void);
        bool IsPushButtonPressed(void);
        void RfTransmitterPower(bool state);
        void RfTransmitData(uint32_t dataByte);
        void RfTransmitText(uint32_t dataByte);
        void RfReceiverPower(bool state);
        bool RfReceiveData(uint32_t* rxData, uint32_t timeout);
        bool RfReceiveDataDetails(uint32_t* rxData, uint32_t timeout);

private:	
    byte data[12];
    byte addr[8];
    uint32_t ledColor;
    void UpdateLED(void);
};
