/*
 * Step 3: Line Following Autonomy
 * Architecture: Arduino UNO + L298N + 2x IR Sensors
 */

// --- Step 1 Pin Definitions (Motors) ---
const int ENA = 5, IN1 = 7, IN2 = 8;
const int ENB = 6, IN3 = A0, IN4 = A1;

// --- Step 3 Pin Definitions (IR Sensors) ---
const int irLeft = A2;
const int irRight = A3;

// Tuning parameters
const int baseSpeed = 120; // Lower speed recommended for bang-bang line following
const int turnSpeed = 150; // Speed differential for pivot turns

void setup() {
  // Motor Setup
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  
  // IR Sensor Setup
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  stopMotors();
  delay(2000); // System stabilization delay before execution
}

void loop() {
  // Read digital states from sensors
  // Assuming HIGH (1) means Black line detected, LOW (0) means White floor detected
  int leftState = digitalRead(irLeft);
  int rightState = digitalRead(irRight);

  // Execute Decision Logic Flow
  if (leftState == LOW && rightState == LOW) {
    // Both on white: centered
    moveForward(baseSpeed);
  } 
  else if (leftState == HIGH && rightState == LOW) {
    // Left sensor on black: drifting right, need to correct left
    turnLeft(turnSpeed);
  } 
  else if (leftState == LOW && rightState == HIGH) {
    // Right sensor on black: drifting left, need to correct right
    turnRight(turnSpeed);
  } 
  else if (leftState == HIGH && rightState == HIGH) {
    // Both sensors on black: intersection or end of line
    stopMotors();
  }
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