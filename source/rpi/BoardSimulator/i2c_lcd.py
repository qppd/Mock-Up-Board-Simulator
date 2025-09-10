# i2c_lcd.py
# I2C LCD simulation for Raspberry Pi

class I2CLcd:
    def __init__(self, address, cols, rows):
        self.address = address
        self.cols = cols
        self.rows = rows
        self.display = ['' for _ in range(rows)]

    def begin(self):
        print(f"LCD initialized at address {hex(self.address)} with size {self.cols}x{self.rows}")

    def print(self, message):
        self.display[0] = message[:self.cols]
        print(f"LCD: {self.display[0]}")

    def clear(self):
        self.display = ['' for _ in range(self.rows)]
        print("LCD cleared")
