/* 

MAIN FRAMEWORK

*/
#include <DFW.h>
#include <Servo.h>
#include<LiquidCrystal.h>

Servo rightmotor; // Servo object for right motor
Servo leftmotor; // Servo object for left motor
Servo beaterbar; //Servo object for beaterbar
Servo arm; //Servo object for arm

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);




int ledpindebug = 13; //Wireless controller Debug pin. If lit then there is no communication.
DFW dfw(ledpindebug, autonomous, teleop); // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds
void setup() {
  Serial.begin(9600); // Serial output begin. Only needed for debug
  dfw.begin(); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"



}
void loop() {
  dfw.run();
}
