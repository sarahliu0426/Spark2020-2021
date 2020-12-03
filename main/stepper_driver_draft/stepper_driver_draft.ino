/***************************
  Code was modified from:

  SparkFun Easy Driver Basic Demo
  Toni Klopfenstein @ SparkFun Electronics
  March 2015
  Arduino 1.6.0

  https://github.com/sparkfun/Easy_Driver
  Simple demo sketch to demonstrate how 5 digital pins can drive a bipolar stepper motor,
  using the Easy Driver (https://www.sparkfun.com/products/12779). Also shows the ability to change
  microstep size, and direction of motor movement.

  Example based off of demos by Brian Schmalz (designer of the Easy Driver).
  http://www.schmalzhaus.com/EasyDriver/Examples/EasyDriverExamples.html

  The following code is for 1 stepper driver
  1 stepper driver can drive 1 motor and needs 5 arduino pins.
  We'll need to duplicate this code, and use 10 pins in total to drive 2 motors.
 ***************************/

//TODO: change pins to the actual pins we're using

//EXPERIMENTAL VALUES - will need to adjust as we prototype
#define CEILING 200 //max value of bar postion
#define FLOOR 3 //min value of bar position. include a bit of space so it won't hit the actual floor
#define STEP_INCR 1000 //number of steps to move at each iteration

//distance sensors will give the motors a number within the range [-3, 3]
//-ve speed means "down", +ve speed means "up"
#define STOP 0
#define SLOW 1
#define MED 2
#define FAST 3

//used to control delay. (MAXDELAY - FAST) gives a delay of 1
//which is faster then (MAXDELAY - SLOW), which gives a delay of 3
#define MAXDELAY 4

int userInputLeft; //from distance sensor functions
int userInputRight;

int x; //looping variable. reused multiple times

#define MAX_BAR_TILT 10 //maximum vertical slope of bar, aka barPosRight - barPosLeft
int barTilt = 0;

/******** variables for motor 1 *******/
#define stp_1 2
#define dir_1 3
#define MS1_1 4
#define MS2_1 5
#define EN_1  6
int barPosLeft = 0; //position of the left side of the bar in steps

/***************************************/

/******** variables for motor 2 *******/

//once 1 motor is working, duplicate code for motor 2
int barPosRight = 0;

/***************************************/

void setup() {
  // put your setup code here, to run once:

  //left motor setup
  pinMode(stp_1, OUTPUT);
  pinMode(dir_1, OUTPUT);
  pinMode(MS1_1, OUTPUT);
  pinMode(MS2_1, OUTPUT);
  pinMode(EN_1, OUTPUT);

  //TODO: right motor setup

  resetEDPins(); //Reset all 10 Easy Driver pins to default states

  Serial.begin(9600); //Open Serial connection for debugging. remove when merging.
  Serial.println("Enter number for control option:"); //LEFT MOTOR ONLY
  //assume user enters 1 for normal, 2 for fast, 3 for fastest speed upwards
  //-1, -2, and -3 move the bar downwards
}

//TODO: instead of using the serial monitor to control the motors, we'll need
//to check the values from our distance sensors to decide which motor and which direction to move

void loop() {
  // FOR LEFT MOTOR ONLY
  while (Serial.available()) {

    userInputLeft = Serial.parseInt(); //Read user input and trigger appropriate function
    Serial.print("Received: ");
    Serial.print(userInputLeft);
    Serial.println();
    
    digitalWrite(EN_1, LOW); //Pull enable pin low to allow motor control

    if (userInputLeft > 0
        && barPosLeft < CEILING
        && barTilt > -MAX_BAR_TILT) {       //move left side of bar UP
      moveStepper(true, dir_1, stp_1, MAXDELAY - userInputLeft);
      barPosLeft += userInputLeft;
    } else if (userInputLeft < 0
               && barPosLeft > FLOOR
               && barTilt < MAX_BAR_TILT) {     //move left side of bar DOWN
      moveStepper(false, dir_1, stp_1, MAXDELAY - userInputLeft);
      barPosLeft -= userInputLeft;
    }

    barTilt = barPosRight - barPosLeft;
    
    Serial.print("left bar pos = ");
    Serial.print(barPosLeft);
    Serial.println();
    Serial.print("bar tilt = ");
    Serial.print(barTilt);
    Serial.println();
    
    resetEDPins();
  }
}

//Reset Easy Driver pins to default states
//TODO: reset right motor pins
void resetEDPins()
{
  digitalWrite(stp_1, LOW);
  digitalWrite(dir_1, LOW);
  digitalWrite(MS1_1, LOW);
  digitalWrite(MS2_1, LOW);
  digitalWrite(EN_1, HIGH);
}

void moveStepper(bool goingUp, int dirPin, int stpPin, int stepperDelay)
{
  Serial.print("Moving stepper ");

  if (goingUp) {
    Serial.print("up with delay ");
    digitalWrite(dirPin, LOW); //Pull direction pin low to move "forward"
  } else {
    Serial.print("down with delay ");
    digitalWrite(dirPin, HIGH); //Pull direction pin high to move "backward"
  }

  Serial.print(stepperDelay, DEC);
  Serial.println();
  
  for (x = 0; x < STEP_INCR; x++) //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stpPin, HIGH); //Trigger one step forward
    delay(stepperDelay);
    digitalWrite(stpPin, LOW); //Pull step pin low so it can be triggered again
    delay(stepperDelay);
  }
  Serial.println("Enter new option");
  Serial.println();
}
