#include "RunningAverage.h"

// defines pins numbers, doesn't have to be 9 and 10
//any d i/o works fine
int trigPin1 = 10;
int echoPin1 = 9;

//corresponds to the RIGHT sensor
int trigPin2 = 8;
int echoPin2 = 7;

//corresponds to start game sensor
int trigPin3 = 6;
int echoPin2 = 5;

RunningAverage left_sensor(20);
RunningAverage right_sensor(20);
RunningAverage start_sensor(20);

float sample_distance_right() {
  int current_duration;
  int current_distance;
  
  // Clears the trigPin
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  //wait max 50,000 microseconds before readingt
  //may have to change this value

  //we can also switch the pin given in these functions to switch which side 
  //of the bar each sensor corresponds to... >:))
  current_duration = pulseIn(echoPin2, HIGH, 50000);
  
  // Calculating the distance
  current_distance = current_duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(current_distance);

  return current_distance;
}

//get a distance from the sensor
float sample_distance_left() {
  int current_duration;
  int current_distance;
  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
 
  current_duration = pulseIn(echoPin1, HIGH, 50000);
  current_distance = current_duration*0.034/2;

  return current_distance;
}

float start_game_distance() {
  int current_duration;
  int current_distance;
  
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
 
  current_duration = pulseIn(echoPin3, HIGH, 50000);
  current_distance = current_duration*0.034/2;

  return current_distance;
}

int get_left_user_input(){
  left_sensor.addValue(start_game_distance());
  //Serial.print("Left Distance: ");
  //Serial.println(left_sensor.getAverage());
  //Serial.print("\n");
  if (sped_up && left_sensor.getAverage() > 15) {
    return 3;
  }
  if (left_sensor.getAverage() > 25) {
     return 3;
  } else if (left_sensor.getAverage() > 15) {
    return 2;
  }
  return 1;
}

int get_right_user_input() {
  right_sensor.addValue(sample_distance_right());
  //Serial.print("\nRight Distance: ");
  //Serial.println(right_sensor.getAverage());
  if (sped_up && right_sensor.getAverage() > 15) {
    return 3;
  }
  if (right_sensor.getAverage() > 25) {
     return 3;
  } else if (right_sensor.getAverage() > 15) {
    return 2;
  }
  return 1;
}

int start_game_input(){
  start_sensor.addValue(start_game_distance());
  if (start_sensor.getAverage() < 15) {
    return 1;
  }
  return 0;
}

//setup to be run once
void setup() {
  //set up pins
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  //clear running averages
  left_sensor.clear();
  right_sensor.clear();
  start_sensor.clear();
  Serial.begin(9600); // Starts the serial communication
}

//to be repeated
void loop() {
  //just for testing purposes
  int right_sensor_read = get_right_user_input();
  int left_sensor_read = get_left_user_input();
  int start_sensor_read = start_game_input();
  
  //Serial.print(right_sensor_read);
  //Serial.print(left_sensor_read);
}
