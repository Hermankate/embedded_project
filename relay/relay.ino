#define RELAY_PIN 3

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relay OFF by default
  Serial.begin(9600); // Communication with Board 4
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "Relay:ON") {
      digitalWrite(RELAY_PIN, HIGH); // Turn relay ON
    } else if (command == "Relay:OFF") {
      digitalWrite(RELAY_PIN, LOW); // Turn relay OFF
    }
  }
}
