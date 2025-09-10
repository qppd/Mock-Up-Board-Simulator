#ifndef I2CLCD_H
#define I2CLCD_H

#include <LiquidCrystal_I2C.h>

class I2CLcd {
public:
    I2CLcd(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
    void begin();
    void print(const char* message);
    void clear();
private:
    LiquidCrystal_I2C lcd;
    uint8_t cols;
    uint8_t rows;
};

#endif // I2CLCD_H
