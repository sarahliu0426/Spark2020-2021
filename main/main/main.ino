/* TO DO
Bar movement mechanism:
- control motors to move bar: Janelle and Matt
  - program motor drivers for the stepper motors
  - program motors
- make sure bar is in starting position resetBar() (1 person)

- convert distance sensor distances to appropriate motor rotations
- prevent bar from hitting the ceiling or floor

Detect if ball fell into hole: Natalia
- assign pins for each sensor and check sensors for each hole in the function ballEntry()

Select new hole as a target hole: Alex (almost done)
- discuss option: for first 4 holes, choose easy ones, then choose random holes for the rest of the game

Ball Reset: Matt and Janelle
- Check with mech: how many balls? make sure ball is at bottom before loading ball
- Load ball when bar is at bottom
- servo motor?

Gameplay: Alex and Ginny
- make sure functions r being called in the right spot in main loop()
- determine inputs and outputs for each function
- fix playingGame bool 

Code to trigger "start the game": Natalia
- if person waves their hands they start the game
- choose a part? IR sensor?

Set scores: Ginny
- fine tune scoring

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

/************END OF CONSTANTS*********************/

/************GLOBAL VARIABLES**********************/
//declare any variables here that need to be remembered throughout the entire game

// time for sound wave to travel to object and bounce back
long duration;
// distance between sensor and object
int distance;


bool playingGame = true; //true if someone is playing, false if game over
bool wonGame = false;

int score = 0;
int targetPin = 0;
bool targetBroken = false;
bool bottomBroken = false;

//global variables for timing 
unsigned long startTime;  //time when the new hole is assigned 

int targetHoles[NUMTARGETS]; //sequential pin numbers of target holes, eg 0, 1, 2, 3...
/************END OF GLOBAL VARIABLES**********************/


void waitToStartGame() {
  //wiat and do nothing until someone presses "start"
  //then continue with game loop
  
  Serial.println("wait to start the game. pretend someone starts playing game");
}

void checkIfGameOver () {
  //check if user "won" or "lost"
  //if user hasn't moved for a while, we assume they walked away and end the game
  Serial.println("check if game over");
}


void updateTarget() {
  
  randomSeed(analogRead(0));

  int targetIncr = (int)random(0,3);

  int oldTarget = targetPin;
  
  targetPin += targetIncr;

  if(targetPin > NUMTARGETHOLES) 
    targetPin = targetIncr;
  
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
}

void resetGame(){
    resetBar();
    resetBall();
    score = 0;
    updateScore(millis());

    //TODO: Need a way to see when a new game has started and set the startTime
    //TO CONSIDER: we may want to add in a pushbutton that starts the game 
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

  //1 is up 0 is down
  bool left_direction = get_left_user_input();
  bool right_direction = get_right_user_input();
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

  int fellIntoTargetHole = true; //TODO: change to false
  int fellIntoBadHole = false;

  if (targetBroken) { //ball fell in good hole
    resetBar();
    resetBall();

    //updateHighScore();
    updateTarget();

    updateScore(millis());
    
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
                         
void updateScore(unsigned long finishTime){
    unsigned long completionTime = finishTime-startTime; 
    score += 12 - (int)(completionTime*10)%12; 
    
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

  resetBar();
}

void loop() {
  // put your main code here, to run repeatedly:
  waitToStartGame();
  resetBar();
  resetBall();
  score = 0;

  targetPin = 0; //remove when finished updateTarget()
  //updateTarget(); //choose 1st target

  //dummy counter for mimicking a game loop
  int dummyCounter = 0;

  while (playingGame) {
    //user input: just for testing purposes
    //distance = smooth_distance(20);
    //Serial.print("Distance: ");
    //Serial.println(distance);

    startTime = millis(); //might not need here depending on when updateTarget is called. 
    moveBar();
    ballEntry();
    checkIfGameOver();
    targetBroken=beamBroken(targetPin);
    bottomBroken=beamBroken(BOTTOMPIN);


    if (dummyCounter > 10) {
      playingGame = false;
    }
    dummyCounter++;
  }

  resetBar();
  //updateScore();
  //turnOffAllLights();
}
