

#include <Servo.h>

// --- Step 2 Pin Definitions ---
const int trigPin = 11;
const int echoPin = 12;
const int servoPin = 4;

Servo sensorServo;  // Create servo object


void setup() {
  Serial.begin(9600);
  // Sensor Setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Servo Setup
  sensorServo.attach(servoPin);
  sensorServo.write(90);  // Initialize looking straight ahead
  delay(1000);            // Allow time for mechanical movement
}

void loop() {
  int distanceFront = getDistance();
  Serial.println(distanceFront);

  sensorServo.write(90);
  delay(300);

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
  long duration = pulseIn(echoPin, HIGH, 30000);  // 30ms timeout prevents infinite blocking

  // Calculate distance in cm
  int distance = duration * 0.034 / 2;

  // Return 999 if timeout (out of range) to prevent false positives
  if (distance == 0) return 999;
  return distance;
}
