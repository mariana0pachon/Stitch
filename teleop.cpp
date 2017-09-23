/*
 *
 *Used for teleoperated commands
 *
 *   
*/


#include <DFW.h>
#include <Servo.h> // servo library
#include <LiquidCrystal.h>

Servo rightmotor3; // Servo object for right motor
Servo leftmotor3; // Servo object for left motor
Servo beaterbar3; //Servo object for beaterbar
Servo arm3; //Servo object for arm

const int rightmotorpin = 7;
const int leftmotorpin = 8;
const int beaterbarpin = 9;
const int armmotorpin = 6;
//pin constants

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd3(12, 11, 5, 4, 3, 2);


void BeaterBar(DFW & dfw) {
  if (dfw.r1() == 1 ) { //If r1 is pressed
    beaterbar3.write(48); //make beaterbar roll inwards
  }
  else if (dfw.r2() == 1 ) { //If r2 is pressed
    beaterbar3.write(160); //make beaterbar roll outwards
  }
  else if (dfw.one() == 1 ) { //If button one is pressed
    beaterbar3.write(90); //make beaterbar stop
  }
}

void Arm(DFW & dfw) {
  if (dfw.l2() == 1) {//If l2 is pressed
    arm3.write(15); //move arm down
  }
  else if (dfw.l1() == 1) {//If l1 is pressed
    arm3.write(165);//move arm up
  }
  else {
    arm3.write(90);//otherwise stop arm
  }
}

boolean incomplete2 = true; //initialize boolean to run actions in teleop once
void teleop( long time, DFW & dfw) { //main teleop function
  if (incomplete2) {//goes through loop the first run because the condition is already true
    lcd3.begin(16, 2);//set up LCD columns and rows
    leftmotor3.attach(leftmotorpin, 1000, 2000); // left drive motor pin#, pulse time for 0,pulse time for 180
    rightmotor3.attach(rightmotorpin, 1000, 2000); // right drive motor pin#, pulse time for 0,pulse time for 180
    beaterbar3.attach(beaterbarpin, 1000, 2000);  // beaterbar motor pin#, pulse time for 0,pulse time for 180
    arm3.attach(armmotorpin, 1000, 2000); // arm motor pin#, pulse time for 0,pulse time for 180
    rightmotor3.write(90);//stop right motor
    leftmotor3.write(90);//stop left motor
    lcd3.clear();//clear LCD display
    lcd3.setCursor(0, 0);//set cursor to first column, first row
    lcd3.print("STITCH!!!");//print to LCD
    incomplete2 = false;//loop is complete, next run through won't attach pins, stop motors or print to LCD
  }

  Arm(dfw); //call arm function
  BeaterBar(dfw); //call beaterbar function

  rightmotor3.write(dfw.joystickrv());   //DFW.joystick will return 0-180 as an int into rightmotor.write
  leftmotor3.write(180 - dfw.joysticklv());  //DFW.joystick will return 0-180 as an int into leftmotor.write

  if (time < 1000) { //If there is one second left in teleop, stop all motors to make sure robot won't keep moving after teleop ends
    leftmotor3.write(90);
    rightmotor3.write(90);
    beaterbar3.write(90);
    arm3.write(90);
  }
}
