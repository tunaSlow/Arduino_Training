/*
 * Step 2: Obstacle Avoidance Integration
 * Architecture: Arduino UNO + L298N + HC-SR04 + SG90
 */

#include <Servo.h>

// --- Step 1 Pin Definitions ---
const int ENA = 5, IN1 = 7, IN2 = 8;
const int ENB = 6, IN3 = A0, IN4 = A1;

// --- Step 2 Pin Definitions ---
const int trigPin = 11;
const int echoPin = 12;
const int servoPin = 4;

Servo sensorServo; // Create servo object
const int safeDistance = 25; // Minimum clearance in cm
const int cruiseSpeed = 150;
const int turnSpeed = 180; // Slightly higher speed to overcome friction during point turns

void setup() {
  // Motor Setup
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  
  // Sensor Setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Servo Setup
  sensorServo.attach(servoPin);
  sensorServo.write(90); // Initialize looking straight ahead
  delay(1000); // Allow time for mechanical movement
}

void loop() {
  int distanceFront = getDistance();

  if (distanceFront > safeDistance && distanceFront != 0) {
    // Path is clear, proceed
    moveForward(cruiseSpeed);
  } else {
    // Obstacle detected, execute avoidance sequence
    stopMotors();
    delay(300); // Stabilize chassis
    
    // Scan environment
    int distanceRight = lookRight();
    int distanceLeft = lookLeft();
    
    // Return sensor to center
    sensorServo.write(90);
    delay(300);

    // Decision matrix
    if (distanceRight >= distanceLeft) {
      turnRight(turnSpeed);
      delay(400); // Adjust delay based on your robot's turning friction
    } else {
      turnLeft(turnSpeed);
      delay(400); 
    }
    stopMotors();
    delay(200); // Stabilize before resuming loop
  }
}

// --- Sensor Functions ---

int getDistance() {
  // Send 10us HIGH pulse to trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin pulse duration (in microseconds)
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout prevents infinite blocking

  // Calculate distance in cm
  int distance = duration * 0.034 / 2;
  
  // Return 999 if timeout (out of range) to prevent false positives
  if (distance == 0) return 999; 
  return distance;
}

int lookRight() {
  sensorServo.write(30); // Look 60 degrees right
  delay(500); // Wait for servo to reach position
  int dist = getDistance();
  return dist;
}

int lookLeft() {
  sensorServo.write(150); // Look 60 degrees left
  delay(500); 
  int dist = getDistance();
  return dist;
}

// --- Kinematic Functions (From Step 1) ---
void moveForward(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, speed);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, speed);
}
void turnRight(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, speed);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, speed);
}
void turnLeft(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, speed);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, speed);
}
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); analogWrite(ENA, 0);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); analogWrite(ENB, 0);
}