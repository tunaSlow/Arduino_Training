
const int ENA = 5;
const int IN1 = 7;
const int IN2 = 8;

const int ENB = 6;
const int IN3 = A0;
const int IN4 = A1;

const int trigPin = 11;
const int echoPin = 12;

// --- Sensor Pin Definitions (IR Modules) ---
const int irLeft = A2;
const int irRight = A3;

const int cruiseSpeed = 120;  // Smooth forward rolling speed
const int turnFSpeed = 180;   // Outer wheel speed during a turn
const int turnRSpeed = 150;   // Inner wheel reverse speed to force the skid

void setup() {
  // 1. Initialize Motor Driver Pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // 2. Initialize IR Sensor Pins
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  stop();
  delay(2000);  // Give the operator time to place the robot on the line
}

void loop() {

  int distanceFront = getDistance();

  if (distanceFront <= 10) {
    stop();
  } else {
    // Read digital states from the IR sensors
    // Standard logic: HIGH (1) = Black Line, LOW (0) = White Floor
    int leftState = digitalRead(irLeft);
    int rightState = digitalRead(irRight);

    // --- Decision Logic Matrix ---
    if (leftState == LOW && rightState == LOW) {
      // Both sensors on white floor: Robot is centered.
      moveForward(cruiseSpeed);
    } else if (leftState == HIGH && rightState == LOW) {
      // Left sensor triggered: Drifting right, execute aggressive left turn.
      aggressiveTurnLeft(turnFSpeed, turnRSpeed);
    } else if (leftState == LOW && rightState == HIGH) {
      // Right sensor triggered: Drifting left, execute aggressive right turn.
      aggressiveTurnRight(turnFSpeed, turnRSpeed);
    } else if (leftState == HIGH && rightState == HIGH) {
      // Both sensors triggered: Reached an intersection or the end of the track.
      stop();
    }
  }
}

void moveForward(int speed) {
  // Left side forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  // Right side forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void aggressiveTurnRight(int turnForwardSpeed, int turnReverseSpeed) {
  // Left side drives FORWARD (Outer wheels push)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, turnForwardSpeed);

  // Right side drives BACKWARD (Inner wheels drag)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, turnReverseSpeed);
}

void aggressiveTurnLeft(int turnForwardSpeed, int turnReverseSpeed) {
  // Left side drives BACKWARD (Inner wheels drag)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, turnReverseSpeed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, turnForwardSpeed);
}

void stop() {
  // Short the terminals of the left motors to abruptly kill momentum
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  // Short the terminals of the right motors
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

int getDistance() {
  // Send 10us HIGH pulse to trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin pulse duration (in microseconds)
  long duration = pulseIn(echoPin, HIGH, 30000);  // 30ms timeout prevents infinite blocking

  // Calculate distance in cm
  int distance = duration * 0.034 / 2;

  // Return 999 if timeout (out of range) to prevent false positives
  if (distance == 0) return 999;
  return distance;
}