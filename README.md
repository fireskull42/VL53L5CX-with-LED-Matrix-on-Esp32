ESP32 Gesture-Controlled 8x8 LED Matrix (ToF Sensor)
An interactive project using an ESP32 and a VL53L5CX Time-of-Flight (ToF) sensor to control an 8x8 WS2812B (NeoPixel) LED Matrix through hand gestures and proximity.
Features
Real-time Distance Mapping: By default, the LED matrix visualizes the 8x8 distance data from the sensor as a "depth map" or "heatmap."
Gesture Recognition:

Fist Clench/All-Zone Close: Toggles the LED matrix ON or OFF.

Swipe Left-to-Right: Cycles through different animation modes (Text Scroll, Rainbow Pattern, LED Show).

Swipe Right-to-Left: Changes the color palettes of the standard distance visualization.
Proximity Brightness Control: Dynamically adjusts the matrix brightness based on the distance of your hand to the sensor.

Proximity Brightness Control: Dynamically adjusts the matrix brightness based on the distance of your hand to the sensor.

Hardware Requirements
Microcontroller: ESP32 (e.g., ESP32-WROOM-DA)

Sensor: VL53L5CX (8x8 Multi-Zone Time-of-Flight sensor)

Display: 8x8 WS2812B LED Matrix (64 NeoPixels)

Power Supply: 5V (ensure sufficient current for the LED matrix)

Software & Libraries
The project is developed using the Arduino IDE and requires the following libraries:

SparkFun VL53L5CX Library: For high-speed distance ranging data.

Adafruit NeoPixel Library: To control the 8x8 LED grid.

Wire.h: For I2C communication.

Wiring Diagram

VL53L5CX SDA	GPIO 21
VL53L5CX SCL	GPIO 22
LED Matrix DIN	GPIO 2
VCC	5V (Matrix) / 3.3V (Sensor)
GND	GND (Common Ground)

Installation & Usage

Clone this repository.

Install the required libraries via the Arduino Library Manager.

Open the .ino file and upload it to your ESP32.

Open the Serial Monitor (115200 baud) to see gesture triggers in real-time.
