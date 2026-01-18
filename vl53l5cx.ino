#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h> 
#include <Adafruit_NeoPixel.h>

// Pins und Hardware
#define DATA_PIN 2
#define NUM_LEDS 64
#define SENSOR_SDA 21
#define SENSOR_SCL 22

SparkFun_VL53L5CX sensor;
VL53L5CX_ResultsData measurementData; 
Adafruit_NeoPixel matrix(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

enum Mode { DISTANCE, RAINBOW, SHOW, TEXT };
Mode currentMode = DISTANCE;
int currentPalette = 0;
bool isOn = true;

unsigned long lastGestureTime = 0;
const int gestureDelay = 500; 

void setup() {
  Serial.begin(115200);
  Wire.begin(SENSOR_SDA, SENSOR_SCL);
  Wire.setClock(400000); 

  Serial.println("Initialisiere Sensor...");
  if (sensor.begin() == false) {
    Serial.println("Sensor nicht gefunden! Bitte Verkabelung prüfen.");
    while (1);
  }

  sensor.setResolution(8 * 8); 
  sensor.setRangingFrequency(15);
  sensor.startRanging();

  matrix.begin();
  matrix.setBrightness(50);
  matrix.show();
}

void loop() {
  if (sensor.isDataReady()) {
    if (sensor.getRangingData(&measurementData)) {
      handleGestures();
      
      if (isOn) {
        updateDisplay();
      } else {
        matrix.clear();
        matrix.show();
      }
    }
  }
}

void updateDisplay() {
  switch (currentMode) {
    case DISTANCE:

      for (int i = 0; i < 64; i++) {
        int dist = measurementData.distance_mm[i];
        // Farbe basierend auf Distanz (Nah = Rot/Hell, Fern = Blau/Dunkel)
        uint8_t r = map(dist, 0, 1000, 255, 0);
        uint8_t b = map(dist, 0, 1000, 0, 255);
        matrix.setPixelColor(i, matrix.Color(r, 0, b));
      }
      break;

    case RAINBOW:
      static uint16_t j = 0;
      for (int i = 0; i < matrix.numPixels(); i++) {
        matrix.setPixelColor(i, wheel(((i * 256 / matrix.numPixels()) + j) & 255));
      }
      j++;
      break;

    case SHOW:
      for(int i=0; i<64; i++) matrix.setPixelColor(i, matrix.Color(0, 255, 100));
      break;
  }
  
  int centerDist = measurementData.distance_mm[28]; 
  uint8_t brightness = map(constrain(centerDist, 50, 600), 50, 600, 200, 10);
  matrix.setBrightness(brightness);
  
  matrix.show();
}

void handleGestures() {
  if (millis() - lastGestureTime < gestureDelay) return;

  // 1. Faust / Alles Nah -> An/Aus
  int closeCount = 0;
  for (int i = 0; i < 64; i++) {
    if (measurementData.distance_mm[i] < 100 && measurementData.target_status[i] != 255) closeCount++;
  }

  if (closeCount > 40) { 
    isOn = !isOn;
    lastGestureTime = millis();
    Serial.println("Geste: Faust - An/Aus");
    return;
  }

  // 2. Swipe Erkennung
  int leftZone = measurementData.distance_mm[24];  
  int rightZone = measurementData.distance_mm[31]; 

  if (leftZone < 150 && rightZone > 400) {
    // Von links nach rechts (vereinfacht)
    currentMode = (Mode)((currentMode + 1) % 3);
    lastGestureTime = millis();
    Serial.println("Geste: Swipe Rechts - Modus gewechselt");
  } 
  else if (rightZone < 150 && leftZone > 400) {
    // Von rechts nach links
    currentPalette = (currentPalette + 1) % 3;
    lastGestureTime = millis();
    Serial.println("Geste: Swipe Links - Farbe gewechselt");
  }
}

// Hilfsfunktion für Regenbogen
uint32_t wheel(byte pos) {
  pos = 255 - pos;
  if (pos < 85) return matrix.Color(255 - pos * 3, 0, pos * 3);
  if (pos < 170) { pos -= 85; return matrix.Color(0, pos * 3, 255 - pos * 3); }
  pos -= 170; return matrix.Color(pos * 3, 255 - pos * 3, 0);
}
