
// defines pins numbers, doesn't have to be 9 and 10
//any d i/o works fine
const int trigPin = 10;
const int echoPin = 9;

// time for sound wave to travel to object and bounce back
long duration;
// distance between sensor and object
int distance;

//value of average distance
int average_distance = 0;
int average_ready = false;

//setup to be run once
void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
}

//take the average of a certain number of sensor samples
int smooth_distance(int num_samples) {
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
  return (value/num_samples);
}

//get a distance from the sensor
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
  current_distance = current_duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(current_distance);

  return current_distance;
}



//to be repeated
void loop() {
  //just for testing purposes
  distance = smooth_distance(20);
  Serial.print("Distance: ");
  Serial.println(distance);
}
