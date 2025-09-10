// BoardSimulator.ino
// Arduino board simulator main file



#include "I2CLcd.h"
#include "FlameSensor.h"


I2CLcd lcd(0x27, 16, 2); // Typical I2C address and 16x2 LCD
FlameSensor flameSensor(A0, 2); // Example: analog pin A0, digital pin 2

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.print("Hello, I2C LCD!");
  flameSensor.begin();
  // Initialize board simulation
}

void loop() {
 
}
