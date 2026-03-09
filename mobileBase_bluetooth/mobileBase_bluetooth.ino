/*
 * Step 4: Bluetooth Teleoperation
 * Architecture: Arduino UNO + L298N + HC-05
 */

#include <SoftwareSerial.h>

// --- Step 1 Pin Definitions (Motors) ---
const int ENA = 5, IN1 = 7, IN2 = 8;
const int ENB = 6, IN3 = A0, IN4 = A1;

// --- Step 4 Pin Definitions (Bluetooth) ---
const int rxPin = 2; // Connects to HC-05 TX
const int txPin = 3; // Connects to HC-05 RX (Use Voltage Divider!)

// Initialize SoftwareSerial
SoftwareSerial BTSerial(rxPin, txPin);

// Control parameters
const int driveSpeed = 200;
char command = 'S'; // Default state is stop

void setup() {
  // Motor Setup
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  
  // Hardware Serial for debugging via USB
  Serial.begin(9600);
  
  // Software Serial for HC-05 communication
  BTSerial.begin(9600); 
  
  Serial.println("System Ready. Awaiting Bluetooth Connection...");
  stopMotors();
}

void loop() {
  // Check if data is available in the Bluetooth buffer
  if (BTSerial.available()) {
    command = BTSerial.read();
    Serial.print("Received Command: ");
    Serial.println(command);
    
    // Command Parser execution
    executeCommand(command);
  }
}

void executeCommand(char cmd) {
  switch (cmd) {
    case 'F': // Forward
      moveForward(driveSpeed);
      break;
    case 'B': // Backward
      moveBackward(driveSpeed);
      break;
    case 'L': // Left
      turnLeft(driveSpeed);
      break;
    case 'R': // Right
      turnRight(driveSpeed);
      break;
    case 'S': // Stop
      stopMotors();
      break;
    default:
      // Ignore invalid characters
      break;
  }
}

// --- Kinematic Functions (From Step 1) ---
void moveForward(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, speed);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, speed);
}
void moveBackward(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, speed);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, speed);
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