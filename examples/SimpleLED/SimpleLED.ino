//   SimpleLED : Shows how simple it is to control the three colour Adafruit LED strip 
//               Requires the EasyPeasy433 board + Adafruit Neopixel library ( https://github.com/adafruit/Adafruit_NeoPixel/archive/master.zip )
#include <EasyPeasy.h>

EasyPeasy easy = EasyPeasy();

void setup() {
  easy.begin();
}

void loop() { 
     byte intensity = 20;
     
     easy.SetLedColor(COLOR_GREEN, intensity);  delay(500);
     easy.SetLedColor(COLOR_YELLOW, intensity); delay(500);
     easy.SetLedColor(COLOR_BLUE, intensity); delay(500);
     easy.SetLedColor(COLOR_CYAN, intensity); delay(500);
     easy.SetLedColor(COLOR_WHITE, intensity); delay(500);
     easy.SetLedColor(COLOR_MAGENTA, intensity); delay(500);
     easy.SetLedColor(COLOR_RED, intensity); delay(500);
     easy.SetLedColor(COLOR_OFF, intensity); delay(500);
  
    // Step through all the above colors with each one increasing its intensity
    for (byte color=1; color<7; color++)
    {
      for (byte brightness=0; brightness<255; brightness++)
      {
         easy.SetLedColor(color, brightness);  delay(20);
      }
    }
  
    //  Demo of picking colors by RGB (red, green, blue) values   
    easy.SetLedColors(22, 12, 22); delay(1000); // Low intensity pink
    easy.SetLedColors(5, 30, 5); delay(1000);    // Bright light green
    easy.SetLedColors(0, 40, 0); delay(1000);    // Bright dark green
    easy.SetLedColors(0, 0, 5); delay(1000);      // Dim blue
}

