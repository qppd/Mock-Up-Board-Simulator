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
