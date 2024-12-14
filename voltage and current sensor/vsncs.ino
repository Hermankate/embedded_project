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
ACS712 currentSensor(ACS712_05B, CURRENT_SENSOR_PIN);

// Constants
const int samples = 10;            // Number of samples for averaging
const float interval = 1.0;       // Measurement interval in seconds
unsigned long lastUpdateTime = 0; // Time tracker for updates

// Variables
float totalEnergy = 0.0; // Total energy in kWh

void setup() {
  Serial.begin(9600);
  currentSensor.calibrate();
}

void loop() {
  unsigned long currentTime = millis();

  // Measure only at defined intervals
  if (currentTime - lastUpdateTime >= interval * 1000) {
    lastUpdateTime = currentTime;

    // Measure voltage RMS
    float voltageRMS = voltageSensor.getRmsVoltage();

    // Measure current RMS using sampling
    float currentSum = 0;
    for (int i = 0; i < samples; i++) {
      currentSum += currentSensor.getCurrentAC();
      delay(10);
    }
    float current = currentSum / samples;

    // Calculate instantaneous power (P = V * I)
    float power = voltageRMS * current; // Power in watts

    // Accumulate energy (E = P * t), convert to kWh (1 watt-second = 1/3600 kWh)
    totalEnergy += (power * interval) / 3600.0;

    // Print results for debugging
    Serial.print("Voltage = ");
    Serial.print(voltageRMS, 2);
    Serial.print(" V, Current = ");
    Serial.print(current, 2);
    Serial.print(" A, Power = ");
    Serial.print(power, 2);
    Serial.print(" W, Energy = ");
    Serial.print(totalEnergy, 4);
    Serial.println(" kWh");

    // Send energy data to the main board in the format "E:<kWh>"
    Serial.print("E:");
    Serial.println(totalEnergy, 4); // Send total energy in kWh
  }
}
