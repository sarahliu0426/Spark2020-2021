/* TO DO

Natalia and Ginny:
Add all-time high score functionality (Ginny)
- keep updating high score if player keeps beating it
- call 7 seg display code

- put regular score function on the 7 segment displays (GINNY)
- 1 set of 7 segment displays

Write ball return function to release special ball


Design "hand sanitizer" function to start/quit game
- ask mech where they want to put it
- add a way to "quit" the game

Powerdowns (ALEX AND NATALIA): 
Choose a new level based on jan 22's mtg mins chart
-Natlia = Bar reversal
holes 0-4: normal, 5 onwards is random

Write powerdown function to increase speed permanently [done]

Program LEDS for powerdowns

Bar movement mechanism:
- control motors to move bar: Janelle and Matt
  - program motor drivers for the stepper motors
  - program motors
- make sure bar is in starting position resetBar() (1 person)

- convert distance sensor distances to appropriate motor rotations
- prevent bar from hitting the ceiling or floor

Detect if ball fell into hole: Natalia
- assign pins for each sensor and check sensors for each hole in the function ballEntry()

Select new hole as a target hole: Alex (DONE)
- discuss option: for first 4 holes, choose easy ones, then choose random holes for the rest of the game

Ball Reset: Matt and Janelle
- Check with mech: how many balls? make sure ball is at bottom before loading ball
- Load ball when bar is at bottom
- servo motor?

Gameplay: Alex and Ginny
- make sure functions r being called in the right spot in main loop()
- determine inputs and outputs for each function
- fix playingGame bool 

Code to trigger "start the game": Natalia [done]
- if person waves their hands they start the game
- choose a part? IR sensor?

Set scores: Ginny (DONE)
- fine tune scoring
- function can be adjusted based on target hole difficulty

7 seg displays for scoring: Natalia

Determine who's protyping what and order parts: Alex and Janelle

Done:
- lights for holes

*/

/**********CONSTANTS*****************************/
#define SENSORPIN 4
#define BOTTOMPIN 13
#define SCOREINCREASE 10
const int echoPin = 9;
const int trigPin = 10;
#define NUMTARGETS 30
<<<<<<< HEAD
#include "SevSeg.h"
=======
//these can be changed and we need 
//two for each IR sensor
int IRSensor = 2; // connect ir sensor to arduino pin 2
int LED = 13; // conect Led to arduino pin 13


>>>>>>> ea5eb00 (Commiting to pull)
/************END OF CONSTANTS*********************/

/************GLOBAL VARIABLES**********************/
//declare any variables here that need to be remembered throughout the entire game

// time for sound wave to travel to object and bounce back
long duration;
// distance between sensor and object
int distance;


bool playingGame = true; //true if someone is playing, false if game over

int score = 0;
int targetDifficulty = 0
int highscore = 0;

int targetPin = 0;
bool targetBroken = false;
bool bottomBroken = false;

//global variables for timing 
int startTime = 0; //time when the new hole is assigned
unsigned long finishTime;  //time when the ball drops into target hole

int targetHoles[NUMTARGETS]; //sequential pin numbers of target holes, eg 0, 1, 2, 3...
/************END OF GLOBAL VARIABLES**********************/

SevSeg sevseg1;
SevSeg sevseg2;
SevSeg sevseg3;

void waitToStartGame() {
  //wait and do nothing until someone presses "start"
  //then continue with game loop


  //player places hand over one of the sensors to start
  //has to be high above sensor
  if (get_left_user_input == 1 || !get_right_user_input == 1) {
    //start the game
    playingGame = true;
    resetGame();
    
  }
  //otherwise do nothing
  //Serial.println("wait to start the game. pretend someone starts playing game");

}

void updateTarget() {
  
  randomSeed(analogRead(0));

  int targetIncr = (int)random(0,3);

  int oldTarget = targetPin;
  
  targetPin += targetIncr;

  if(targetPin > NUMTARGETHOLES){
    targetPin = targetIncr;
  }
  
  updateLights(oldTarget, targetPin);
}

void resetBar() {
  //given current position of bar, reset the bar
  //put the ball back onto the bar
  Serial.println("reset bar");
}

void resetBall() {
  //wait until ball is ready to roll onto the bar
  //put the ball back onto the bar
  Serial.println("reset ball");
  startTime = finishTime
}

void resetGame(){
    
    score = 0;
    targetPin = 0;
    targetDifficulty = 0;
    targetBroken = false;
    bottomBroken = false;
    resetBar();
    resetBall();
    finishTime = millis();
    updateScore();
    displayScore();
    digitalWrite(targetPin, HIGH);
}

/****** USER INPUT FUNCTIONS ****/
int smooth_distance (int num_samples) {
  int value = 0;
  int current_distance = 0;
  for (int i = 0; i < num_samples; i++) {
    current_distance = sample_distance();
    if (current_distance > 0 && current_distance < 25) {
      value = value + sample_distance();
      //Serial.print("Good Reading.\n");
    } else {
      //Serial.print("Bad Reading.");
      //take another sample of the data if there was a bad read
      //probably going to need to remove this
      //but it works well to test averages
      i--;
    }

  }

  return (value / num_samples);
}

int sample_distance() {
  int current_duration;
  int current_distance;
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  //wait max 50,000 microseconds before reading
  //may have to change this value
  current_duration = pulseIn(echoPin, HIGH, 50000);

  // Calculating the distance
  current_distance = current_duration * 0.034 / 2;

  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(current_distance);

  return current_distance;
}

/****** END OF USER INPUT FUNCTIONS ****/

void moveBar() {
  //get user input, ie "move left side down by X amount"

  //control motors to move bar
  //Need to check position of bar:
  //If bar is too tilted, don't move it
  //If bar is at top of game, don't move up anymore
  //If bar is at bottom of game, don't move down anymore

}
/************ BALL DETECTION FUNCTIONS *********/

bool beamBroken(int target)
{

  bool beamBroke = false;
  //#define SENSORPIN 4

  // variables will change:
  int sensorState = 0, lastState = 0;       // variable for reading the pushbutton status


  // initialize the LED pin as an output:


  //pinMode(LEDPIN, OUTPUT);

  // initialize the sensor pin as an input:
  //pinMode(target, INPUT);
  //digitalWrite(target, HIGH); // turn on the pullup

  // read the state of the pushbutton value:
  sensorState = digitalRead(target);

  // check if the sensor beam is broken
  // if it is, the sensorState is LOW:
  if (sensorState == LOW) {
    beamBroke = true;
  }
  else {
    beamBroke = false;
  }

  if (sensorState && !lastState) {
    Serial.println("Unbroken");
  }
  if (!sensorState && lastState) {
    Serial.println("Broken");
  }
  lastState = sensorState;

  return beamBroken;
}


void ballEntry() {

  targetBroken=beamBroken(targetPin);
  bottomBroken=beamBroken(BOTTOMPIN);

  int fellIntoTargetHole = true; //TODO: change to false
  int fellIntoBadHole = false; 

  if (targetBroken) { //ball fell in good hole
    resetBar();
    resetBall();

    //updateHighScore();
    updateTarget();

    updateScore();
    displayScore();
    
  } else if (bottomBroken) { //ball fell in bad hole  
    playingGame = false;
    resetGame();

  }

  resetBar();
}

/************ END OF BALL DETECTION FUNCTIONS ***********/

/************ START OF OUTPUT FUNCTIONS ***********/
void updateLights(int lastHole, int newHole){
  digitalWrite(lastHole, LOW)
  digitalWrite(newHole, HIGH)
}

//we can put this array somewhere else
//for the seven seg display
//we should probably wait until we know which display
//we're using to code this
int num_array[10][7] = {  { 1,1,1,1,1,1,0 },    // 0
                          { 0,1,1,0,0,0,0 },    // 1
                          { 1,1,0,1,1,0,1 },    // 2
                          { 1,1,1,1,0,0,1 },    // 3
                          { 0,1,1,0,0,1,1 },    // 4
                          { 1,0,1,1,0,1,1 },    // 5
                          { 1,0,1,1,1,1,1 },    // 6
                          { 1,1,1,0,0,0,0 },    // 7
                          { 1,1,1,1,1,1,1 },    // 8
                          { 1,1,1,0,0,1,1 }};   // 9
                         
void updateScore() {

    targetDifficulty += 1; // no powerups in the first 4 rounds

  if (targetDifficult <= 4 && (50 - (finishTime - startTime)/600) > 0) { 
    score += int(50 - (finishTime - startTime)/600); 
    // user gets 0 if they spend more than 30 secs 

  }
  else {
    score += int(50 - (finishTime - startTime)/2400); 
    // user gets 0 if they spend more than 120 secs 
  }
  delay(1000);
  displayScore();
  sethighScore();
}

void displayScore() {
  int firstDigit = score/100;
  int secondDigit = (score-firstDigit)/10;
  int thirdDigit = (score-firstDigit-secondDigit);

  sevseg1.setNumber(firstDigit);
  sevseg1.refreshDisplay(); 
  sevseg2.setNumber(secondDigit);
  sevseg2.refreshDisplay(); 
  sevseg3.setNumber(thirdDigit);
  sevseg3.refreshDisplay(); 
  
  
  //update digits on the seven seg display 
}

void sethighScore() {
  if (score > highscore) {
    highscore = score;
    int highFirst = high/100;
    int highSecond = (highscore-highFirst)/10;
    int highThird = (highscore-highFirst-highSecond);
  }
}
/************ END OF OUTPUT FUNCTIONS ***********/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Starts the serial communication

  for (int i = 0; i < NUMTARGETS; i++) {
    pinMode(i, INPUT); //IR receivers, one pin per target hole
    digitalWrite(i, HIGH); // turn on the pullup
    targetHoles[i] = i;
  }

  pinMode(8, OUTPUT);

  pinMode(echoPin, INPUT); //echoPin, for distance sensor
  pinMode(trigPin, OUTPUT); //trigggerPin, for distance sensor

  pinMode(11, OUTPUT); //IR LEDs. keep on at all times
  pinMode(12, OUTPUT);
  pinMode(BOTTOMPIN, INPUT); //IR receivers for all bad holes

  //fill targetHoles array with pin numbers

  //initialize motors
  //initialize user input devices
  //initialize displays

  //set up seven seg display x3
  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins1[] = {6, 5, 2, 3, 4, 7, 8, 9};
  byte segmentPins2[] = {6, 5, 2, 3, 4, 7, 8, 9};
  byte segmentPins3[] = {6, 5, 2, 3, 4, 7, 8, 9};
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_CATHODE; 
  sevseg1.begin(hardwareConfig, numDigits, digitPins, segmentPins1, resistorsOnSegments);
  sevseg1.setBrightness(90);
  sevseg2.begin(hardwareConfig, numDigits, digitPins, segmentPins2, resistorsOnSegments);
  sevseg2.setBrightness(90);
  sevseg3.begin(hardwareConfig, numDigits, digitPins, segmentPins3, resistorsOnSegments);
  sevseg3.setBrightness(90);
  
  sevseg1.setNumber(0);
  sevseg1.refreshDisplay(); 
  sevseg2.setNumber(0);
  sevseg2.refreshDisplay(); 
  sevseg3.setNumber(0);
  sevseg3.refreshDisplay(); 
  
  resetBar();
}
/************ START OF POWER DOWN FUNCTIONS ***********/




//NatatiasFUNCTION GOES HERE



/************ END OF POWER DOWN  FUNCTIONS ***********/
void loop() {

  resetGame(); //sets bar, ball, score, lights for initial game start
  waitToStartGame(); //wait for user to start game and set PlayingGame = true

  while (playingGame) {
    //user input: just for testing purposes
    //distance = smooth_distance(20);
    //Serial.print("Distance: ");
    //Serial.println(distance);

    finishTime = millis(); //might not need here depending on when updateTarget is called. 

    get_left_user_input();
    get_right_user_input();
    moveBar();
    ballEntry();
    updatescore();

  }
}
