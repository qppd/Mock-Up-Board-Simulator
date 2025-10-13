# matrix_keypad.py
# 4x4 Matrix Keypad simulation for Raspberry Pi

import time
try:
    import RPi.GPIO as GPIO
except ImportError:
    # Fallback for development/testing without actual RPi hardware
    class MockGPIO:
        BCM = "BCM"
        OUT = "OUT"
        IN = "IN"
        PUD_UP = "PUD_UP"
        HIGH = 1
        LOW = 0
        
        @staticmethod
        def setmode(mode):
            pass
        
        @staticmethod
        def setup(pin, mode, pull_up_down=None):
            pass
        
        @staticmethod
        def output(pin, state):
            pass
        
        @staticmethod
        def input(pin):
            return 1  # Default HIGH state
        
        @staticmethod
        def cleanup():
            pass
    
    GPIO = MockGPIO()

class MatrixKeypad:
    def __init__(self, row_pins, col_pins, keymap):
        """
        Initialize 4x4 matrix keypad
        
        Args:
            row_pins (list): GPIO pins for rows [row1, row2, row3, row4]
            col_pins (list): GPIO pins for columns [col1, col2, col3, col4]  
            keymap (list): 4x4 matrix of key characters
        """
        self.row_pins = row_pins
        self.col_pins = col_pins
        self.keymap = keymap
        self.rows = len(row_pins)
        self.cols = len(col_pins)
        self.debounce_time = 0.05  # 50ms debounce
        self.last_key_time = 0
        self.last_key = None
        self.key_pressed = False
        self.initialized = False
    
    def begin(self):
        """Initialize GPIO pins for keypad"""
        try:
            GPIO.setmode(GPIO.BCM)
            
            # Set row pins as outputs (initially HIGH)
            for pin in self.row_pins:
                GPIO.setup(pin, GPIO.OUT)
                GPIO.output(pin, GPIO.HIGH)
            
            # Set column pins as inputs with pull-up resistors
            for pin in self.col_pins:
                GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
            
            self.initialized = True
            print("MatrixKeypad: Initialized successfully")
            
        except Exception as e:
            print(f"MatrixKeypad: Initialization failed - {e}")
            self.initialized = False
    
    def get_key(self):
        """
        Get the currently pressed key with debouncing
        
        Returns:
            char: Pressed key character or None if no key pressed
        """
        if not self.initialized:
            return None
        
        key = self._scan_keys()
        
        # Handle debouncing
        if key is not None:
            current_time = time.time()
            if key != self.last_key or (current_time - self.last_key_time) > self.debounce_time:
                self.last_key = key
                self.last_key_time = current_time
                self.key_pressed = True
                return key
        else:
            self.key_pressed = False
        
        return None
    
    def is_pressed(self):
        """Check if any key is currently pressed"""
        return self.key_pressed
    
    def set_debounce_time(self, debounce_time):
        """Set debounce time in seconds"""
        self.debounce_time = debounce_time
    
    def _scan_keys(self):
        """Scan the keypad matrix for pressed keys"""
        try:
            # Scan each row
            for row in range(self.rows):
                # Set current row LOW, others HIGH
                for i in range(self.rows):
                    GPIO.output(self.row_pins[i], GPIO.LOW if i == row else GPIO.HIGH)
                
                # Small delay for signal stabilization
                time.sleep(0.001)  # 1ms delay
                
                # Check each column
                for col in range(self.cols):
                    if GPIO.input(self.col_pins[col]) == GPIO.LOW:
                        # Key is pressed at this row/col
                        return self.keymap[row][col]
            
            # No key pressed
            return None
            
        except Exception as e:
            print(f"MatrixKeypad: Scan error - {e}")
            return None
    
    def self_test(self):
        """
        Perform self-test of keypad connections
        
        Returns:
            bool: True if test passed, False otherwise
        """
        if not self.initialized:
            return False
        
        try:
            # Test if all pins are properly configured
            for pin in self.row_pins:
                GPIO.output(pin, GPIO.HIGH)
            
            time.sleep(0.01)  # 10ms delay
            
            for pin in self.col_pins:
                if GPIO.input(pin) != GPIO.HIGH:
                    return False
            
            return True
            
        except Exception as e:
            print(f"MatrixKeypad: Self-test failed - {e}")
            return False
    
    def cleanup(self):
        """Clean up GPIO resources"""
        if self.initialized:
            try:
                GPIO.cleanup()
                print("MatrixKeypad: GPIO cleanup completed")
            except Exception as e:
                print(f"MatrixKeypad: Cleanup error - {e}")
    
    def get_key_name(self, key):
        """Get descriptive name for special keys"""
        key_names = {
            '*': 'ASTERISK',
            '#': 'HASH',
            'A': 'KEY_A',
            'B': 'KEY_B', 
            'C': 'KEY_C',
            'D': 'KEY_D'
        }
        return key_names.get(key, key)