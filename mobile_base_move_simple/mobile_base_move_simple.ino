

// Define Left Motor (Motor A) pins
const int ENA = 5;  // PWM speed control
const int IN1 = 7;  // Direction control 1
const int IN2 = 8;  // Direction control 2

// Define Right Motor (Motor B) pins
// Note: Using A0 and A1 as digital pins to save standard digital pins for sensors
const int ENB = 6;  // PWM speed control
const int IN3 = A0; // Direction control 1
const int IN4 = A1; // Direction control 2


void setup() {
  // Configure all motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


}

void loop() {
 // Set Left Motor forward
  Forward(150);
  delay(2000);
  Stop();
  delay(3000);
  
  
}

void Forward (int speed)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
  // Set Right Motor forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);
}

void Stop ()
{
  // Cut power via PWM pins
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  // Set logic LOW for safety
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(500);
}

