// BoardSimulator.ino
// Arduino board simulator main file



#include "I2CLcd.h"
#include "FlameSensor.h"
#include "ServoMotor.h"
#include "JoystickController.h"
#include "MatrixKeypad.h"
#include "InfraredReceiver.h"
#include "RFID_RC522.h"
#include "SolenoidController.h"
#include "SoundSensor.h"
#include "DotMatrixDisplay.h"
#include "UltrasonicSensor.h"
#include "PirMotionSensor.h"
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

// Component instances
I2CLcd lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
FlameSensor flameSensor(FLAME_ANALOG_PIN, FLAME_DIGITAL_PIN);
ServoMotor servoMotor(SERVO_PIN);
JoystickController joystick(JOY_VRX_PIN, JOY_VRY_PIN, JOY_SW_PIN);
MatrixKeypad keypad(rowPins, colPins, keyRows, 4, 4);
InfraredReceiver irReceiver(IR_RECEIVER_PIN);
RFID_RC522 rfidReader(RFID_SS_PIN, RFID_RST_PIN);
SolenoidController solenoid(SOLENOID_RELAY_PIN, SOLENOID_STATUS_PIN);
SoundSensor soundSensor(SOUND_ANALOG_PIN, SOUND_DIGITAL_PIN);
DotMatrixDisplay matrixDisplay(MATRIX_DATA_PIN, MATRIX_CLOCK_PIN, MATRIX_CS_PIN);
UltrasonicSensor ultrasonicSensor(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN);
PirMotionSensor pirSensor(PIR_MOTION_PIN);

void setup() {
  Serial.begin(9600);
  
  // Initialize all components
  lcd.begin();
  lcd.print("Initializing...");
  
  flameSensor.begin();
  servoMotor.begin();
  joystick.begin();
  keypad.begin();
  irReceiver.begin();
  rfidReader.begin();
  solenoid.begin();
  soundSensor.begin();
  matrixDisplay.begin();
  ultrasonicSensor.begin();
  pirSensor.begin();
  
  lcd.clear();
  lcd.print("System Ready!");
  
  Serial.println("=== BOARD SIMULATOR READY ===");
  Serial.println("Available Commands:");
  Serial.println("LCD, FLAME, SERVO:<angle>, JOYSTICK, KEYPAD");
  Serial.println("IR, RFID, SOLENOID:<ON/OFF>, SOUND, MATRIX:<text>, ULTRASONIC");
  Serial.println("TEST_ALL - Run all component tests");
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
    } else if (cmd == "IR") {
      if (irReceiver.isDataAvailable()) {
        uint32_t code = irReceiver.readCode();
        if (code != 0) {
          String command = irReceiver.decodeCommand(code);
          Serial.print("IR Code: 0x");
          Serial.print(code, HEX);
          Serial.print(" (");
          Serial.print(command);
          Serial.println(")");
        }
      } else {
        Serial.println("No IR signal received");
      }
    } else if (cmd == "RFID") {
      if (rfidReader.readCard()) {
        String uid = rfidReader.getCardUID();
        String cardType = rfidReader.getCardType();
        Serial.print("RFID Card - UID: ");
        Serial.print(uid);
        Serial.print(", Type: ");
        Serial.println(cardType);
      } else {
        Serial.println("No RFID card detected");
      }
    } else if (cmd.startsWith("SOLENOID:")) {
      String action = cmd.substring(9);
      if (action == "ON") {
        solenoid.activate();
      } else if (action == "OFF") {
        solenoid.deactivate();
      } else if (action.startsWith("PULSE:")) {
        int duration = action.substring(6).toInt();
        solenoid.pulse(duration);
      } else {
        Serial.println("Invalid solenoid command. Use ON, OFF, or PULSE:<ms>");
      }
    } else if (cmd == "SOUND") {
      int analog = soundSensor.readAnalog();
      bool digital = soundSensor.readDigital();
      bool detected = soundSensor.isSoundDetected();
      Serial.print("Sound - Analog: ");
      Serial.print(analog);
      Serial.print(", Digital: ");
      Serial.print(digital ? "HIGH" : "LOW");
      Serial.print(", Detected: ");
      Serial.println(detected ? "YES" : "NO");
    } else if (cmd.startsWith("MATRIX:")) {
      String text = cmd.substring(7);
      if (text.length() > 0) {
        matrixDisplay.displayString(text);
        Serial.print("Matrix display: ");
        Serial.println(text);
      } else {
        matrixDisplay.clear();
        Serial.println("Matrix display cleared");
      }
    } else if (cmd == "ULTRASONIC") {
      long distance = ultrasonicSensor.readDistanceFiltered();
      if (distance > 0) {
        Serial.print("Ultrasonic distance: ");
        Serial.print(distance);
        Serial.println(" cm");
      } else {
        Serial.println("Ultrasonic: Out of range or error");
      }
    } else if (cmd == "PIR") {
      bool motion = pirSensor.readMotion();
      bool detected = pirSensor.motionDetected();
      Serial.print("PIR Motion: ");
      Serial.print(motion ? "HIGH" : "LOW");
      Serial.print(", Detected: ");
      Serial.println(detected ? "YES" : "NO");
    } else if (cmd == "TEST_ALL") {
      Serial.println("=== COMPONENT SELF-TEST ===");
      Serial.print("LCD: "); Serial.println("OK"); // LCD doesn't have self-test
      Serial.print("Flame Sensor: "); Serial.println(flameSensor.selfTest() ? "PASS" : "FAIL");
      Serial.print("Servo: "); Serial.println("OK"); // Servo doesn't have self-test
      Serial.print("Joystick: "); Serial.println(joystick.selfTest() ? "PASS" : "FAIL");
      Serial.print("Keypad: "); Serial.println(keypad.selfTest() ? "PASS" : "FAIL");
      Serial.print("IR Receiver: "); Serial.println(irReceiver.selfTest() ? "PASS" : "FAIL");
      Serial.print("RFID: "); Serial.println(rfidReader.selfTest() ? "PASS" : "FAIL");
      Serial.print("Solenoid: "); Serial.println(solenoid.selfTest() ? "PASS" : "FAIL");
      Serial.print("Sound Sensor: "); Serial.println(soundSensor.selfTest() ? "PASS" : "FAIL");
      Serial.print("Matrix Display: "); Serial.println(matrixDisplay.selfTest() ? "PASS" : "FAIL");
      Serial.print("Ultrasonic: "); Serial.println(ultrasonicSensor.selfTest() ? "PASS" : "FAIL");
      Serial.print("PIR Sensor: "); Serial.println(pirSensor.selfTest() ? "PASS" : "FAIL");
      Serial.println("=== TEST COMPLETE ===");
    } else {
      Serial.println("Unknown command. Type 'TEST_ALL' for available commands.");
    }
  }
  
  // Continuous keypad scanning (optional - shows real-time key presses)
  char key = keypad.getKey();
  if (key != '\0') {
    Serial.print("Real-time key: ");
    Serial.println(key);
  }
}
