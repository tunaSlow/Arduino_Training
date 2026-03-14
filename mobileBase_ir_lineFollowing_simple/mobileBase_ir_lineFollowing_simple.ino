
// --- Step 3 Pin Definitions (IR Sensors) ---
const int irLeft = A2;
const int irRight = A3;


void setup() {
  Serial.begin(9600);
  // IR Sensor Setup
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

}

void loop() {
   int leftState = digitalRead(irLeft);
  int rightState = digitalRead(irRight);

  // Execute Decision Logic Flow
  if (leftState == LOW && rightState == LOW) {
    Serial.println("Forward");
  } 
  else if (leftState == HIGH && rightState == LOW) {
    Serial.println("Turn Left");
  } 
  else if (leftState == LOW && rightState == HIGH) {
    Serial.println("Turn Right");
  } 
  else if (leftState == HIGH && rightState == HIGH) {
    Serial.println("Stop");
  }
}
