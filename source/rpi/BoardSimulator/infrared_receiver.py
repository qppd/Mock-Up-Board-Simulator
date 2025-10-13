# infrared_receiver.py
# Infrared Receiver simulation for Raspberry Pi

import time
try:
    import RPi.GPIO as GPIO
    import lirc
except ImportError:
    # Fallback for development/testing without actual RPi hardware
    class MockGPIO:
        BCM = "BCM"
        IN = "IN"
        PUD_UP = "PUD_UP"
        
        @staticmethod
        def setmode(mode):
            pass
        
        @staticmethod
        def setup(pin, mode, pull_up_down=None):
            pass
        
        @staticmethod
        def input(pin):
            return 1
        
        @staticmethod
        def cleanup():
            pass
    
    class MockLirc:
        @staticmethod
        def init():
            return True
        
        @staticmethod
        def nextcode():
            return []
        
        @staticmethod
        def deinit():
            pass
    
    GPIO = MockGPIO()
    lirc = MockLirc()

class InfraredReceiver:
    def __init__(self, pin):
        """
        Initialize Infrared Receiver
        
        Args:
            pin (int): GPIO pin number for IR receiver
        """
        self.pin = pin
        self.initialized = False
        self.last_code = None
        self.last_time = 0
        self.repeat_delay = 0.2  # 200ms repeat delay
        
        # Common IR remote code mappings (NEC protocol simulation)
        self.code_mappings = {
            "KEY_POWER": "POWER",
            "KEY_PREVIOUS": "PREV", 
            "KEY_NEXT": "NEXT",
            "KEY_PLAYPAUSE": "PLAY_PAUSE",
            "KEY_VOLUMEDOWN": "VOL_DOWN",
            "KEY_VOLUMEUP": "VOL_UP",
            "KEY_0": "0",
            "KEY_1": "1",
            "KEY_2": "2",
            "KEY_3": "3",
            "KEY_4": "4",
            "KEY_5": "5",
            "KEY_6": "6",
            "KEY_7": "7",
            "KEY_8": "8",
            "KEY_9": "9"
        }
    
    def begin(self):
        """Initialize IR receiver"""
        try:
            GPIO.setmode(GPIO.BCM)
            GPIO.setup(self.pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
            
            # Initialize LIRC for IR code reading
            if lirc.init():
                self.initialized = True
                print("InfraredReceiver: Initialized successfully")
            else:
                print("InfraredReceiver: LIRC initialization failed")
                self.initialized = False
                
        except Exception as e:
            print(f"InfraredReceiver: Initialization failed - {e}")
            self.initialized = False
    
    def is_data_available(self):
        """Check if IR data is available"""
        if not self.initialized:
            return False
        
        try:
            codes = lirc.nextcode()
            return len(codes) > 0
        except:
            return False
    
    def read_code(self):
        """
        Read IR code from receiver
        
        Returns:
            str: IR command string or None if no data
        """
        if not self.initialized:
            return None
        
        try:
            codes = lirc.nextcode()
            if codes:
                raw_code = codes[0]
                current_time = time.time()
                
                # Handle repeat suppression
                if (raw_code != self.last_code or 
                    (current_time - self.last_time) > self.repeat_delay):
                    self.last_code = raw_code
                    self.last_time = current_time
                    return raw_code
                else:
                    return None  # Suppress repeat
            return None
            
        except Exception as e:
            print(f"InfraredReceiver: Read error - {e}")
            return None
    
    def decode_command(self, code):
        """
        Decode IR code to human-readable command
        
        Args:
            code (str): Raw IR code
            
        Returns:
            str: Decoded command name
        """
        if code in self.code_mappings:
            return self.code_mappings[code]
        else:
            return f"UNKNOWN_{code}"
    
    def self_test(self):
        """
        Perform self-test of IR receiver
        
        Returns:
            bool: True if test passed, False otherwise
        """
        if not self.initialized:
            return False
        
        try:
            # Check if GPIO pin is readable
            state = GPIO.input(self.pin)
            return True  # If we can read the pin, consider it working
        except Exception as e:
            print(f"InfraredReceiver: Self-test failed - {e}")
            return False
    
    def enable_repeats(self, enable):
        """Enable or disable repeat code detection"""
        self.repeat_delay = 0 if enable else 0.2
    
    def cleanup(self):
        """Clean up IR receiver resources"""
        if self.initialized:
            try:
                lirc.deinit()
                print("InfraredReceiver: Cleanup completed")
            except Exception as e:
                print(f"InfraredReceiver: Cleanup error - {e}")
    
    def get_signal_strength(self):
        """
        Simulate signal strength reading
        
        Returns:
            int: Signal strength (0-100)
        """
        if not self.initialized:
            return 0
        
        # Simulate signal strength based on pin state
        try:
            state = GPIO.input(self.pin)
            return 85 if state == 1 else 15  # Simulate good/poor signal
        except:
            return 0