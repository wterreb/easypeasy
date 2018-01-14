#include <EasyPeasy.h>

// SimpleTemperature -  Example of how to read temperature off the Dallas DS18B20 chip
//                       Requires the EasyPeasy433 board 

EasyPeasy easy = EasyPeasy();

void setup() {
  Serial.begin(115200);
  easy.begin();
}

void loop() {
  float temperature = easy.GetTemperatureCelcuis();
  Serial.print("Celcuis = ");  Serial.println(temperature);
  temperature = easy.GetTemperatureFahrenheit();
  Serial.print("Fahrenheit = ");  Serial.println(temperature);
}

