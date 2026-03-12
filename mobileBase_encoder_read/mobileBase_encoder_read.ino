// Define the pins based on our wiring
const int encoderPinA = 2; // Interrupt Pin 0
const int encoderPinB = 3;

// 'volatile' is required for variables modified inside an Interrupt Service Routine (ISR)
// It tells the compiler not to optimize this variable out.
volatile long pulseCount = 0; 

// A variable to keep track of the last count so we only print when it changes
long lastReportedCount = 0;

void setup() {
  Serial.begin(9600);

  // Set pins as inputs. 
  // INPUT_PULLUP enables the internal resistors, which is usually required for encoders
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  // Attach the interrupt to Pin 2 (Interrupt 0)
  // trigger the 'readEncoder' function whenever Pin A changes state (HIGH to LOW or LOW to HIGH)
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, CHANGE);
  
  Serial.println("Encoder Initialized. Waiting for rotation...");
}

void loop() {
  // Briefly disable interrupts to safely read the volatile variable
  long currentCount;
  noInterrupts();
  currentCount = pulseCount;
  interrupts();

  // If the count has changed since we last checked, print it to the Serial Monitor
  if (currentCount != lastReportedCount) {
    Serial.print("Pulse Count: ");
    Serial.println(currentCount);
    lastReportedCount = currentCount;
  }
}

// ---- Interrupt Service Routine (ISR) ----
// This function runs automatically every time Pin A changes state.
// Keep ISRs as short and fast as possible! No Serial.print() inside here!
void readEncoder() {
  int stateA = digitalRead(encoderPinA);
  int stateB = digitalRead(encoderPinB);

  // If Pin A and Pin B are the same state, it's rotating clockwise
  if (stateA == stateB) {
    pulseCount++;
  } 
  // If they are different states, it's rotating counter-clockwise
  else {
    pulseCount--;
  }
}