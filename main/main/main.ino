//jotted down some ideas. we'll decide what kinds of arguments to pass to the functions soon

void resetBar() {
  //reset the bar
  //put the ball back onto the bar
  
}

void updateScore() {
  //change value of score
  //change the score display
}

void moveBar() {
  //get user input
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
//  Bring bar to bottom of game
//  Put ball back onto bar
//  increment the score
//  Start the next level
//  Choose a new target hole
  updateScore();
  resetBar();
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

  moveBar();
  ballEntry();

}
