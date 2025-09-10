# main.py
# Raspberry Pi board simulator main file


from i2c_lcd import I2CLcd
from flame_sensor import FlameSensor
from servo_motor import ServoMotor

def main():
    lcd = I2CLcd(0x27, 16, 2)
    flame_sensor = FlameSensor(0, 2)
    servo_motor = ServoMotor(9)

    lcd.begin()
    lcd.print("Hello, I2C LCD!")
    flame_sensor.begin()
    servo_motor.begin()

    print("Type commands: LCD, FLAME, SERVO:<angle>, SERVO_READ, SERVO_DETACH, or EXIT")
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
        else:
            print("Unknown command. Use 'LCD', 'FLAME', 'SERVO:<angle>', 'SERVO_READ', 'SERVO_DETACH', or 'EXIT'.")

if __name__ == "__main__":
    main()
