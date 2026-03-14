/*
 * Master Integration: Teleoperation + Autonomous Safety Override
 * Architecture: Arduino UNO + L298N + HC-05 + HC-SR04 + SG90
 */

#include <SoftwareSerial.h>
#include <Servo.h>

// --- Pin Definitions ---
const int ENA = 5, IN1 = 7, IN2 = 8;
const int ENB = 6, IN3 = A0, IN4 = A1;
const int rxPin = 2, txPin = 3;
SoftwareSerial BTSerial(rxPin, txPin);

const int trigPin = 11, echoPin = 12, servoPin = 4;
Servo sensorServo;

// --- System Parameters ---
const int driveSpeed = 200;
const int safeDistance = 25; 

char currentMotorCommand = 'S'; 
int currentServoAngle = 90; // Track where the sensor is looking

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);
  
  sensorServo.attach(servoPin);
  sensorServo.write(currentServoAngle); 
  
  BTSerial.begin(9600);
  Serial.begin(9600); 
  
  stopMotors();
  Serial.println("Hybrid System Online. Awaiting Telemetry...");
}

void loop() {
  // 1. Environmental Polling
  int currentDistance = getDistance();
  
  // 2. Telemetry Polling & Routing
  if (BTSerial.available()) {
    char incomingData = BTSerial.read();
    
    // BUG FIX: Added '||' operators
    if (incomingData == '1' || incomingData == '2' || incomingData == '3') {
      executeServo(incomingData);
    } 
    // BUG FIX: Removed stray 'a'
    else {
      currentMotorCommand = incomingData;
    }
  }

  // 3. Subsumption Logic (Safety Override)
  if (currentDistance > 0 && currentDistance <= safeDistance) {
    if (currentMotorCommand == 'F' && currentServoAngle == 90) {
      Serial.println("WARNING: Collision Imminent. Forward Suppressed.");
      currentMotorCommand = 'S'; 
    }
  }

  // 4. Kinematic Execution
  executeMotors(currentMotorCommand);
  
  delay(10); 
}

// --- Sensor Function ---
int getDistance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); 
  int distance = duration * 0.034 / 2;
  
  if (distance == 0) return 999; 
  return distance;
}

// --- Servo Execution ---
void executeServo(char cmd) {
  switch (cmd) {
    case '1': currentServoAngle = 30; break;
    case '2': currentServoAngle = 90; break;
    case '3': currentServoAngle = 150; break;
  }
  sensorServo.write(currentServoAngle);
}

// --- Motor Execution ---
void executeMotors(char cmd) {
  switch (cmd) {
    case 'F': moveForward(driveSpeed); break;
    case 'B': moveBackward(driveSpeed); break;
    case 'L': turnLeft(driveSpeed); break;
    case 'R': turnRight(driveSpeed); break;
    case 'S': stopMotors(); break;
    default: break; 
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