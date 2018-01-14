#include <EasyPeasy.h>

// UniqueID -  Example of how to read the unique ID off the Dallas DS18B20 chip
//               Requires the EasyPeasy433 board (see www.easypeasy


EasyPeasy easy = EasyPeasy();

void setup() {
     Serial.begin(115200);
     easy.begin();
    // Do at least one dummy temperature reading.  (Required because this also reads the unique id
    float temp = easy.GetTemperatureCelcuis();
}

void loop() {
      //Now we can read the UniqueID in two parts
    uint32_t id0 = easy.GetIdPart0(); Serial.print(id0, HEX);
    uint32_t id1 = easy.GetIdPart1(); Serial.println(id1, HEX);
    delay(1000);
}



