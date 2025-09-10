# servo_motor.py
# Servo motor simulation for Raspberry Pi

class ServoMotor:
    def __init__(self, pin):
        self.pin = pin
        self.angle = 0
        self.attached = False

    def begin(self):
        self.attached = True
        print(f"Servo attached to pin {self.pin}")

    def write(self, angle):
        if not self.attached:
            print("Servo not attached!")
            return
        self.angle = angle
        print(f"Servo angle set to: {angle}")

    def read(self):
        print(f"Servo current angle: {self.angle}")
        return self.angle

    def detach(self):
        self.attached = False
        print("Servo detached.")
