#ifndef Menu_h
#define Menu_h

#include <LiquidCrystal_I2C.h>

class Menu {
    private:
        // Variables
        int menuLength = 0;
        int lcdRow = 0;
        int lcdColumn = 0;
        int currentPointer = 0;
        bool shown;
        String *menus;

        // Functions
        void print(LiquidCrystal_I2C lcd);
    public:
        Menu();
        Menu(String *menus, int lcdRow, int lcdColumn, int menuLength);
        
        // Setter
        void setMenu(String *menus);
        void setShown(bool shown);

        // Getter
        bool isShown();

        // Others
        void reset();
        void printPrevious(LiquidCrystal_I2C lcd);
        void printNext(LiquidCrystal_I2C lcd);
};

#endif