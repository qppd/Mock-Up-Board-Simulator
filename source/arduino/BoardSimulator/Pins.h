#ifndef PINS_H
#define PINS_H

// Pin assignments for Arduino
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
#define FLAME_ANALOG_PIN A0
#define FLAME_DIGITAL_PIN 2
#define SERVO_PIN 9
#define JOY_VRX_PIN A1
#define JOY_VRY_PIN A2
#define JOY_SW_PIN 3

// 4x4 Matrix Keypad pin assignments
#define KEYPAD_ROW1_PIN 4
#define KEYPAD_ROW2_PIN 5
#define KEYPAD_ROW3_PIN 6
#define KEYPAD_ROW4_PIN 7
#define KEYPAD_COL1_PIN 8
#define KEYPAD_COL2_PIN 10
#define KEYPAD_COL3_PIN 11
#define KEYPAD_COL4_PIN 12

// Infrared Receiver pin assignment
#define IR_RECEIVER_PIN 13

// RFID RC522 SPI pin assignments
#define RFID_SS_PIN 53     // Slave Select (for Arduino Mega)
#define RFID_RST_PIN 49    // Reset pin (for Arduino Mega)

// Solenoid + Relay control pins
#define SOLENOID_RELAY_PIN 14
#define SOLENOID_STATUS_PIN 15  // Optional status feedback

// Sound Sensor pin assignments  
#define SOUND_ANALOG_PIN A3
#define SOUND_DIGITAL_PIN 16

// Dot Matrix Display (MAX7219) SPI pins
#define MATRIX_DATA_PIN 51   // MOSI (for Arduino Mega)
#define MATRIX_CLOCK_PIN 52  // SCK (for Arduino Mega) 
#define MATRIX_CS_PIN 50     // Chip Select (for Arduino Mega)

// Ultrasonic Sensor (HC-SR04) pins
#define ULTRASONIC_TRIG_PIN 27
#define ULTRASONIC_ECHO_PIN 25

// PIR Motion Sensor pin assignment
#define PIR_MOTION_PIN 23

// Note: For Arduino Uno, adjust pins as needed due to limited I/O
// Arduino Mega 2560 recommended for full module support

#endif // PINS_H
