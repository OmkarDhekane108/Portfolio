/*
  IoT-Based Smart Agriculture: Soil Moisture Monitoring System
  ---------------------------------------------------------------
  Project: IoT-Based Smart Agriculture: Soil Moisture Monitoring System
  Author : Omkar Dhekane

  Description:
  Reads a capacitive soil moisture sensor on an ESP32, converts the raw
  reading into a moisture percentage, and automatically switches a water
  pump (via relay) ON when the soil is too dry and OFF once it's wet
  enough — with a small threshold gap to avoid the pump switching on/off
  rapidly near the threshold.

  Hardware:
  - ESP32 Dev Board
  - Capacitive soil moisture sensor -> analog pin (SOIL_PIN)
  - 5V relay module -> controls the water pump -> RELAY_PIN
  - (Optional) Status LED

  Calibration:
  Dip the sensor in dry air and fully in water first to find your own
  DRY_VALUE and WET_VALUE, then update the constants below.
*/

const int SOIL_PIN   = 34;   // ADC1 pin - reads the moisture sensor
const int RELAY_PIN  = 26;   // controls the pump relay
const int STATUS_LED = 2;    // onboard LED, lights up while pump is ON

// Calibration values from dry-air / full-water test (adjust to your sensor)
const int DRY_VALUE = 3000;  // raw ADC reading in dry air
const int WET_VALUE = 1200;  // raw ADC reading fully in water

// Moisture percentage thresholds
const int MOISTURE_ON_THRESHOLD  = 30; // turn pump ON below this %
const int MOISTURE_OFF_THRESHOLD = 55; // turn pump OFF above this %

bool pumpRunning = false;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(STATUS_LED, LOW);

  Serial.println("Soil Moisture Irrigation System starting...");
}

void loop() {
  int raw = analogRead(SOIL_PIN);
  int moisturePercent = rawToPercent(raw);

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("  Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  // Hysteresis control: avoids the pump flicking on/off near the threshold
  if (!pumpRunning && moisturePercent < MOISTURE_ON_THRESHOLD) {
    startPump();
  } else if (pumpRunning && moisturePercent > MOISTURE_OFF_THRESHOLD) {
    stopPump();
  }

  delay(2000); // read every 2 seconds
}

int rawToPercent(int raw) {
  int percent = map(raw, DRY_VALUE, WET_VALUE, 0, 100);
  return constrain(percent, 0, 100);
}

void startPump() {
  pumpRunning = true;
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(STATUS_LED, HIGH);
  Serial.println(">> Soil is dry. Pump turned ON.");
}

void stopPump() {
  pumpRunning = false;
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(STATUS_LED, LOW);
  Serial.println(">> Soil is sufficiently wet. Pump turned OFF.");
}
