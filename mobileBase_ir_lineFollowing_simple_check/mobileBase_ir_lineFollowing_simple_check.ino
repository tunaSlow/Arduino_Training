
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
  if (leftState == HIGH) {
    Serial.print("Left: 1,    ");
  }
  else
  {
     Serial.print("Left: 0,    ");
  } 

  if (rightState == HIGH) {
    Serial.println("Right: 1");
  }
  else
  {
     Serial.println("Right: 0");
  } 
  
}
