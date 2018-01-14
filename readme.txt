

Note :  You will need to install the GIThub client for Windows  (or other OS) before you continue.

To get the EasyPeasy library
============================
git clone https://github.com/kiwisaner/easypeasy.git

To add the EasyPeasy library In Arduino IDE 
===========================================

In Arduino IDE go to the menu option Sketch -> Include Library > Add .Zip Library
Select the EasyPeasy.zip file in your download directory

To load and run the examples in Arduino IDE:
============================================
Simply go to File -> Examples -> EasyPeasy  and select any of the following examples:

SimpleLED  -  Just demonstrates how change the color of the NeoPixel LED
SimpleTemperarure - Just demonstrates how to read and distplay the Dallas DS18B20 temperature sensor
UniqueID - Shows how to read and display the unique ID from the Dallas temperature sensor
SimpleRfTrasnmitter -  Shows how to transmit a signal on the onboard 433 MhZ transmitter
SimpleRfReceiver - Show the 433 Mhz receiver in action and displays any signals it receives.
RfReceiveDetails - Useful for analysing details of received RF signals.
Remote Temperature - Shows how to implement a wireless temperature sensor
Remote Text - How to transmit text strings using the 433 transmitter and receiver
PingPong - Uses almost all functionality of the EasyPeasy board.   Usefull for testing the boards.

