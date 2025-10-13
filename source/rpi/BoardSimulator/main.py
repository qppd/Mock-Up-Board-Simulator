# main.py
# Raspberry Pi board simulator main file


from i2c_lcd import I2CLcd
from flame_sensor import FlameSensor
from servo_motor import ServoMotor
from joystick_controller import JoystickController
from matrix_keypad import MatrixKeypad
import pins
import atexit

def main():
    lcd = I2CLcd(pins.LCD_ADDR, pins.LCD_COLS, pins.LCD_ROWS)
    flame_sensor = FlameSensor(pins.FLAME_ANALOG_PIN, pins.FLAME_DIGITAL_PIN)
    servo_motor = ServoMotor(pins.SERVO_PIN)
    joystick = JoystickController(pins.JOY_VRX_PIN, pins.JOY_VRY_PIN, pins.JOY_SW_PIN)
    keypad = MatrixKeypad(pins.KEYPAD_ROW_PINS, pins.KEYPAD_COL_PINS, pins.KEYPAD_LAYOUT)

    # Register cleanup function
    atexit.register(lambda: keypad.cleanup())

    lcd.begin()
    lcd.print("Hello, I2C LCD!")
    flame_sensor.begin()
    servo_motor.begin()
    joystick.begin()
    keypad.begin()

    print("Board Simulator Ready!")
    print("Type commands: LCD, FLAME, SERVO:<angle>, SERVO_READ, SERVO_DETACH, JOYSTICK, KEYPAD, KEYPAD_TEST, or EXIT")
    while True:
        cmd = input(">> ").strip()
        if cmd.upper() == "EXIT":
            print("Exiting...")
            break
        elif cmd.upper() == "LCD":
            lcd.clear()
            lcd.print("LCD Test OK")
            print("LCD: Message displayed")
        elif cmd.upper() == "FLAME":
            analog_val = flame_sensor.read_analog()
            digital_val = flame_sensor.read_digital()
            print(f"FlameSensor Analog: {analog_val}, Digital: {'HIGH' if digital_val else 'LOW'}")
        elif cmd.upper().startswith("SERVO:"):
            try:
                angle = int(cmd.split(":")[1])
                servo_motor.write(angle)
            except Exception as e:
                print("Invalid angle.")
        elif cmd.upper() == "SERVO_READ":
            servo_motor.read()
        elif cmd.upper() == "SERVO_DETACH":
            servo_motor.detach()
        elif cmd.upper() == "JOYSTICK":
            x = joystick.read_x()
            y = joystick.read_y()
            sw = joystick.read_switch()
            print(f"Joystick X: {x}, Y: {y}, SW: {'PRESSED' if sw else 'RELEASED'}")
        elif cmd.upper() == "KEYPAD":
            key = keypad.get_key()
            if key is not None:
                key_name = keypad.get_key_name(key)
                print(f"Key pressed: {key} ({key_name})")
            else:
                print("No key pressed")
        elif cmd.upper() == "KEYPAD_TEST":
            test_result = keypad.self_test()
            print(f"Keypad self-test: {'PASSED' if test_result else 'FAILED'}")
        else:
            print("Unknown command. Use 'LCD', 'FLAME', 'SERVO:<angle>', 'SERVO_READ', 'SERVO_DETACH', 'JOYSTICK', 'KEYPAD', 'KEYPAD_TEST', or 'EXIT'.")

if __name__ == "__main__":
    main()
