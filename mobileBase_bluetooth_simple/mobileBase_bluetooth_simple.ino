

#include <SoftwareSerial.h>

// --- Step 4 Pin Definitions (Bluetooth) ---
const int rxPin = 2; // Connects to HC-05 TX
const int txPin = 3; // Connects to HC-05 RX (Use Voltage Divider!)

// Initialize SoftwareSerial
SoftwareSerial BTSerial(rxPin, txPin);
char command = 'S'; // Default state is stop

void setup() {

  Serial.begin(9600);
  BTSerial.begin(9600); 
  Serial.println("System Ready. Awaiting Bluetooth Connection...");

}

void loop() {
  // Check if data is available in the Bluetooth buffer
  if (BTSerial.available()) {
    command = BTSerial.read();
    Serial.print("Received Command: ");
    Serial.println(command);
    
  }
}

