#include <SPI.h>
#include <Wire.h>
#include <ZMPT101B.h>
#include <ACS712.h>  // Include the ACS712 library

// Define pins
#define VOLTAGE_SENSOR_PIN A0
#define CURRENT_SENSOR_PIN A1

// AC mains frequency in your country
#define AC_FREQUENCY 50.0

// Initialize ZMPT101B voltage sensor
ZMPT101B voltageSensor(VOLTAGE_SENSOR_PIN, AC_FREQUENCY); // Constructor with pin and frequency

// Calibration constants for the current sensor
const float currentSensorSensitivity = 0.185; // ACS712-05B sensitivity in V/A
const float deadbandThreshold = 0.1;          // Ignore currents smaller than this (A)

void setup() {
  Serial.begin(9600);

  Serial.println("Calibrating current sensor...");
  float currentOffset = calibrateCurrentSensor(CURRENT_SENSOR_PIN);
  Serial.print("Current Sensor Offset: ");
  Serial.println(currentOffset);
}

void loop() {
  // Measure voltage RMS
  float voltageRMS = voltageSensor.getRmsVoltage();

  // Measure current RMS
  float voltageRaw = (5.0 / 1023.0) * analogRead(CURRENT_SENSOR_PIN); // Read raw voltage
  float current = (voltageRaw - 2.5) / currentSensorSensitivity; // Adjust midpoint for ACS712-05B

  // Apply deadband to filter out small fluctuations
  if (abs(current) < deadbandThreshold) {
    current = 0.0; // Treat small currents as zero
  }

  // Calculate power
  float power = voltageRMS * current;

  // Print results
  Serial.print("Voltage = ");
  Serial.print(voltageRMS, 2);
  Serial.print(" V, Current = ");
  Serial.print(current, 2);
  Serial.print(" A, Power = ");
  Serial.print(power, 2);
  Serial.println(" W");

  delay(1000); // Update every second
}

// Function to calibrate the current sensor (find offset with no load)
float calibrateCurrentSensor(int pin) {
  long sum = 0;
  for (int i = 0; i < 100; i++) {
    sum += analogRead(pin);
    delay(10);
  }
  float rawOffset = (5.0 / 1023.0) * (sum / 100.0); // Average raw voltage
  return rawOffset;
}
