/*
 * Master Integration: Teleoperation + Autonomous Safety Override
 * Architecture: Arduino UNO + L298N + HC-05 + HC-SR04 + SG90
 */

#include <SoftwareSerial.h>
#include <Servo.h>

// --- Pin Definitions ---
// Motors
const int ENA = 5, IN1 = 7, IN2 = 8;
const int ENB = 6, IN3 = A0, IN4 = A1;

// Bluetooth
const int rxPin = 2; // Connects to HC-05 TX
const int txPin = 3; // Connects to HC-05 RX (Voltage Divider)
SoftwareSerial BTSerial(rxPin, txPin);

// Sensors & Actuators
const int trigPin = 11;
const int echoPin = 12;
const int servoPin = 4;
Servo sensorServo;

// --- System Parameters ---
const int driveSpeed = 200;
const int safeDistance = 25; // Stop threshold in cm
char currentCommand = 'S';   // Default state is stop

void setup() {
  // Initialize Hardware
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);
  
  sensorServo.attach(servoPin);
  sensorServo.write(90); // Lock sensor facing strictly forward
  
  BTSerial.begin(9600);
  Serial.begin(9600); // For USB debugging
  
  stopMotors();
  Serial.println("Hybrid System Online. Awaiting Telemetry...");
}

void loop() {
  // 1. Environmental Polling
  int currentDistance = getDistance();
  
  // 2. Telemetry Polling
  if (BTSerial.available()) {
    currentCommand = BTSerial.read();
  }

  // 3. Subsumption Logic (Safety Override)
  // If object is too close, AND the human is trying to drive forward
  if (currentDistance > 0 && currentDistance <= safeDistance) {
    if (currentCommand == 'F') {
      Serial.println("WARNING: Collision Imminent. Forward Command Suppressed.");
      currentCommand = 'S'; // Force stop
    }
  }

  // 4. Kinematic Execution
  executeCommand(currentCommand);
  
  // Minimal cycle delay to prevent buffer flooding
  delay(10); 
}

// --- Sensor Function ---
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // 30ms timeout is critical here to prevent blocking Bluetooth telemetry
  long duration = pulseIn(echoPin, HIGH, 30000); 
  int distance = duration * 0.034 / 2;
  
  if (distance == 0) return 999; // Out of range
  return distance;
}

// --- Command Execution ---
void executeCommand(char cmd) {
  switch (cmd) {
    case 'F': moveForward(driveSpeed); break;
    case 'B': moveBackward(driveSpeed); break;
    case 'L': turnLeft(driveSpeed); break;
    case 'R': turnRight(driveSpeed); break;
    case 'S': stopMotors(); break;
    default: break; // Ignore noise
  }
}

// --- Kinematic Functions ---
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