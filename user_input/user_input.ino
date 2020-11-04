//Need to make these rolling averages


// defines pins numbers, doesn't have to be 9 and 10
//any d i/o works fine
const int trigPin1 = 10;
const int echoPin1 = 9;

//corresponds to the RIGHT sensor
const int trigPin2 = 8;
const int echoPin2 = 7;

// distance between sensor and object
int left_distance;
int right_distance;


//setup to be run once
void setup() {
pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
}

//take the average of a certain number of sensor samples
int smooth_distance_left(int num_samples) {
  //all values from left sensor
  int left_value = 0;
  
  int current_left_distance = 0;
  
  for (int i = 0; i < num_samples; i++) {
    current_left_distance = sample_distance_left();
    if (current_left_distance > 0 && current_left_distance < 25) {
      left_value = left_value + sample_distance_left();
      //Serial.print("Good Reading.\n");
    } else {
      //Serial.print("Bad Reading.");
      //take another sample of the data if there was a bad read
      //probably going to need to remove this
      //but it works well to test averages
      i--;
    }
    
  }
  return (left_value/num_samples);
}

int smooth_distance_right(int num_samples) {
 //all values from right sensor
  int right_value = 0;
  
  int current_right_distance = 0;
  for (int i = 0; i < num_samples; i++) {
    current_right_distance = sample_distance_right();
    if (current_right_distance > 0 && current_right_distance < 25) {
      right_value = right_value + sample_distance_right();
      //Serial.print("Good Reading.\n");
    } else {
      //Serial.print("Bad Reading.");
      //take another sample of the data if there was a bad read
      //probably going to need to remove this
      //but it works well to test averages
      i--;
    }
    
  }
  return (right_value/num_samples);
}

//get a distance from the sensor
int sample_distance_left() {
  int current_duration;
  int current_distance;
  // Clears the trigPin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  //wait max 50,000 microseconds before reading
  //may have to change this value

  //we can also switch the pin given in these functions to switch which side 
  //of the bar each sensor corresponds to... >:)
  current_duration = pulseIn(echoPin1, HIGH, 50000);
  
  // Calculating the distance
  current_distance = current_duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(current_distance);

  return current_distance;
}

int sample_distance_right() {
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
  //wait max 50,000 microseconds before reading
  //may have to change this value

  //we can also switch the pin given in these functions to switch which side 
  //of the bar each sensor corresponds to... >:)
  current_duration = pulseIn(echoPin2, HIGH, 50000);
  
  // Calculating the distance
  current_distance = current_duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(current_distance);

  return current_distance;
}


//to be repeated
void loop() {
  //just for testing purposes
  left_distance = smooth_distance_left(20);
  right_distance = smooth_distance_right(20);
  Serial.print("Left Distance: ");
  Serial.println(left_distance);
  Serial.print("\nRight Distance: ");
  Serial.println(right_distance);
}
