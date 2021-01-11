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
[barPosL = 600       barPosR = 600]
        (bottom of display)

  References:

  AccelStepper Library Documentation
  https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html

  Makerguides A4988 Motor Driver Tutorial
  - includes tips about setting up hardware as well as step-by-step code
  https://www.makerguides.com/a4988-stepper-motor-driver-arduino-tutorial/

  Tutorial for smaller 5V 28byj48 stepper motors
  https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/

  SparkFun Easy Driver Basic Demo
  - we are no longer using this driver, but the pin setup is similar
  https://github.com/sparkfun/Easy_Driver

 ***************************/

#include <AccelStepper.h>
#include <Stepper.h>
#include <MultiStepper.h>

//EXPERIMENTAL VALUES - will need to adjust as we prototype
#define CEILING 0                //highest height of bar
#define FLOOR 10000              //bottom of the playing area, not actually the floor
#define BALL_RETURN_HEIGHT 11000 //lowest height of bar, where bar will pick up ball
#define MAX_BAR_TILT 2000        //maximum vertical slope of bar, aka barPosRight - barPosLeft
#define MAX_SPEED 5
#define SPEED_MULT 1 //multiply user input value with this number to set desired stepper speed
#define STEP_INCR 1   //steps taken on each loop() iteration
#define BALL_RETURN_DELAY 2000 //time to wait until a new ball has rolled onto bar
//distance sensors code will give the motors a number within the range [-3, 3]
//-ve speed means "down", +ve speed means "up"
#define STOP 0
#define SLOW 1
#define MED 2
#define FAST 3

//without AccelStepper:
// #define STEP_INCR 50 //number of steps to move at each iteration
// #define MAXDELAY 4
//used to control delay. eg (MAXDELAY - FAST) gives a delay of 1
//which is faster than (MAXDELAY - SLOW), which gives a delay of 3

#define STEPS_PER_REV 2048 //only for 28byj stepper with ULN2003 driver

int userInputLeft; //from distance sensor functions
int userInputRight;
int barPosL = FLOOR;
int barPosR = FLOOR;

int barTilt = 0;

//setup motor pins
#define RT_COIL_1A 10
#define RT_COIL_1B 11
#define RT_COIL_2A 12
#define RT_COIL_2B 13
Stepper motorR = Stepper(STEPS_PER_REV, RT_COIL_1A, RT_COIL_1B, RT_COIL_2A, RT_COIL_2B);
// AccelStepper accelMotorR(AccelStepper::FULL4WIRE, RT_COIL_1A, RT_COIL_1B, RT_COIL_2A, RT_COIL_2B);

#define LT_COIL_1A 4
#define LT_COIL_1B 5
#define LT_COIL_2A 6
#define LT_COIL_2B 7
Stepper motorL = Stepper(STEPS_PER_REV, LT_COIL_1A, LT_COIL_1B, LT_COIL_2A, LT_COIL_2B);
// AccelStepper accelMotorL(AccelStepper::FULL4WIRE, LT_COIL_1A, LT_COIL_1B, LT_COIL_2A, LT_COIL_2B);

/***************************************/

////To make both motors arrive at the same position at the same time
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
//end of prototyping variables

void setup()
{
  // bothSteppers.addStepper(accelMotorR);
  // bothSteppers.addStepper(accelMotorL);
  //   accelMotorR.setMaxSpeed(MAX_SPEED);
  // accelMotorL.setMaxSpeed(MAX_SPEED);

  motorR.setSpeed(MAX_SPEED);
  motorL.setSpeed(MAX_SPEED);


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

  barPosL = FLOOR;
  barPosR = FLOOR;

  resetBar(); 

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

  //if button pressed, toggle left and right controls
  if (digitalRead(SWAP) == LOW && prevSwapReading == HIGH)
  {
    swapControls = !swapControls;
  }
  prevSwapReading = digitalRead(SWAP);

  moveSteppers();
}

void moveSteppers() {
  //Move motors
  if (swapControls) //special level: SWAP right and left controls
  {
//    motorR.setSpeed(userInputLeft * SPEED_MULT);
//    motorL.setSpeed(userInputRight * SPEED_MULT);

    if (userInputRight > 0 && barPosL > CEILING && barTilt > -MAX_BAR_TILT)
    { //move LEFT side of bar UP
      motorL.step(-STEP_INCR);
      barPosL--;
    }
    else if (userInputRight < 0 && barPosL < FLOOR && barTilt < MAX_BAR_TILT)
    { //move LEFT side of bar DOWN
      motorL.step(STEP_INCR);
      barPosL++;
    }

    if (userInputLeft > 0 && barPosR > CEILING && barTilt < MAX_BAR_TILT)
    { //move RIGHT side of bar UP
      motorR.step(-STEP_INCR);
      barPosR--;
    }
    else if (userInputLeft < 0 && barPosR < FLOOR && barTilt > -MAX_BAR_TILT)
    { //move RIGHT side of bar DOWN
      motorR.step(STEP_INCR);
      barPosR++;
    }
  }
  else //normal controls
  {
//    motorR.setSpeed(userInputRight * SPEED_MULT);
//    motorL.setSpeed(userInputLeft * SPEED_MULT);

    if (userInputRight > 0 && barPosR > CEILING && barTilt < MAX_BAR_TILT)
    { //move right side of bar UP
      motorR.step(-STEP_INCR);
      barPosR--;
    }
    else if (userInputRight < 0 && barPosR < FLOOR && barTilt > -MAX_BAR_TILT)
    { //move right side of bar DOWN
      motorR.step(STEP_INCR);
      barPosR++;
    }

    if (userInputLeft > 0 && barPosL > CEILING && barTilt > -MAX_BAR_TILT)
    { //move left side of bar UP
      motorL.step(-STEP_INCR);
      barPosL--;
    }
    else if (userInputLeft < 0 && barPosL < FLOOR && barTilt < MAX_BAR_TILT)
    { //move left side of bar DOWN
      motorL.step(STEP_INCR);
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

void resetBar()
{
  while(barPosL < BALL_RETURN_HEIGHT && barPosR < BALL_RETURN_HEIGHT) {
    motorL.step(STEP_INCR);
    motorR.step(STEP_INCR);
    barPosL++;
    barPosR++;
  }

  delay(BALL_RETURN_DELAY);

  while(barPosL > FLOOR && barPosR > FLOOR) {
    motorL.step(-STEP_INCR);
    motorR.step(-STEP_INCR);
    barPosL--;
    barPosR--;
  }
  
}
