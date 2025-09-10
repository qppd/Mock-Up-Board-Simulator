#include "I2CLcd.h"

I2CLcd::I2CLcd(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
    : lcd(lcd_Addr, lcd_cols, lcd_rows), cols(lcd_cols), rows(lcd_rows) {}

void I2CLcd::begin() {
    lcd.init();
    lcd.backlight();
}

void I2CLcd::print(const char* message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
}

void I2CLcd::clear() {
    lcd.clear();
}
