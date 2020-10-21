//jotted down some ideas. we'll decide what kinds of arguments to pass to the functions soon

/************GLOBAL VARIABLES**********************/
//declare any variables here that need to be remembered throughout the entire game

/************END OF GLOBAL VARIABLES**********************/
void waitToStartGame() {
  //wiat and do nothing until someone presses "start"
  //then continue with game loop
}

void checkIfGameOver () {
//check if user "won" or "lost"
//if user hasn't moved for a while, we assume they walked away and end the game
}

void resetBar(int barPosLeft, int barPosRight) {
  //reset the bar
  //put the ball back onto the bar
  
}

void resetBall() {
  //wait until ball is ready to roll onto the bar
  //put the ball back onto the bar
}

void checkForUserInput(distSensorVal) {

//put your code here


//possible output:
//tell left motor to go up, down, or don't move, by X amount
//right motor to go up, down, or don't move, by Y amount
}

void moveBar() {
  //get user input, ie "move left side down by X amount"
  
  //control motors to move bar
//Need to check position of bar:
//If bar is too tilted, don't move it
//If bar is at top of game, don't move up anymore
//If bar is at bottom of game, don't move down anymore

}

void ballEntry() {
//  If ball falls into a bad hole
//    End the game

//If ball falls into target hole
//  Increment and display score
//  increment the score
//  Start the next level
//  Choose a new target hole
  
  //TODO for next week:
  //choose an input sensor (IR vs ultrasonic distance vs reed switch vs limit switch)
  //based on sensor, write code to determine if ball got close enough === fell into that hole
  //need 1 sensor for bottom of gameboard: if ball got to bottom of the gameboard, and didn't trigger a target hole, it went into a bad hole
  
  updateScore();
  resetBar();
}

void updateScore(int newScore) {
  //change value of score
  //change the score display
}

void updateTarget(int prevTarget) {
  //change the new target hole based on the last one
  //decide if we want pre-specified "levels"
  //turn off any lights on the old target hole
  //make the new target hole light up
}

void setup() {
  // put your setup code here, to run once:

  resetBar();

  //initialize motors
  //initialize user input devices
  //initialize displays
  
}

void loop() {
  // put your main code here, to run repeatedly:
  waitToStartGame();
  resetBar();
  resetBall();
  updateScore(0);
  updateTarget(target # 1);
  
  while(ball still alive) {
    checkForUserInput();
    moveBar();
    ballEntry();
    checkIfGameOver();
  }

  resetBar();
  updateScore();
  updateHighScore();
  turnOffAllLights();
}
