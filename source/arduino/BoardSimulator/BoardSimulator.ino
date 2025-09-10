// BoardSimulator.ino
// Arduino board simulator main file


#include "I2CLcd.h"

I2CLcd lcd(0x27, 16, 2); // Typical I2C address and 16x2 LCD

void setup() {
  lcd.begin();
  lcd.print("Hello, I2C LCD!");
  // Initialize board simulation
}

void loop() {
  // Main simulation loop
}
