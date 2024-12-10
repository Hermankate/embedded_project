// Define pins for sensors
const int voltagePin = A0; // Voltage sensor connected to A0
const int currentPin = A1; // Current sensor connected to A1

void setup() {
  Serial.begin(9600); // Start serial communication
  pinMode(voltagePin, INPUT);
  pinMode(currentPin, INPUT);
}

void loop() {
  // Read raw values from the sensors
  int voltageRaw = analogRead(voltagePin);
  int currentRaw = analogRead(currentPin);

  // Print the raw values to the Serial Monitor
  Serial.print("Voltage Sensor Reading: ");
  Serial.println(voltageRaw);

  Serial.print("Current Sensor Reading: ");
  Serial.println(currentRaw);

  delay(1000); // Update every second
}
