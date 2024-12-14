#define RELAY_PIN 3 // Assuming RELAY_PIN is mapped to PORTB3 (digital pin 11)

// Define pointers to the registers
volatile uint8_t *ddrb = (volatile uint8_t *)0x24; // Data Direction Register for PORTB
volatile uint8_t *portb = (volatile uint8_t *)0x25; // Data Register for PORTB

void setup() {
  // Set the relevant bit in the DDRB register (set PORTB3 as OUTPUT)
  *ddrb |= (1 << RELAY_PIN); 

  // Set the relevant bit in the PORTB register to LOW (turn relay OFF)
  *portb &= ~(1 << RELAY_PIN); 

  // Initialize Serial communication
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // Read the command sent via Serial
    String command = Serial.readStringUntil('\n');
    
    if (command == "Relay:ON") {
      // Set the relevant bit in the PORTB register to HIGH (turn relay ON)
      *portb |= (1 << RELAY_PIN);
    } else if (command == "Relay:OFF") {
      // Clear the relevant bit in the PORTB register to LOW (turn relay OFF)
      *portb &= ~(1 << RELAY_PIN);
    }
  }
}
