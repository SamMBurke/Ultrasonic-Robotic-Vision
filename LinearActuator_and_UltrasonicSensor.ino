#include <Servo.h>

Servo ServoX;
Servo ServoY;

#define PIN_X (3)
#define PIN_Y (2)
#define echoPin 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5 //attach pin D3 Arduino to pin Trig of HC-SR04
#define resolution 10
#define delayS 4

// Define global variables
long duration; // variable for the duration of sound wave travel
double distance; // variable for the distance measurement
double counter;

int stepSizeX = 100/resolution;
int stepSizeY = stepSizeX;

int delayMS = delayS * 1000;

//int distanceMatrix[256];  // maximum size of an array?

bool isExtending = true;

// Sets x-direction linear actuator to a percentage of the total distance
void SetStrokePercX(float strokePercentageX)
{
  if(strokePercentageX < 1)
    strokePercentageX = 1;
  else if (strokePercentageX > 99)
    strokePercentageX == 99;
    
  int usecX = 1000 + strokePercentageX * (2000-1000)/100; 
  ServoX.writeMicroseconds(usecX);  // Apply a frequency with pulsewidth usecx to move servo motor set distance
}

// Sets y-direction linear actuator to a percentage of the total distance
void SetStrokePercY(float strokePercentageY)
{
  if(strokePercentageY < 1)
    strokePercentageY = 1;
  else if (strokePercentageY > 99)
    strokePercentageY == 99;
    
  int usecY = 1000 + strokePercentageY * (2000-1000)/100; // Acquire the pulsewidth in micro seconds
  ServoY.writeMicroseconds(usecY);  // Apply a frequency with pulsewidth usecx to move servo motor set distance
} 

// Read value from the ultrasonic sensor and calculate the distance measured between the sensor and the object | float
void GetDistance() {
      // Clears the trigPin condition
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      
      // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin, HIGH);
      
      // Calculating the distance
      distance = duration * 0.03436426117 / 2; // Speed of sound wave (cm/us) divided by 2 (account for forward and back travel

      // Delay the increase of a counter variable
      delay(100);
      counter += 0.1;
      
      Serial.println(distance);
//      return counter;
}

void setup() {
  ServoX.attach(PIN_X);
  ServoY.attach(PIN_Y);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  
  Serial.begin(9600);
  Serial.setTimeout(1);
}

void loop() {
  // Reset position of the linear actuators
  SetStrokePercX(1);
  SetStrokePercY(1);
  delay(15000); // Provide the linear actuators enough time to reach the starting positions regardless of their position at the start of runtime
  
  for(int x = 1; x < 101; x += stepSizeX){
    // Move X linear actuator to some percentage of the full extendable distance
    SetStrokePercX(x);
    delay(delayMS);           // Delay the programming for the minimum amount of time it takes for actuator X to move to position x% of the full distance
    counter = 0;

    // Indicative that the actuator is at the starting position 0, thus extend
    if(isExtending) {
      SetStrokePercY(99);     // Extend the linear actuator to 99% of the full length

      while(isExtending) {
        GetDistance();        // Measure the distance from the ultrasonic sensor
        if(counter >= 13) {   // 13 seconds have passed and the linear actuator should be at the full distance
          isExtending = false;
        }
      }
    } else {  // Indicative that the actuator is at the full extended length, thus retract
      SetStrokePercY(1);      // Retract the linear actuator to 1% of the full length

      while(!isExtending) {
        GetDistance();        // Measure the distance from the ultrasonic sensor
        if(counter >= 13) {   // 13 seconds have passed and the linear actuator should be at the full distance
          isExtending = true;
        }
      }
    }
  }

  // Return linear actuators to starting position and exit the program
  SetStrokePercX(1);
  SetStrokePercY(1);  
  delay(15000);
  exit(0);
}
