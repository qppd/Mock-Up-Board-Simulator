// BoardSimulator.ino
// Arduino board simulator main file



#include "I2CLcd.h"
#include "FlameSensor.h"
#include "ServoMotor.h"
#include "JoystickController.h"
#include "Pins.h"


I2CLcd lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
FlameSensor flameSensor(FLAME_ANALOG_PIN, FLAME_DIGITAL_PIN);
ServoMotor servoMotor(SERVO_PIN);
JoystickController joystick(JOY_VRX_PIN, JOY_VRY_PIN, JOY_SW_PIN);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.print("Hello, I2C LCD!");
  flameSensor.begin();
  servoMotor.begin();
  joystick.begin();
  // Initialize board simulation
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
    } else {
      Serial.println("Unknown command. Use 'LCD', 'FLAME', 'SERVO:<angle>', 'SERVO_READ', 'SERVO_DETACH', 'JOYSTICK'.");
    }
  }
}
