#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

void setup() {
  Serial.begin(9600);  // Increased baud rate for faster communication
  Wire.begin();

  if (!sensor.init()) {
    Serial.println("-1"); // Error code
    while(1);
  }

  sensor.setTimeout(500);
  sensor.startContinuous();
}

void loop() {
  int distance = sensor.readRangeContinuousMillimeters();
  
  if (sensor.timeoutOccurred()) {
    Serial.println("-2"); // Timeout error code
  } else {
    Serial.println(distance); // Send only the number
  }
  
  delay(100); // Reduced delay for faster updates
}