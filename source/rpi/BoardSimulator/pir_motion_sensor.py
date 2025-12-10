# pir_motion_sensor.py
# PIR Motion Sensor module for Raspberry Pi

import time
try:
    import RPi.GPIO as GPIO
except ImportError:
    # Fallback for development/testing
    class MockGPIO:
        BCM = "BCM"
        IN = "IN"
        PUD_DOWN = "PUD_DOWN"
        
        @staticmethod
        def setmode(mode):
            pass
        
        @staticmethod
        def setup(pin, mode, pull_up_down=None):
            pass
        
        @staticmethod
        def input(pin):
            return 0
        
        @staticmethod
        def cleanup():
            pass
    
    GPIO = MockGPIO()

class PirMotionSensor:
    def __init__(self, pir_pin):
        """
        Initialize PIR Motion Sensor
        
        Args:
            pir_pin (int): GPIO pin for PIR sensor
        """
        self.pir_pin = pir_pin
        self.last_motion_state = False
        self.last_motion_time = 0
        self.motion_debounce_time = 0.05  # 50ms debounce

    def begin(self):
        """Initialize the PIR sensor"""
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pir_pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
        self.last_motion_state = GPIO.input(self.pir_pin)
        print(f"PIR Motion Sensor initialized on GPIO pin {self.pir_pin}")

    def read_motion(self):
        """
        Read current motion state
        
        Returns:
            bool: True if motion detected, False otherwise
        """
        current_state = GPIO.input(self.pir_pin)
        return bool(current_state)

    def motion_detected(self):
        """
        Detect rising edge (motion started) with debouncing
        
        Returns:
            bool: True if new motion detected, False otherwise
        """
        current_state = self.read_motion()
        current_time = time.time()
        
        # Detect rising edge (motion started)
        if current_state and not self.last_motion_state:
            if (current_time - self.last_motion_time) > self.motion_debounce_time:
                self.last_motion_time = current_time
                self.last_motion_state = current_state
                return True
        
        self.last_motion_state = current_state
        return False

    def get_last_motion_time(self):
        """
        Get timestamp of last motion detection
        
        Returns:
            float: Timestamp of last motion
        """
        return self.last_motion_time

    def set_debounce_time(self, debounce_seconds):
        """
        Set debounce time for motion detection
        
        Args:
            debounce_seconds (float): Debounce time in seconds
        """
        self.motion_debounce_time = debounce_seconds

    def self_test(self):
        """
        Perform self-test of the PIR sensor
        
        Returns:
            bool: True if sensor responds, False otherwise
        """
        try:
            # Basic test: check if we can read from the pin
            test_read1 = GPIO.input(self.pir_pin)
            time.sleep(0.01)
            test_read2 = GPIO.input(self.pir_pin)
            
            # If we can read valid values (0 or 1), sensor is working
            return (test_read1 in [0, 1]) and (test_read2 in [0, 1])
        except Exception as e:
            print(f"PIR self-test failed: {e}")
            return False

    def cleanup(self):
        """Clean up GPIO resources"""
        GPIO.cleanup()
