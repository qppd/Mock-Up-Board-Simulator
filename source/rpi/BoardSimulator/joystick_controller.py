# joystick_controller.py
# Joystick simulation for Raspberry Pi

import random

class JoystickController:
    def __init__(self, pin_vrx, pin_vry, pin_sw):
        self.pin_vrx = pin_vrx
        self.pin_vry = pin_vry
        self.pin_sw = pin_sw
        self.x = 512
        self.y = 512
        self.sw = False

    def begin(self):
        print(f"Joystick initialized on VRx pin {self.pin_vrx}, VRy pin {self.pin_vry}, SW pin {self.pin_sw}")

    def read_x(self):
        # Simulate analog value (0-1023)
        self.x = random.randint(0, 1023)
        print(f"Joystick X: {self.x}")
        return self.x

    def read_y(self):
        # Simulate analog value (0-1023)
        self.y = random.randint(0, 1023)
        print(f"Joystick Y: {self.y}")
        return self.y

    def read_switch(self):
        # Simulate switch (pressed or not)
        self.sw = random.choice([True, False])
        print(f"Joystick SW: {'PRESSED' if self.sw else 'RELEASED'}")
        return self.sw
