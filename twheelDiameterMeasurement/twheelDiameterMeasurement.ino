#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

/* Number of samples for median filtering */
const uint8_t SAMPLES = 64;

/* Buffer to hold burst readings */
uint16_t readings[SAMPLES];

/* ----------- helper: median of N uint16 ------------ */
uint16_t medianN(uint16_t *arr, uint8_t n) {
  // simple insertion sort (OK for n = 64)
  for (uint8_t i = 1; i < n; i++) {
    uint16_t key = arr[i];
    int8_t j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
  return arr[n / 2];  // n is even → upper median
}
/* ---------------------------------------------------- */

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!sensor.init()) {
    Serial.println("-1"); // init error
    while (1);
  }

  sensor.setTimeout(500);
  sensor.setMeasurementTimingBudget(200000);  // 200 ms High-Accuracy
  sensor.startContinuous();
}

void loop() {
  /* ---- burst-capture 64 readings ---- */
  for (uint8_t i = 0; i < SAMPLES; i++) {
    readings[i] = sensor.readRangeContinuousMillimeters();
  }

  if (sensor.timeoutOccurred()) {
    Serial.println("-2");  // timeout error code
    return;
  }

  /* ---- compute median of raw distance (mm) ---- */
  uint16_t g_raw = medianN(readings, SAMPLES);

  /* ---- output the raw distance in mm ---- */
  Serial.println(g_raw);

  delay(100);  // 0.1 s between bursts
}
