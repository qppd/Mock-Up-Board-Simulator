// BoardSimulator.ino
// Arduino board simulator main file



#include "I2CLcd.h"
#include "FlameSensor.h"
#include "ServoMotor.h"


I2CLcd lcd(0x27, 16, 2); // Typical I2C address and 16x2 LCD
FlameSensor flameSensor(A0, 2); // Example: analog pin A0, digital pin 2
ServoMotor servoMotor(9); // Attach servo to pin 9

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.print("Hello, I2C LCD!");
  flameSensor.begin();
  servoMotor.begin();
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
    } else {
      Serial.println("Unknown command. Use 'LCD', 'FLAME', 'SERVO:<angle>', 'SERVO_READ', 'SERVO_DETACH'.");
    }
  }
}
