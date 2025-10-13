# pins.py
# Pin assignments for Raspberry Pi simulation

LCD_ADDR = 0x27
LCD_COLS = 16
LCD_ROWS = 2
FLAME_ANALOG_PIN = 0
FLAME_DIGITAL_PIN = 2
SERVO_PIN = 18  # GPIO18 (Hardware PWM)
JOY_VRX_PIN = 1
JOY_VRY_PIN = 2
JOY_SW_PIN = 3

# 4x4 Matrix Keypad GPIO assignments
KEYPAD_ROW_PINS = [4, 5, 6, 7]     # GPIO4, GPIO5, GPIO6, GPIO7
KEYPAD_COL_PINS = [8, 9, 10, 11]   # GPIO8, GPIO9, GPIO10, GPIO11

# Keypad layout configuration
KEYPAD_LAYOUT = [
    ['1', '2', '3', 'A'],
    ['4', '5', '6', 'B'],
    ['7', '8', '9', 'C'],
    ['*', '0', '#', 'D']
]

# Infrared Receiver GPIO pin
IR_RECEIVER_PIN = 12

# RFID RC522 SPI pins (using default SPI)
RFID_SS_PIN = 8     # GPIO8 (CE0)
RFID_RST_PIN = 25   # GPIO25

# Solenoid + Relay control pins
SOLENOID_RELAY_PIN = 13
SOLENOID_STATUS_PIN = 19  # Optional status feedback

# Sound Sensor GPIO assignments
SOUND_ANALOG_PIN = 3      # ADC channel 3
SOUND_DIGITAL_PIN = 14

# Dot Matrix Display SPI pins
MATRIX_DATA_PIN = 10     # GPIO10 (MOSI)
MATRIX_CLOCK_PIN = 11    # GPIO11 (SCLK)
MATRIX_CS_PIN = 15       # GPIO15

# Ultrasonic Sensor (HC-SR04) pins
ULTRASONIC_TRIG_PIN = 16
ULTRASONIC_ECHO_PIN = 20

# I2C pins (hardware defined)
# SDA: GPIO2 (Pin 3)
# SCL: GPIO3 (Pin 5)

# SPI pins (hardware defined)
# MOSI: GPIO10 (Pin 19)
# MISO: GPIO9 (Pin 21) 
# SCLK: GPIO11 (Pin 23)
# CE0: GPIO8 (Pin 24)
# CE1: GPIO7 (Pin 26)
