#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD with address 0x27, 16 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables for energy calculations
float totalKWh = 0.0;    // Total energy in kWh
float costPerKWh = 500.0; // Cost per kWh in UGX
float totalCost = 0.0;   // Total cost in UGX

unsigned long previousSeconds = 0; // To store the last received time
unsigned long elapsedTime = 0;     // To calculate elapsed time between updates

void setup() {
  Serial.begin(9600);    // Initialize serial communication with RTC board
  lcd.init();            // Initialize LCD
  lcd.backlight();       // Turn on the backlight

  lcd.setCursor(0, 0);
  lcd.print("Waiting for RTC");
}

void loop() {
  // Check if data is available from RTC board
  if (Serial.available() > 0) {
    // Read incoming data as a string
    String data = Serial.readStringUntil('\n');

    // Parse data format (e.g., "T:<seconds>")
    if (data.startsWith("T:")) {
      unsigned long currentSeconds = data.substring(2).toInt(); // Extract time in seconds

      // Calculate elapsed time since last update
      if (previousSeconds > 0) { // Ignore the first reading
        elapsedTime = currentSeconds - previousSeconds;
        if (elapsedTime < 0) {   // Handle rollover at midnight
          elapsedTime += 86400; // Add 24 hours in seconds
        }

        // Use elapsed time in energy calculations
        float power = 200.0; // Mock power in watts (replace with actual sensor data)
        float energy = (power * elapsedTime) / 3600.0; // Energy in kWh
        totalKWh += energy;
        totalCost = totalKWh * costPerKWh;

        // Update LCD with the new values
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Energy: ");
        lcd.print(totalKWh, 2); // Display energy in kWh with 2 decimal places
        lcd.print(" kWh");

        lcd.setCursor(0, 1);
        lcd.print("Cost: UGX ");
        lcd.print(totalCost, 2); // Display cost with 2 decimal places
      }

      previousSeconds = currentSeconds; // Update previous time
    }
  }

  delay(1000); // Update every second
}
