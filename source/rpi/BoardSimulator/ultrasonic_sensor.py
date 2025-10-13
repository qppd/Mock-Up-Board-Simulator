# ultrasonic_sensor.py
# Ultrasonic Sensor simulation for Raspberry Pi

import time
import random
try:
    import RPi.GPIO as GPIO
except ImportError:
    # Fallback for development/testing
    class MockGPIO:
        BCM = "BCM"
        OUT = "OUT"
        IN = "IN"
        HIGH = 1
        LOW = 0
        
        @staticmethod
        def setmode(mode):
            pass
        
        @staticmethod
        def setup(pin, mode):
            pass
        
        @staticmethod
        def output(pin, state):
            pass
        
        @staticmethod
        def input(pin):
            return 1
        
        @staticmethod
        def cleanup():
            pass
    
    GPIO = MockGPIO()

class UltrasonicSensor:
    def __init__(self, trig_pin, echo_pin, max_distance=400):
        """
        Initialize Ultrasonic Sensor (HC-SR04)
        
        Args:
            trig_pin (int): GPIO pin for trigger
            echo_pin (int): GPIO pin for echo
            max_distance (int): Maximum distance in cm
        """
        self.trig_pin = trig_pin
        self.echo_pin = echo_pin
        self.max_distance = max_distance
        self.initialized = False
        self.last_distance = 0
    
    def begin(self):
        """Initialize ultrasonic sensor"""
        try:
            GPIO.setmode(GPIO.BCM)
            GPIO.setup(self.trig_pin, GPIO.OUT)
            GPIO.setup(self.echo_pin, GPIO.IN)
            GPIO.output(self.trig_pin, GPIO.LOW)
            time.sleep(0.1)  # Settle time
            
            self.initialized = True
            print("UltrasonicSensor: Initialized successfully")
            
        except Exception as e:
            print(f"UltrasonicSensor: Initialization failed - {e}")
            self.initialized = False
    
    def read_distance(self):
        """
        Read distance measurement
        
        Returns:
            float: Distance in cm, -1 if error
        """
        if not self.initialized:
            return -1
        
        try:
            # Send trigger pulse
            GPIO.output(self.trig_pin, GPIO.HIGH)
            time.sleep(0.00001)  # 10us pulse
            GPIO.output(self.trig_pin, GPIO.LOW)
            
            # Simulate measurement (in real implementation, measure echo pulse)
            # For simulation, return random distance with some consistency
            distance = random.uniform(5, self.max_distance * 0.8)
            self.last_distance = distance
            return round(distance, 1)
            
        except Exception as e:
            print(f"UltrasonicSensor: Read error - {e}")
            return -1
    
    def read_distance_filtered(self, samples=3):
        """Read distance with filtering"""
        if not self.initialized:
            return -1
        
        readings = []
        for _ in range(samples):
            distance = self.read_distance()
            if distance > 0:
                readings.append(distance)
            time.sleep(0.01)
        
        if not readings:
            return -1
        
        # Return median for better filtering
        readings.sort()
        return readings[len(readings)//2]
    
    def is_object_detected(self, threshold):
        """Check if object is within threshold distance"""
        distance = self.read_distance()
        return 0 < distance < threshold
    
    def self_test(self):
        """Perform self-test"""
        if not self.initialized:
            return False
        
        try:
            # Test trigger pin
            GPIO.output(self.trig_pin, GPIO.HIGH)
            time.sleep(0.001)
            GPIO.output(self.trig_pin, GPIO.LOW)
            
            # Test a reading
            distance = self.read_distance()
            return distance >= 0
            
        except Exception as e:
            print(f"UltrasonicSensor: Self-test failed - {e}")
            return False
    
    def cleanup(self):
        """Clean up GPIO resources"""
        if self.initialized:
            try:
                GPIO.cleanup()
                print("UltrasonicSensor: GPIO cleanup completed")
            except Exception as e:
                print(f"UltrasonicSensor: Cleanup error - {e}")