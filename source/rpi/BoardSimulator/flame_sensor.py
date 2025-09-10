# flame_sensor.py
# Flame sensor simulation for Raspberry Pi

class FlameSensor:
    def __init__(self, analog_pin, digital_pin):
        self.analog_pin = analog_pin
        self.digital_pin = digital_pin

    def begin(self):
        print(f"Flame sensor initialized on analog pin {self.analog_pin}, digital pin {self.digital_pin}")

    def read_analog(self):
        # Simulate analog value
        value = 123  # Replace with actual reading logic if needed
        print(f"FlameSensor Analog: {value}")
        return value

    def read_digital(self):
        # Simulate digital value
        value = True  # Replace with actual reading logic if needed
        print(f"FlameSensor Digital: {'HIGH' if value else 'LOW'}")
        return value
