/*
 *
 * Used for first 30 seconds of automated performance
 *
 * */

#include <DFW.h>
#include <Servo.h> // servo library
#include <LiquidCrystal.h> //LCD library

//  State machine for robot
typedef enum  {FORWARD, LEFT, LEFT2, FOLLOW} State;
State state = FORWARD;

Servo rightmotor2; // Servo object for right motor
Servo leftmotor2; // Servo object for left motor
Servo beaterbar2; //Servo object for beaterbar
Servo arm2; // Servo object for arm

const int rightmotorpin = 7;
const int leftmotorpin = 8;
const int beaterbarpin = 9;
const int armmotorpin = 6;
const int buttonpin = 22 ;
const int rightsensorpin = A7;
const int leftsensorpin = A6;
//pin constants

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd2(12, 11, 5, 4, 3, 2);

int buttonState = 0; // initialize limit switch at zero to register as not hit
int right = analogRead(rightsensorpin); //initialize sensor reading for right line tracker
int left = analogRead(leftsensorpin); //initialize sensor reading for left line tracker

void driveForward() { //moves robot forward at relatively fast speed
  rightmotor2.write(140);
  leftmotor2.write(40);
}

void driveForward2() { //moves robot forward at a slow speed to accomodate line sensing
  rightmotor2.write(112);
  leftmotor2.write(68);
}

void Stop() { // makes robot stop
  rightmotor2.write(90);
  leftmotor2.write(90);
}

void turnLeft() { //makes robot turn left
  rightmotor2.write(130);
  leftmotor2.write(130);
}

void turnRight() { //makes robot turn right
  rightmotor2.write(20);
  leftmotor2.write(20);
}

void armUp() { //raises arm up at a moderate speed
  arm2.write(125);
}

void Auto1() { //Autonomous state machine
  switch (state) {//switch will differentiate between different states with each one being a separate case
    case FORWARD://Forward state
      right = analogRead(rightsensorpin);//read right line tracker
      left = analogRead(leftsensorpin);//read left line tracker

      Serial.println(" " + String(left) + " " + String(right) + " " + "FORWARD");//Diagnostic print out

      if (left < 600) { // if left line tracker senses white line
        state = LEFT;//switches state to left turn
      }
      else {
        driveForward();//keep driving forward otherwise
      }
      break;

    case LEFT://Left turn state
      right = analogRead(rightsensorpin);//read right line tracker
      left = analogRead(leftsensorpin);//read left line tracker

      turnLeft();//turn left at the beging of this state

      if (right < 850 ) { //if right line tracker senses white line
        state = LEFT2;//switches state to right turn
      }

      Serial.println(" " + String(left) + " " + String(right) + " " + "LEFT");//Diagnostic print out

      break;

    case LEFT2://Second left turn state
      right = analogRead(rightsensorpin);//read right line tracker
      left = analogRead(leftsensorpin);//read left line tracker

      if (right < 850 ) { //If right line tracker sense white line
        state = FOLLOW; //Go to line following mode
      }
      else {
        turnLeft(); //turn left until right line tracker senses white
      }

      Serial.println(" " + String(left) + " " + String(right) + " " + "LEFT2");//Diagnostic print out

      break;

    case FOLLOW://Line Following state
      right = analogRead(rightsensorpin);//read right line tracker
      left = analogRead(leftsensorpin);//read left line tracker
      buttonState = digitalRead(buttonpin);//read if button is hit or not
      if (right < 850) { //if right line tracker senses white line
        turnRight(); //turn right
      }
      if (left < 600) { //if left line tracker sense white line
        turnLeft(); //turn left
      }
      if (right > 850 && left > 800) { //if both line trackers are off the white line
        driveForward2(); //drive forward at a moderate speed
      }

      Serial.println(" " + String(left) + " " + String(right) + " " + "FOLLOW"); //Diagnostic print out

      if (buttonState == 0) { //If the limit switch is hit
        beaterbar2.write(148); //Make beaterbar roll outwards to dump eggs in PEN
      }

      break;
  }
}



boolean incomplete = true;//initialize boolean to run actions in the autonomous loop once
void autonomous( long time, DFW & dfw) { // main autonomous function

  if (incomplete) {//goes through loop because the condition is already true
    lcd2.begin(16, 2);//set up LCD columns and rows
    leftmotor2.attach(leftmotorpin, 1000, 2000); // left drive motor pin#, pulse time for 0,pulse time for 180
    rightmotor2.attach(rightmotorpin, 1000, 2000); // right drive motor pin#, pulse time for 0,pulse time for 180
    beaterbar2.attach(beaterbarpin, 1000, 2000);  // beaterbar motor pin#, pulse time for 0,pulse time for 180
    arm2.attach(armmotorpin, 1000, 2000); // arm motor pin#, pulse time for 0,pulse time for 180
    lcd2.setCursor(0, 0);//set LCD cursor to first column, first row
    lcd2.print("Autonomous");//print to LCD
    lcd2.setCursor(0, 1);//set LCD cursor to first column, second row
    lcd2.print("STITCH!!!");//print to LCD
    incomplete = false;//loop is complete, the next loop through autonomous will not attach pins or print to LCD
  }

  if (time > 18000) { //the first two seconds of autonomous the arm is lifted
    armUp();
  }
  else {
    arm2.write(90); //otherwise the arm needs to stop moving
  }

  Auto1();//call state machine function

  if (time < 5000) {//Around five seconds remaining in autonomous, stop all motors so they wont keep running after autonmous
    Stop();
    beaterbar2.write(90);
    arm2.write(90);
  }

  if (time == 10000) {//If halfway through autnomous
    lcd2.clear();//clear LCD display
    lcd2.setCursor(0, 1);//set cursor to first column, second row
    lcd2.print(String(left) + " " + String(right));//print diagnostic to LCD
    lcd2.setCursor(0, 0); //set cursor to first column, first row
    lcd2.print("Autonomous"); //print diagnostic to LCD
  }
}
