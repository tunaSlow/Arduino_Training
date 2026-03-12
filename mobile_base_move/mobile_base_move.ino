/*
 * Step 1: Differential Drive Base Control
 * Architecture: Arduino UNO + L298N Motor Driver
 */

// Define Left Motor (Motor A) pins
const int ENA = 5;  // PWM speed control
const int IN1 = 7;  // Direction control 1
const int IN2 = 8;  // Direction control 2

// Define Right Motor (Motor B) pins
// Note: Using A0 and A1 as digital pins to save standard digital pins for sensors
const int ENB = 6;  // PWM speed control
const int IN3 = A0; // Direction control 1
const int IN4 = A1; // Direction control 2

// Set a base cruising speed (0-255)
const int cruiseSpeed = 150; 
int turnSpeed = 200;

void setup() {
  // Configure all motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize motors to a stopped state
  stopMotors();
}

void moveForward(int speed) {
  // Set Left Motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
  
  // Set Right Motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void loop() {
  delay(1500);
  moveCircle(99, 150);
}

// --- Kinematic Control Functions ---



void moveBackward(int speed) {
  // Set Left Motor backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
  
  // Set Right Motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

void turnRight(int speed) {
  // Left Motor forward, Right Motor backward (Point turn)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);
}

void turnLeft(int speed) {
  // Left Motor backward, Right Motor forward (Point turn)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void stopMotors() {
  // Cut power via PWM pins
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  
  // Set logic LOW for safety
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveCircle(int vl, int vr) {
  // Left Motor backward, Right Motor forward (Point turn)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, vl);
  
  // Set Right Motor backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, vr);
}

