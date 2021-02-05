/***************************

  Moves the stepper motors for the bar, given a value from user input (ie distance sensors)
  
  IMPORTANT: If you don't have a driver connected to your arduino and directly plugged the stepper into
            your arduino instead, uncomment the code labeled "without driver" and 
            comment out the code labeled "with driver"

  
  Assumptions:
  - distance sensor code will give the motors an int within the range [-3, 3]
  - +ve speed means "up", -ve speed means "down", 0 means stationary, and |3| is the fastest speed
  - when u power on the arduino, the bar always starts at the height needed to load a ball
  - spinning motor CCW moves bar UP
  - the top of the display is at stepper position 0
  - stepper position increases as bar goes down

 coordinate system:

        (top of display)
 [barPosL = 0         barPosR = 0]
  |                     |
  |                     |
  ...    (gameboard)   ...
  |                     |
  |                     |
[barPosL = 10000       barPosR = 10000]
   (bottom of playing area aka FLOOR)
  |                     |
  |                     |
 [barPosL = 11000       barPosR = 11000]
 (height for ball return area aka BALL_RETURN_HEIGHT)

  References:

  AccelStepper Library Documentation
  https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html

 DRV8825 Stepper Driver Tutorial
  - includes tips about setting up hardware as well as step-by-step code
  https://www.makerguides.com/drv8825-stepper-motor-driver-arduino-tutorial/

  Tutorial for smaller 5V 28byj48 stepper motors
  https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/
  Warning: If using ULN2003 motor drivers, you must intentionally swap pins 2 and 3 for the reverse direction to work

 ***************************/

#include <AccelStepper.h>
#include <Stepper.h>
//#include <MultiStepper.h>

//EXPERIMENTAL VALUES - will need to adjust as we prototype

//Uncomment me if using stepper.h
//#define STEPPER_H 1

//Uncomment me if using accelstepper.h
#define ACCELSTEPPER 1

//Uncomment me if using a driver
//#define ACCELSTEPPER_WITH_DRIVER

//Uncomment me if NOT using a driver
#define ACCELSTEPPER_NO_DRIVER

//instead of manually uncommenting blocks, the compiler will check if you've included the single line "#define STEPPER_H"
//it won't include this code if STEPPER_H is undefined
#ifdef STEPPER_H
#define CEILING 0                //highest height of bar
#define FLOOR 10000              //bottom of the playing area, not actually the floor
#define BALL_RETURN_HEIGHT 11000 //lowest height of bar, where bar will pick up ball
#define MAX_BAR_TILT 200         //maximum vertical slope of bar, aka barPosRight - barPosLeft
#define MAX_SPEED 15
#define STEP_INCR 1 //steps taken on each loop() iteration
#endif

#ifdef ACCELSTEPPER
#define CEILING -30000
#define FLOOR 50000
#define BALL_RETURN_HEIGHT 51000
#define MAX_BAR_TILT 100000 //need to test
#define MAX_SPEED 600
#define MAX_ACCEL 300
#define SPEED_MULT 5
#define STEP_INCR 100
#endif

#define STEPS_PER_REV 2048 //only for 28byj stepper with ULN2003 driver

//for both stepper.h and accelstepper.h
#define SPEED_MULT 5           //multiply user input value with this number to set desired stepper speed
#define BALL_RETURN_DELAY 2000 //time to wait until a new ball has rolled onto bar
//distance sensors code will give the motors a number within the range [-3, 3]
//-ve speed means "down", +ve speed means "up"
#define STOP 0
#define SLOW 1
#define MED 2
#define FAST 3

int userInputLeft; //from distance sensor functions
int userInputRight;
int barPosL = FLOOR;
int barPosR = FLOOR;

int barTilt = 0;

//WITHOUT DRIVER
#define RT_COIL_1A 4
#define RT_COIL_1B 5
#define RT_COIL_2A 6
#define RT_COIL_2B 7
#define LT_COIL_1A 10
#define LT_COIL_1B 11
#define LT_COIL_2A 12
#define LT_COIL_2B 13

//For stepper.h, no driver
#ifdef STEPPER_H
Stepper motorR = Stepper(STEPS_PER_REV, RT_COIL_1A, RT_COIL_1B, RT_COIL_2A, RT_COIL_2B);
Stepper motorL = Stepper(STEPS_PER_REV, LT_COIL_1A, LT_COIL_1B, LT_COIL_2A, LT_COIL_2B);
#endif

//we will eventually need to use AccelStepper.h functions, since Stepper.h isn't compatiable with motor driver that
//have a STEP and DIR pin

//for accelstepper.h, no driver
#ifdef ACCELSTEPPER_NO_DRIVER
AccelStepper motorR(AccelStepper::FULL4WIRE, RT_COIL_1A, RT_COIL_1B, RT_COIL_2A, RT_COIL_2B);
AccelStepper motorL(AccelStepper::FULL4WIRE, LT_COIL_1A, LT_COIL_1B, LT_COIL_2A, LT_COIL_2B);
#endif

#ifdef ACCELSTEPPER_WITH_DRIVER
#define STEP_R 4
#define DIR_R 5
#define STEP_L 6
#define STEP_L 7
AccelStepper motorR(AccelStepper::DRIVER, STEP_R, DIR_R);
AccelStepper motorL(AccelStepper::DRIVER, STEP_L, DIR_L);
#endif

/***************************************/

////To make both motors arrive at the same position at the same time? need to research
//MultiStepper bothSteppers;

//FOR PROTOTYPING ONLY. pushbuttons and LED pins
//these should actually be global variables from other parts of code
#define R_UP 9 //pushbuttons
#define R_DOWN 8
#define L_UP 3
#define L_DOWN 2
#define SWAP A0

#define L_FLOOR A5 //LEDs
#define L_CEIL A4
#define R_FLOOR A3
#define R_CEIL A2
#define MAX_TILT_REACHED A1

bool swapControls = false;
int prevSwapReading = LOW;

int userSpeed = 1;
//end of prototyping variables

void setup()
{

  //uncomment me if using accelstepper
  #ifdef ACCELSTEPPER
  motorR.setMaxSpeed(MAX_SPEED);
  motorL.setMaxSpeed(MAX_SPEED);
  motorR.setAcceleration(MAX_SPEED);
  motorL.setAcceleration(MAX_SPEED);
  #endif
  
  //for both stepper.h and accelstepper.h
  //Note: speed is in rpm for stepper.h, but it's in steps/sec for accelstepper.h
  #ifdef STEPPER_H
  motorR.setSpeed(MAX_SPEED);
  motorL.setSpeed(MAX_SPEED);
  #endif

  //FOR PROTOTYPING ONLY
  pinMode(R_DOWN, INPUT);
  pinMode(R_UP, INPUT);
  pinMode(L_DOWN, INPUT);
  pinMode(L_UP, INPUT);
  pinMode(SWAP, INPUT);
  pinMode(L_FLOOR, OUTPUT);
  pinMode(R_FLOOR, OUTPUT);
  pinMode(L_CEIL, OUTPUT);
  pinMode(R_CEIL, OUTPUT);
  pinMode(MAX_TILT_REACHED, OUTPUT);

  barPosL = FLOOR/2; //TODO: set to FLOOR when finished debugging
  barPosR = FLOOR/2;

  //resetBar();
}

void loop()
{
  //PROTYPING ONLY - get user input from pushbuttons
  if (digitalRead(R_UP) == HIGH)
  {
    userInputRight = 1;
  }
  else if (digitalRead(R_DOWN) == HIGH)
  {
    userInputRight = -1;
  }
  else
  {
    userInputRight = 0;
  }

  if (digitalRead(L_UP) == HIGH)
  {
    userInputLeft = 1;
  }
  else if (digitalRead(L_DOWN) == HIGH)
  {
    userInputLeft = -1;
  }
  else
  {
    userInputLeft = 0;
  }

  //pressing 'swap' button will toggle left and right controls
  //  if (digitalRead(SWAP) == LOW && prevSwapReading == HIGH)
  //  {
  //    swapControls = !swapControls;
  //  }

  //pressing 'swap' button will toggle speed
  //from slow (5rpm), med (10rpm), fast (15rpm), then back to slow
  if (digitalRead(SWAP) == LOW && prevSwapReading == HIGH)
  {
    userSpeed++;
    if (userSpeed > 3)
    {
      userSpeed = 1;
    }
    motorR.setSpeed(userSpeed * SPEED_MULT); //TODO: if swapControls, swap left and right speed
    motorL.setSpeed(userSpeed * SPEED_MULT); //only set the speed if user input changes
  }

  prevSwapReading = digitalRead(SWAP);

  moveBar();
}
//STEPPER.H VERSION
#ifdef STEPPER_H
void moveBar()
{
  //Move motors one step
  if (swapControls) //special level: SWAP right and left controls
  {
    if (userInputRight > 0 && barPosL > CEILING && barTilt > -MAX_BAR_TILT)
    { //move LEFT side of bar UP
      motorL.step(STEP_INCR);
      barPosL--;
    }
    else if (userInputRight < 0 && barPosL < FLOOR && barTilt < MAX_BAR_TILT)
    { //move LEFT side of bar DOWN
      motorL.step(-STEP_INCR);
      barPosL++;
    }

    if (userInputLeft > 0 && barPosR > CEILING && barTilt < MAX_BAR_TILT)
    { //move RIGHT side of bar UP
      motorR.step(STEP_INCR);
      barPosR--;
    }
    else if (userInputLeft < 0 && barPosR < FLOOR && barTilt > -MAX_BAR_TILT)
    { //move RIGHT side of bar DOWN
      motorR.step(-STEP_INCR);
      barPosR++;
    }
  }
  else //normal controls
  {

    if (userInputRight > 0 && barPosR > CEILING && barTilt < MAX_BAR_TILT)
    { //move right side of bar UP
      motorR.step(STEP_INCR);
      barPosR--;
    }
    else if (userInputRight < 0 && barPosR < FLOOR && barTilt > -MAX_BAR_TILT)
    { //move right side of bar DOWN
      motorR.step(-STEP_INCR);
      barPosR++;
    }

    if (userInputLeft > 0 && barPosL > CEILING && barTilt > -MAX_BAR_TILT)
    { //move left side of bar UP
      motorL.step(STEP_INCR);
      barPosL--;
    }
    else if (userInputLeft < 0 && barPosL < FLOOR && barTilt < MAX_BAR_TILT)
    { //move left side of bar DOWN
      motorL.step(-STEP_INCR);
      barPosL++;
    }
  }

  barTilt = barPosL - barPosR;

  //turn on LEDs for debugging
  if (abs(barTilt) == MAX_BAR_TILT)
  {
    digitalWrite(MAX_TILT_REACHED, HIGH);
  }
  else
  {
    digitalWrite(MAX_TILT_REACHED, LOW);
  }

  switch (barPosL)
  {
  case FLOOR:
    digitalWrite(L_FLOOR, HIGH);
    break;
  case CEILING:
    digitalWrite(L_CEIL, HIGH);
    break;
  default:
    digitalWrite(L_FLOOR, LOW);
    digitalWrite(L_CEIL, LOW);
  }

  switch (barPosR)
  {
  case FLOOR:
    digitalWrite(R_FLOOR, HIGH);
    break;
  case CEILING:
    digitalWrite(R_CEIL, HIGH);
    break;
  default:
    digitalWrite(R_FLOOR, LOW);
    digitalWrite(R_CEIL, LOW);
  }
}
#endif //END OF STEPPER.H VERSION

//ACCELSTEPPER.H VERSION
#ifdef ACCELSTEPPER
void moveBar()
{
  //Move motors one step
  if (swapControls) //special level: SWAP right and left controls
  {
    if (userInputRight > 0 && barPosL > CEILING && barTilt > -MAX_BAR_TILT)
    { //move LEFT side of bar UP
      motorL.move(STEP_INCR);
      barPosL--;
    }
    else if (userInputRight < 0 && barPosL < FLOOR && barTilt < MAX_BAR_TILT)
    { //move LEFT side of bar DOWN
      motorL.move(-STEP_INCR);
      barPosL++;
    }

    if (userInputLeft > 0 && barPosR > CEILING && barTilt < MAX_BAR_TILT)
    { //move RIGHT side of bar UP
      motorR.move(STEP_INCR);
      barPosR--;
    }
    else if (userInputLeft < 0 && barPosR < FLOOR && barTilt > -MAX_BAR_TILT)
    { //move RIGHT side of bar DOWN
      motorR.move(-STEP_INCR);
      barPosR++;
    }
  }
  else //normal controls
  {

    if (userInputRight > 0 && barPosR > CEILING && barTilt < MAX_BAR_TILT)
    { //move right side of bar UP
      motorR.move(STEP_INCR);
      barPosR--;
    }
    else if (userInputRight < 0 && barPosR < FLOOR && barTilt > -MAX_BAR_TILT)
    { //move right side of bar DOWN
      motorR.move(-STEP_INCR);
      barPosR++;
    }

    if (userInputLeft > 0 && barPosL > CEILING && barTilt > -MAX_BAR_TILT)
    { //move left side of bar UP
      motorL.move(STEP_INCR);
      barPosL--;
    }
    else if (userInputLeft < 0 && barPosL < FLOOR && barTilt < MAX_BAR_TILT)
    { //move left side of bar DOWN
      motorL.move(-STEP_INCR);
      barPosL++;
    }
  }

  //accelstepper fxn. makes the motor go one step
  motorR.run();
  motorL.run();

  barTilt = barPosL - barPosR;

  //turn on LEDs for debugging
  if (abs(barTilt) == MAX_BAR_TILT)
  {
    digitalWrite(MAX_TILT_REACHED, HIGH);
  }
  else
  {
    digitalWrite(MAX_TILT_REACHED, LOW);
  }

  switch (barPosL)
  {
  case FLOOR:
    digitalWrite(L_FLOOR, HIGH);
    break;
  case CEILING:
    digitalWrite(L_CEIL, HIGH);
    break;
  default:
    digitalWrite(L_FLOOR, LOW);
    digitalWrite(L_CEIL, LOW);
  }

  switch (barPosR)
  {
  case FLOOR:
    digitalWrite(R_FLOOR, HIGH);
    break;
  case CEILING:
    digitalWrite(R_CEIL, HIGH);
    break;
  default:
    digitalWrite(R_FLOOR, LOW);
    digitalWrite(R_CEIL, LOW);
  }
}
#endif //END OF ACCELSTEPPER VERSION

//only for stepper.h - accelstepper version not implemented yet
#ifdef STEPPER_H
void resetBar()
{
  while (barPosL < BALL_RETURN_HEIGHT && barPosR < BALL_RETURN_HEIGHT)
  {
    motorL.step(-STEP_INCR);
    motorR.step(-STEP_INCR);
    barPosL++;
    barPosR++;
  }

  delay(BALL_RETURN_DELAY);

  while (barPosL > FLOOR && barPosR > FLOOR)
  {
    motorL.step(STEP_INCR);
    motorR.step(STEP_INCR);
    barPosL--;
    barPosR--;
  }
}
#endif
