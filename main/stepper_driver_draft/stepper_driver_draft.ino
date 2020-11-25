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

#define CEILING 200 //max value of bar postion
#define STEP_INCR 1000 //number of steps to move at each iteration
//at each iteration, either bar_pos++ or bar_pos--

char user_input;
int x; //looping variable. reused multiple times

/******** variables for motor 1 *******/
#define stp_1 2
#define dir_1 3
#define MS1_1 4
#define MS2_1 5
#define EN_1  6


int bar_pos_left = 0; //position of the left side of the bar in steps
//bar_pos_left = 0 means left side of bar is on floor

/***************************************/

/******** variables for motor 2 *******/

//once 1 motor is working, duplicate code for motor 2


/***************************************/

void setup() {
  // put your setup code here, to run once:
  pinMode(stp_1, OUTPUT);
  pinMode(dir_1, OUTPUT);
  pinMode(MS1_1, OUTPUT);
  pinMode(MS2_1, OUTPUT);
  pinMode(EN_1, OUTPUT);

  resetEDPins(); //Reset all 10 Easy Driver pins to default states

  Serial.begin(9600); //Open Serial connection for debugging. remove when merging.
  Serial.println("Enter number for control option:");
  Serial.println("1. Step forward 1 step.");
  Serial.println("2. Step backward 1 step.");
}


//the sparkfun example has 4 demo functions,
//but we only need "forward" and "backward"
//Assume "step forward" translates to "move the bar up a little bit"

//TODO: instead of using the serial monitor to control the motors, we'll need
//to check the values from our distance sensors to decide which motor and which direction to move
void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      digitalWrite(EN_1, LOW); //Pull enable pin low to allow motor control
      if (user_input =='1')
      {
        if (bar_pos_left < CEILING) {
         StepForwardDefault();
         bar_pos_left++;
        }
        //else do nothing
      }
      else if(user_input =='2')
      {
        if(bar_pos_left > 0) {
          ReverseStepDefault();
          bar_pos_left--;
        }
      } else {
        Serial.println("Invalid option entered.");
      }
      resetEDPins();
  }
}

//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp_1, LOW);
  digitalWrite(dir_1, LOW);
  digitalWrite(MS1_1, LOW);
  digitalWrite(MS2_1, LOW);
  digitalWrite(EN_1, HIGH);
}

//Default microstep mode function
void StepForwardDefault()
{
  Serial.println("Moving forward at default step mode.");
  digitalWrite(dir_1, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x<STEP_INCR; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp_1,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp_1,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}

//Reverse default microstep mode function
void ReverseStepDefault()
{
  Serial.println("Moving in reverse at default step mode.");
  digitalWrite(dir_1, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 0; x<STEP_INCR; x++)  //Loop the stepping enough times for motion to be visible
  {
    digitalWrite(stp_1,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stp_1,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}
