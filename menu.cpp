#include "Arduino.h"
#include "menu.h"

byte next[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b01110,
  0b00100
};

byte dual[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b00000,
    0b00000,
    0b11111,
    0b01110,
    0b00100
};

byte previous[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b11111
};

Menu::Menu(String *menus, int lcdRow, int lcdColumn, int menuLength) {
    this->menus = menus;
    this->lcdRow = lcdRow;
    this->lcdColumn = lcdColumn;
    this->menuLength = menuLength;
}

void Menu::printPrevious(LiquidCrystal_I2C lcd) {
    this->currentPointer = this->currentPointer - 
        (this->currentPointer%2==0?(this->lcdRow + 2):(this->lcdRow+1));
    if (this->currentPointer < 0) this->currentPointer = 0;
    this->print(lcd);
}

void Menu::printNext(LiquidCrystal_I2C lcd) {
    if (this->currentPointer >= this->menuLength) return;
    this->print(lcd);
}

void Menu::print(LiquidCrystal_I2C lcd) {
    lcd.clear();
    for (int i = 0; i < this->lcdRow; i++) {
        if (this->currentPointer >= this->menuLength) break;
        lcd.setCursor(0, i);
        lcd.print(this->menus[this->currentPointer]);
        this->currentPointer++;
    }
    lcd.createChar(1, previous);
    lcd.createChar(2, next);
    lcd.createChar(3, dual);
    if (!((this->currentPointer - this->lcdRow) <= 0) && 
        !(this->currentPointer >= this->menuLength)) {
        lcd.setCursor(this->lcdColumn - 1, this->lcdRow - 1);
        lcd.write(byte(3));
    }

    else if (((this->currentPointer - this->lcdRow) <= 0) && 
        !(this->currentPointer >= this->menuLength)) {
        lcd.setCursor(this->lcdColumn - 1, this->lcdRow - 1);
        lcd.write(byte(2));
    }

    else {
        lcd.setCursor(this->lcdColumn - 1, this->lcdRow - 1);
        lcd.write(byte(1));
    }
}

void Menu::reset() {
    this->currentPointer = 0;
}

void Menu::setMenu(String *menus) {
    this->menus = menus;
}

void Menu::setShown(bool shown) {
    this->shown = shown;
}

bool Menu::isShown() {
    return this->shown;
}