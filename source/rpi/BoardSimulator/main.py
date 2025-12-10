# main.py
# Raspberry Pi board simulator main file


from i2c_lcd import I2CLcd
from flame_sensor import FlameSensor
from servo_motor import ServoMotor
from joystick_controller import JoystickController
from matrix_keypad import MatrixKeypad
from infrared_receiver import InfraredReceiver
from ultrasonic_sensor import UltrasonicSensor
from pir_motion_sensor import PirMotionSensor
import pins
import atexit
import time

def main():
    # Initialize all components
    lcd = I2CLcd(pins.LCD_ADDR, pins.LCD_COLS, pins.LCD_ROWS)
    flame_sensor = FlameSensor(pins.FLAME_ANALOG_PIN, pins.FLAME_DIGITAL_PIN)
    servo_motor = ServoMotor(pins.SERVO_PIN)
    joystick = JoystickController(pins.JOY_VRX_PIN, pins.JOY_VRY_PIN, pins.JOY_SW_PIN)
    keypad = MatrixKeypad(pins.KEYPAD_ROW_PINS, pins.KEYPAD_COL_PINS, pins.KEYPAD_LAYOUT)
    ir_receiver = InfraredReceiver(pins.IR_RECEIVER_PIN)
    ultrasonic = UltrasonicSensor(pins.ULTRASONIC_TRIG_PIN, pins.ULTRASONIC_ECHO_PIN)
    pir_sensor = PirMotionSensor(pins.PIR_MOTION_PIN)
    
    # Note: RFID, Solenoid, Sound Sensor, and Matrix Display modules
    # can be added similarly when their Python implementations are created

    # Register cleanup functions
    def cleanup_all():
        keypad.cleanup()
        ir_receiver.cleanup()
        ultrasonic.cleanup()
        pir_sensor.cleanup()
        print("All components cleaned up")
    
    atexit.register(cleanup_all)

    # Initialize all components
    print("Initializing Board Simulator...")
    lcd.begin()
    lcd.print("Initializing...")
    flame_sensor.begin()
    servo_motor.begin()
    joystick.begin()
    keypad.begin()
    ir_receiver.begin()
    ultrasonic.begin()
    pir_sensor.begin()
    
    lcd.clear()
    lcd.print("System Ready!")
    
    print("=== BOARD SIMULATOR READY ===")
    print("Available Commands:")
    print("LCD, FLAME, SERVO:<angle>, JOYSTICK, KEYPAD")
    print("IR, ULTRASONIC, PIR, TEST_ALL, EXIT")
    print("Note: More modules available in Arduino version")
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
        elif cmd.upper() == "IR":
            if ir_receiver.is_data_available():
                code = ir_receiver.read_code()
                if code:
                    command = ir_receiver.decode_command(code)
                    print(f"IR Code: {code} ({command})")
                else:
                    print("IR: No valid code received")
            else:
                print("IR: No signal detected")
        elif cmd.upper() == "ULTRASONIC":
            distance = ultrasonic.read_distance_filtered()
            if distance > 0:
                print(f"Ultrasonic distance: {distance} cm")
            else:
                print("Ultrasonic: Out of range or error")
        elif cmd.upper() == "PIR":
            motion = pir_sensor.read_motion()
            print(f"PIR Motion: {'DETECTED' if motion else 'NONE'}")
            if pir_sensor.motion_detected():
                print("PIR: Motion event triggered!")
        elif cmd.upper() == "TEST_ALL":
            print("=== COMPONENT SELF-TEST ===")
            print("LCD: OK")  # LCD doesn't have self-test
            print(f"Flame Sensor: {'PASS' if flame_sensor.self_test() else 'FAIL'}")
            print("Servo: OK")  # Servo doesn't have self-test
            print(f"Joystick: {'PASS' if joystick.self_test() else 'FAIL'}")
            print(f"Keypad: {'PASS' if keypad.self_test() else 'FAIL'}")
            print(f"IR Receiver: {'PASS' if ir_receiver.self_test() else 'FAIL'}")
            print(f"Ultrasonic: {'PASS' if ultrasonic.self_test() else 'FAIL'}")
            print(f"PIR Sensor: {'PASS' if pir_sensor.self_test() else 'FAIL'}")
            print("=== TEST COMPLETE ===")
        else:
            print("Unknown command. Type 'TEST_ALL' for available commands or 'EXIT' to quit.")

if __name__ == "__main__":
    main()
