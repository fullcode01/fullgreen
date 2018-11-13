#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RealTimeClockDS1307.h>
#include <IRremote.h>
#include "menu.h"

#define LCD_COLUMN 16
#define LCD_ROW 2
#define RECV_PIN 11
#define Display_Clock_Every_N_Seconds 1
#define Display_ShortHelp_Every_N_Seconds 25

// Custom characters
byte checklist[8] = {
  0b00000,
  0b00001,
  0b00001,
  0b10010,
  0b01010,
  0b01010,
  0b00100,
  0b00000
};

// Menus
String strMainMenu[] = {
  "1: Routine",
  "2: Brightness",
  "3: Set Time",
  "4: Set Date",
  "0: Close"
};

String strRoutine[] = {
  "08:00:00",
  "16:00:00",
  "00:00:00"
};

String strRoutineMenu[] = {
  "1: " + strRoutine[0],
  "2: " + strRoutine[1],
  "3: " + strRoutine[2],
  "+: Add New",
  "-: Cancel"
};

String strBrightness = "";
String strSetTime = "<   00:00:00   >";

const int MAIN_MENU_LENGTH = 5;

int routineNumber = 3;
int setTimeCursorPos = 4;
int brightness = 0;
int menuCode = 0;
int lastPressedCode = 0;
int count=0;
int value = 0;

long double routineDuration = 60000;
long double millisTrackerCursor = 0;
long double millisTrackerRoutine = 0;
long double millisTrackerBrightness = 0;

char tmpsetDateChar = '0';
char tmpSetTimeChar = '0';
char formatted[] = "00-00-00 00:00:00x";

bool routineActiveFlag = false;
bool viewTimeFlag = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;

Menu routineMenu(strRoutineMenu, LCD_ROW, LCD_COLUMN, 5);
Menu mainMenu(strMainMenu, LCD_ROW, LCD_COLUMN, MAIN_MENU_LENGTH);
Menu *menuHolder = &mainMenu;

void setup() {
//  Wire.begin();

  lcd.begin();
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  irrecv.enableIRIn(); // Start the receiver
  // digitalWrite(13, OUTPUT);
  lcd.createChar(3, checklist);
  menuHolder->printNext(lcd);
}
 
void loop() {
  RTC.readClock();
  String time = verify(RTC.getHours()) + ":" +
                  verify(RTC.getMinutes()) + ":" +
                  verify(RTC.getSeconds());
  String date = verify(RTC.getDate()) + "-" +
                  verify(RTC.getMonth()) + "-" + "20" +
                  verify(RTC.getYear());
  for (int i =0; i < routineNumber; i++) {
    if (strRoutine[i].equals(time)) {
      routineActiveFlag = true;
      millisTrackerRoutine = millis();
    }
  }
  if (viewTimeFlag) {
    lcd.setCursor(3, 0);
    lcd.print(date);
    lcd.setCursor(4, 1);
    lcd.print(time); 
  }
  if (irrecv.decode(&results)) {
    // Serial.println(results.value);
    switch(results.value) {
      // 0
      case 16738455:
        switch(menuCode) {
          case 0:
            viewTimeFlag = true;
            lcd.clear();
            menuCode = 1;
          break;
          case 1:
            viewTimeFlag = false;
          case 20:
            changeMenu(&mainMenu, 0);
          break;
          case 30:
            insertSetTime('0');
          break;
          default:
          break;
        }
      break;

      // 1
      case 16724175:
        switch(menuCode) {
          case 0:
            changeMenu(&routineMenu, 10);
          break;
          case 30:
            insertSetTime('1');
          break;
        }
        lastPressedCode = 16724175;
      break;

      // 2
      case 1033561079:
      case 16718055:
        switch(menuCode) {
          case 0:
            menuCode = 20;
            lcd.clear();
            lcd.setCursor(0, 0);
            strBrightness = "Brightness: " + (String)map(brightness, 0, 255, 0, 100) + "%";
            lcd.print(strBrightness);
            lcd.setCursor(0, 1);
            lcd.print("0: Back       -+");
            menuHolder->reset();
          break;
          case 30:
            insertSetTime('2');
          break;
          default:
          break;
        }
        lastPressedCode = 16718055;
      break;

      // 3
      case 16743045:
        switch(menuCode) {
          case 0:
            menuCode = 30;
            lcd.clear();
            lcd.setCursor(0, 0);
            strSetTime = "<   " + time + "   >";
            lcd.print(strSetTime);
            lcd.setCursor(0, 1);
            lcd.print("+:Set  -:Back");
            tmpSetTimeChar = strSetTime.charAt(setTimeCursorPos);
          break;
          case 30:
            insertSetTime('3');
          break;
          default:
          break;
        }
        lastPressedCode = 16743045;
      break;

      // 4
      case 2351064443:
      case 16716015:
        switch(menuCode) {
          case 0:
            menuCode = 40;
            lcd.clear();
            lcd.setCursor(0, 0);
            strSetTime = "<  " + date + "  >";
            lcd.print(strSetTime);
            lcd.setCursor(0, 1);
            lcd.print("+:Set  -:Back");
            tmpSetTimeChar = strSetTime.charAt(setTimeCursorPos);
          break;
          case 30:
            insertSetTime('4');
          break;
          default:
          break;
        }
        lastPressedCode = 16716015;
      break;

      // 5
      case 16726215:
        switch(menuCode) {
          case 30:
            insertSetTime('5');
          break;
          default:
          break;
        }
        lastPressedCode = 16726215;
      break;

      // 6
      case 16734885:
        switch(menuCode) {
          case 30:
            insertSetTime('6');
          break;
          default:
          break;
        }
        lastPressedCode = 16734885;
      break;

      // 7
      case 16728765:
        switch(menuCode) {
          case 30:
            insertSetTime('7');
          break;
          default:
          break;
        }
        lastPressedCode = 16728765;
      break;

      // 8
      case 16730805:
        switch(menuCode) {
          case 30:
            insertSetTime('8');
          break;
          default:
          break;
        }
        lastPressedCode = 16730805;
      break;

      // 9
      case 16732845:
        switch(menuCode) {
          case 30:
            insertSetTime('9');
          break;
          default:
          break;
        }
        lastPressedCode = 16730805;
      break;

      // Previous
      case 16720605:
      case 1386468383:
        switch(menuCode) {
          case 30:
            if(setTimeCursorPos == 4) break;
            else {
              moveSetTimeCursor('p');
            }
          break;
          default:
            menuHolder->printPrevious(lcd);
          break;
        }
        lastPressedCode = 16720605;
      break;

      // Next
      case 16712445:
      case 3622325019:
        switch(menuCode) {
          case 30:
            if(setTimeCursorPos == 11) break;
            else {
              moveSetTimeCursor('n');
            }
          break;
          default:
            menuHolder->printNext(lcd);
          break;
        }
        lastPressedCode = 16712445;
      break;

      // -
      case 16769055:
        switch(menuCode) {
          case 10:
            changeMenu(&mainMenu, 0);
          break;
          case 20:
            if (brightness <= 0) break;
            adjustBrightness(brightness--);
          break;
          case 30:
            setTimeCursorPos = 4;
            changeMenu(&mainMenu, 0);
          break;
        }
        lastPressedCode = 16769055;
      break;

      // +
      case 16754775:
        switch(menuCode) {
          case 20:
            if (brightness >= 255) break;
            adjustBrightness(brightness++);
            lastPressedCode = 16754775;
          break;
          case 30:
            if (processSetTime()) changeMenu(&mainMenu, 0);
            else {
              lcd.setCursor(15, 1);
              lcd.print("X");
            }
          break;
        }
      break;

      case 4294967295:
        switch(menuCode) {
          case 20:
            switch(lastPressedCode) {
              case 16754775:
                //if (millis() - millisTrackerBrightness > 10) {
                  if (brightness >= 255) break;
                  adjustBrightness(brightness++);
                  //millisTrackerBrightness = millis();
                //}
              break;
              case 16769055:
                if (brightness <= 0) break;
                adjustBrightness(brightness--);
              break;
            }
          break;
        }
      break;

      default:
        break;
    }
    irrecv.resume(); // Receive the next value
  }
  
  if (menuCode == 30) {
    if ((millis() - millisTrackerCursor) > 400) {
      lcd.setCursor(setTimeCursorPos, 0);
      if (tmpSetTimeChar == strSetTime.charAt(setTimeCursorPos)) {
        lcd.print(" ");
        constructSetTimeString(' ');
      } else {
        lcd.print(tmpSetTimeChar);
        constructSetTimeString(tmpSetTimeChar);
      }
      millisTrackerCursor = millis();
    }
  }

  if (routineActiveFlag) {
    if ((millis() - millisTrackerRoutine) < routineDuration) {
      digitalWrite(8, HIGH);
    } else {
      routineActiveFlag = false;
      digitalWrite(8, LOW);
    }
  }

  #ifdef TEST_Squarewave
  if(count%10 == 0)
  {
    switch(count/10 % 6)
    {
      case 0:
      Serial.print("Squarewave disabled (low impedance): ");
      RTC.sqwDisable(0);
      Serial.println((int) RTC.readData(7));
      break;
      case 1:
      Serial.print("Squarewave disabled (high impedance): ");
      RTC.sqwDisable(1);
      Serial.println((int) RTC.readData(7));
      break;
      case 2:
      Serial.println("Squarewave enabled at 1 Hz");
      RTC.sqwEnable(RTC.SQW_1Hz);
      break;
      case 3:
      Serial.println("Squarewave enabled at 4.096 kHz");
      RTC.sqwEnable(RTC.SQW_4kHz);
      break;
      case 4:
      Serial.println("Squarewave enabled at 8.192 kHz");
      RTC.sqwEnable(RTC.SQW_8kHz);
      break;
      case 5:
      Serial.println("Squarewave enabled at 32.768 kHz");
      RTC.sqwEnable(RTC.SQW_32kHz);
      break;
      default:
      Serial.println("Squarewave test not defined");
    }//switch
  }
  #endif

  #ifdef TEST_StopStart
  if(count%10 == 0)
  {
    if(!RTC.isStopped()) 
    {
      if(RTC.getSeconds() < 45) 
      {
        Serial.println("Stopping clock for 10 seconds");
        RTC.stop();
      }//if we have enough time
    } else {
      RTC.setSeconds(RTC.getSeconds()+11);
      RTC.start();
      Serial.println("Adding 11 seconds and restarting clock");
    }
  }//if on a multiple of 10 counts
  #endif

  #ifdef TEST_1224Switch  
    if(count%10 == 0)
    {
      if(count %20 == 0)
      {
        Serial.println("switching to 12-hour time");
        RTC.switchTo12h();
        RTC.setClock();
      }
      else
      {
        Serial.println("switching to 24-hour time");
        RTC.switchTo24h();
        RTC.setClock();
      }
    }
  #endif

  delay(100);
}

void adjustBrightness(int brightness) {
  analogWrite(9, brightness);
  lcd.setCursor(0, 0);
  strBrightness = "Brightness: " + (String)map(brightness, 0, 255, 0, 100) + "% ";
  lcd.print(strBrightness);
}

void changeMenu(Menu *menu, int code) {
  lcd.clear();
  menuHolder->reset();
  menu->printNext(lcd);
  menuHolder = menu;
  menuCode = code;
}

String verify(int unverified) {
  String un = (String)unverified;
  if (un.length() == 1) return "0"+un;
  else return un;
}

void constructSetTimeString(char insert) {
  strSetTime = strSetTime.substring(0, setTimeCursorPos) + insert +
    strSetTime.substring(setTimeCursorPos + 1, strSetTime.length());
}

void moveSetTimeCursor(char code) {
  constructSetTimeString(tmpSetTimeChar);
  if (code=='n') setTimeCursorPos++;
  else setTimeCursorPos--;
  if (setTimeCursorPos == 9 || setTimeCursorPos == 6) {
    if (code=='n') setTimeCursorPos++;
    else setTimeCursorPos--;
  }
  if (setTimeCursorPos < 4) setTimeCursorPos = 4;
  if (setTimeCursorPos > 11) setTimeCursorPos = 11;
  tmpSetTimeChar = strSetTime.charAt(setTimeCursorPos);
  lcd.setCursor(0, 0);
  lcd.print(strSetTime);
}

void insertSetTime(char ch) {
  constructSetTimeString(ch);
  tmpSetTimeChar = ch;
  lcd.setCursor(0, 0);
  lcd.print(strSetTime);
  moveSetTimeCursor('n');
}

bool processSetTime() {
  // strSetTime = "<   00:00:00   >"
  constructSetTimeString(tmpSetTimeChar);
  String tmpStr = strSetTime.substring(4, 12);
  int timeHours = tmpStr.substring(0, 2).toInt();
  int timeMinutes = tmpStr.substring(3, 5).toInt();
  int timeSeconds = tmpStr.substring(6, tmpStr.length()).toInt();
  if (timeHours > 24) return false;
  if (timeMinutes > 59) return false;
  if (timeSeconds > 59) return false;
  RTC.setHours(timeHours);
  RTC.setMinutes(timeMinutes);
  RTC.setSeconds(timeSeconds);
  RTC.setClock();
  setTimeCursorPos = 4;
  return true;
}
/*
void processCommand() {
  if(!Serial.available()) { return; }
  char command = Serial.read();
  int in,in2;
  switch(command)
  {
    case 'H':
    case 'h':
    in=SerialReadPosInt();
    RTC.setHours(in);
    RTC.setClock();
    Serial.print("Setting hours to ");
    Serial.println(in);
    break;
    case 'I':
    case 'i':
    in=SerialReadPosInt();
    RTC.setMinutes(in);
    RTC.setClock();
    Serial.print("Setting minutes to ");
    Serial.println(in);
    break;
    case 'S':
    case 's':
    in=SerialReadPosInt();
    RTC.setSeconds(in);
    RTC.setClock();
    Serial.print("Setting seconds to ");
    Serial.println(in);
    break;
    case 'Y':
    case 'y':
    in=SerialReadPosInt();
    RTC.setYear(in);
    RTC.setClock();
    Serial.print("Setting year to ");
    Serial.println(in);
    break;
    case 'M':
    case 'm':
    in=SerialReadPosInt();
    RTC.setMonth(in);
    RTC.setClock();
    Serial.print("Setting month to ");
    Serial.println(in);
    break;
    case 'D':
    case 'd':
    in=SerialReadPosInt();
    RTC.setDate(in);
    RTC.setClock();
    Serial.print("Setting date to ");
    Serial.println(in);
    break;
    case 'W':
    Serial.print("Day of week is ");
    Serial.println((int) RTC.getDayOfWeek());
    break;
    case 'w':
    in=SerialReadPosInt();
    RTC.setDayOfWeek(in);
    RTC.setClock();
    Serial.print("Setting day of week to ");
    Serial.println(in);
    break;
    
    case 't':
    case 'T':
    if(RTC.is12hour()) {
      RTC.switchTo24h();
      Serial.println("Switching to 24-hour clock.");
    } else {
      RTC.switchTo12h();
      Serial.println("Switching to 12-hour clock.");
    }
    RTC.setClock();
    break;
    
    case 'A':
    case 'a':
    if(RTC.is12hour()) {
      RTC.setAM();
      RTC.setClock();
      Serial.println("Set AM.");
    } else {
      Serial.println("(Set hours only in 24-hour mode.)");
    }
    break;
    
    case 'P':
    case 'p':
    if(RTC.is12hour()) {
      RTC.setPM();
      RTC.setClock();
      Serial.println("Set PM.");
    } else {
      Serial.println("(Set hours only in 24-hour mode.)");
    }
    break;

    case 'q':
    RTC.sqwEnable(RTC.SQW_1Hz);
    Serial.println("Square wave output set to 1Hz");
    break;
    case 'Q':
    RTC.sqwDisable(0);
    Serial.println("Square wave output disabled (low)");
    break;
    
    case 'z':
    RTC.start();
    Serial.println("Clock oscillator started.");
    break;
    case 'Z':
    RTC.stop();
    Serial.println("Clock oscillator stopped.");
    break;
    
    case '>':
    in=SerialReadPosInt();
    in2=SerialReadPosInt();
    RTC.writeData(in, in2);
    Serial.print("Write to register ");
    Serial.print(in);
    Serial.print(" the value ");
    Serial.println(in2);
    break;    
    case '<':
    in=SerialReadPosInt();
    in2=RTC.readData(in);
    Serial.print("Read from register ");
    Serial.print(in);
    Serial.print(" the value ");
    Serial.println(in2);
    break;

    default:
    Serial.println("Unknown command. Try these:");
    Serial.println(" h## - set Hours       d## - set Date");
    Serial.println(" i## - set mInutes     m## - set Month");
    Serial.println(" s## - set Seconds     y## - set Year");
    Serial.println(" w## - set arbitrary day of Week");
    Serial.println(" t   - toggle 24-hour mode");
    Serial.println(" a   - set AM          p   - set PM");
    Serial.println();
    Serial.println(" z   - start clock     Z   - stop clock");
    Serial.println(" q   - SQW/OUT = 1Hz   Q   - stop SQW/OUT");
    Serial.println();
    Serial.println(" >##,###  - write to register ## the value ###");
    Serial.println(" <##      - read the value in register ##");
    
  }//switch on command
  
} */

/*
//read in numeric characters until something else
//or no more data is available on serial.
int SerialReadPosInt() {
  int i = 0;
  boolean done=false;
  while(Serial.available() && !done)
  {
    char c = Serial.read();
    if (c >= '0' && c <='9')
    {
      i = i * 10 + (c-'0');
    }
    else 
    {
      done = true;
    }
  }
  return i;
} */
