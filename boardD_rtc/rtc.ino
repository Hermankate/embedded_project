#include <Wire.h>
#include <RTClib.h> // Include the RTC library

// Initialize RTC
RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600); // Initialize communication with the main board

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC Error! Stopping...");
    while (1); // Halt if RTC is not found
  }

  // Check if RTC lost power and set to compile time
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now(); // Get the current time
  
  // Send the time as total seconds since midnight
  unsigned long elapsedSeconds = now.hour() * 3600 + now.minute() * 60 + now.second();
  Serial.print("T:"); // Prefix to indicate time data
  Serial.println(elapsedSeconds);

  delay(1000); // Send time every second
}
