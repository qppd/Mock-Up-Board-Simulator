// BoardSimulator.ino
// Arduino board simulator main file



#include "I2CLcd.h"
#include "FlameSensor.h"
#include "ServoMotor.h"
#include "JoystickController.h"
#include "MatrixKeypad.h"
#include "Pins.h"

// Keypad configuration
uint8_t rowPins[4] = {KEYPAD_ROW1_PIN, KEYPAD_ROW2_PIN, KEYPAD_ROW3_PIN, KEYPAD_ROW4_PIN};
uint8_t colPins[4] = {KEYPAD_COL1_PIN, KEYPAD_COL2_PIN, KEYPAD_COL3_PIN, KEYPAD_COL4_PIN};

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char* keyRows[4] = {keys[0], keys[1], keys[2], keys[3]};

I2CLcd lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
FlameSensor flameSensor(FLAME_ANALOG_PIN, FLAME_DIGITAL_PIN);
ServoMotor servoMotor(SERVO_PIN);
JoystickController joystick(JOY_VRX_PIN, JOY_VRY_PIN, JOY_SW_PIN);
MatrixKeypad keypad(rowPins, colPins, keyRows, 4, 4);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.print("Hello, I2C LCD!");
  flameSensor.begin();
  servoMotor.begin();
  joystick.begin();
  keypad.begin();
  // Initialize board simulation
  Serial.println("Board Simulator Ready!");
  Serial.println("Commands: LCD, FLAME, SERVO:<angle>, SERVO_READ, SERVO_DETACH, JOYSTICK, KEYPAD");
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    if (cmd == "LCD") { // Test LCD
      lcd.clear();
      lcd.print("LCD Test OK");
      Serial.println("LCD: Message displayed");
    } else if (cmd == "FLAME") { // Test FlameSensor
      int analogVal = flameSensor.readAnalog();
      bool digitalVal = flameSensor.readDigital();
      Serial.print("FlameSensor Analog: ");
      Serial.print(analogVal);
      Serial.print(", Digital: ");
      Serial.println(digitalVal ? "HIGH" : "LOW");
    } else if (cmd.startsWith("SERVO:")) { // Test ServoMotor, e.g. SERVO:90
      int angle = cmd.substring(6).toInt();
      servoMotor.write(angle);
      Serial.print("Servo angle set to: ");
      Serial.println(angle);
    } else if (cmd == "SERVO_READ") {
      int angle = servoMotor.read();
      Serial.print("Servo current angle: ");
      Serial.println(angle);
    } else if (cmd == "SERVO_DETACH") {
      servoMotor.detach();
      Serial.println("Servo detached.");
    } else if (cmd == "JOYSTICK") {
      int x = joystick.readX();
      int y = joystick.readY();
      bool sw = joystick.readSwitch();
      Serial.print("Joystick X: ");
      Serial.print(x);
      Serial.print(", Y: ");
      Serial.print(y);
      Serial.print(", SW: ");
      Serial.println(sw ? "PRESSED" : "RELEASED");
    } else if (cmd == "KEYPAD") {
      char key = keypad.getKey();
      if (key != '\0') {
        Serial.print("Key pressed: ");
        Serial.println(key);
      } else {
        Serial.println("No key pressed");
      }
    } else if (cmd == "KEYPAD_TEST") {
      bool test_result = keypad.selfTest();
      Serial.print("Keypad self-test: ");
      Serial.println(test_result ? "PASSED" : "FAILED");
    } else {
      Serial.println("Unknown command. Use 'LCD', 'FLAME', 'SERVO:<angle>', 'SERVO_READ', 'SERVO_DETACH', 'JOYSTICK', 'KEYPAD', 'KEYPAD_TEST'.");
    }
  }
  
  // Continuous keypad scanning (optional - shows real-time key presses)
  char key = keypad.getKey();
  if (key != '\0') {
    Serial.print("Real-time key: ");
    Serial.println(key);
  }
}
