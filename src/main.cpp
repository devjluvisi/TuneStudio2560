/*
-------------------------------
TUNE STUDIO 2560
Open source song creation and playback using the ATMega2560 microchip
on AVR 8-Bit architecture.

CURRENT PROJECT STATUS: NOT FUNCTIONAL
-------------------------------
Official GitHub Repo:
  - (TBD)

Hardware Compatability:
  - TuneStudio2560 was created specifically for 8-bit AVR microarchitecture although 
  it may be possible to run TuneStudio2560 on compatible AVR boards which can supply
  the neccessarily Analog/Digital & I2C interfaces.
  - A port to the RP2040 microchip is currently under consideration.

Unit Testing Information:
  - (TBD)

C++ Library Information:
  - (TBD)

TuneStudio2560 was created entirely in PlatformIO a visual studio extension
which allows the fast and complete creation of programs for various microcontrollers.
For the best compatiblity it is reccomended that this GitHub repo is edited and run
using PlatformIO.
--------------------------------
By Jacob LuVisi (2021)
6/27/2021 - ???

*/


#include <Arduino.h>
#include <tune_studio.h>

void setup() {
  Serial.begin(9600);
  while(!Serial)
  Serial.println("Started serial monitor.");
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
}

void loop() {
        digitalWrite(RGB_RED, HIGH);
        delay(1000);
        Serial.println(String(digitalRead(RGB_RED)));
        digitalWrite(RGB_RED, LOW);
}