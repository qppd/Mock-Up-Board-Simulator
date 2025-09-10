# main.py
# Raspberry Pi board simulator main file


from i2c_lcd import I2CLcd
from flame_sensor import FlameSensor
from servo_motor import ServoMotor
from joystick_controller import JoystickController

def main():
    lcd = I2CLcd(0x27, 16, 2)
    flame_sensor = FlameSensor(0, 2)
    servo_motor = ServoMotor(9)
    joystick = JoystickController(1, 2, 3)

    lcd.begin()
    lcd.print("Hello, I2C LCD!")
    flame_sensor.begin()
    servo_motor.begin()
    joystick.begin()

    print("Type commands: LCD, FLAME, SERVO:<angle>, SERVO_READ, SERVO_DETACH, JOYSTICK, or EXIT")
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
        else:
            print("Unknown command. Use 'LCD', 'FLAME', 'SERVO:<angle>', 'SERVO_READ', 'SERVO_DETACH', 'JOYSTICK', or 'EXIT'.")

if __name__ == "__main__":
    main()
