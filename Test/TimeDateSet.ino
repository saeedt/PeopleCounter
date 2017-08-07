#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1307.h>
#include <Keypad.h>

const byte rows = 4; //four rows
const byte cols = 4; //four columns
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[rows] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[cols] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
DS1307 rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address

const byte TIMEOUT = 1000; //max cycles of wait for user input
const int DPDELAY = 1000; //display delay

void setup() {
  rtc.begin();
  rtc.halt(false);
  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.backlight();
  delay(2000);
  inputTime();
}

void loop() {
  delay (DPDELAY);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time: "+(String)rtc.getTimeStr());
  lcd.setCursor(0,1);
  lcd.print("Date: "+ (String)rtc.getDateStr());
}

void inputTime() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time: "+(String)rtc.getTimeStr());
  lcd.setCursor(0,1);
  lcd.print("A: Keep  B: Set");
  byte ctr =0;
  char key;
  do {
    key = keypad.getKey();
    if (key == 'A') {      
       break; //moves forward without setting the time
     } else if (key == 'B') {
       setTime(); //set the time 
       break;
     }    
    ctr++;
    delay (50);
    } while (ctr<TIMEOUT); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Date: "+ (String)rtc.getDateStr());
  lcd.setCursor(0,1);
  lcd.print("A: Keep  B: Set");
  ctr =0;  
  do {
    key = keypad.getKey();
    if (key == 'A') {   
      break; //moves forward without setting the time
    } else if (key == 'B'){
      setDate();//call set date function: set the date 
      break;
    }
    ctr++;
    delay (50);
  } while (ctr<TIMEOUT); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time: "+(String)rtc.getTimeStr());
  lcd.setCursor(0,1);
  lcd.print("Date: "+ (String)rtc.getDateStr());
}

void setTime() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Hour 00-23");
  lcd.setCursor(0,1);
  lcd.print("??");
  boolean done = false;
  char key;
  byte tmp;
  byte ctr = 0;
  uint8_t hour = 0;
  uint8_t min = 0;
  //set hour
  do {
    key = keypad.getKey();
    if (isDigit(key)) {  
      tmp = key - '0';
      if (tmp <3) {
        lcd.setCursor(0,1);
        lcd.print(String(tmp)+'?');
        hour =10 * tmp;
        done = true;
      }
        }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT));
  if (!done){
    lcd.setCursor(0,1);
    lcd.print("0?");
  }
  done = false;
  ctr = 0;
  do {
    key = keypad.getKey();
    if (isDigit(key)) {   
      tmp = key - '0';
      if ((tmp + hour) < 24) {        
        hour += tmp;
        if (hour>9) {
          lcd.setCursor(0,1);
          lcd.print(String(hour));
        } else {
          lcd.setCursor(0,1);
          lcd.print("0"+String(hour));
        }
        done = true;
      }
    }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT)); 
  if (!done) {
    if (hour > 9 ) {
      lcd.setCursor(0,1);
      lcd.print(String(hour));
    } else {
      lcd.setCursor(0,1);
      lcd.print("0"+String(hour));
    }
  }
  delay (DPDELAY);
  //set minute 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Min 00-59");
  lcd.setCursor(0,1);
  lcd.print("??");
  done = false;
  ctr = 0;
  do {
    key = keypad.getKey();
    if (isDigit(key)) {   
      tmp = key - '0';
      if (tmp <6) {
        lcd.setCursor(0,1);
        lcd.print(String(tmp)+"?");
        min =tmp * 10;
        done = true;
      }
    }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT)); 
  if (!done){
    lcd.setCursor(0,1);
    lcd.print("0?");
  }
  done = false;
  ctr = 0;
  do {
    key = keypad.getKey();
    if (isDigit(key)) {   
      tmp = key - '0';
      if (tmp + min < 59) {
        min +=tmp;
        if (min > 9){
          lcd.setCursor(0,1);
          lcd.print(String(min));
        } else {
          lcd.setCursor(0,1);
          lcd.print("0"+String(min));
        }
        done =true;
      }
    }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT));
  if (!done) {
    if (min > 9 ) {
      lcd.setCursor(0,1);
      lcd.print(min);
    } else {
      lcd.setCursor(0,1);
      lcd.print("0"+String(min));
    }   
  }   
  rtc.setTime(hour,min,0);
  delay (DPDELAY);
}

void setDate() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Year 10-30");
  lcd.setCursor(0,1);
  lcd.print("20??");
  boolean done = false;
  char key;
  byte tmp;
  byte ctr = 0; 
  uint8_t day = 1;
  uint8_t maxDays;
  uint8_t month = 1;
  uint16_t year = 2017;
  //set year
  do {
    key = keypad.getKey();
    if (isDigit(key)) {   
      tmp = key - '0';
      if (tmp <4 && tmp >0) {
        lcd.setCursor(0,1);
        lcd.print("20"+String(tmp)+"?");
        year = 2000+(10 * tmp);
        done = true;
      }
        }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT));
  if (!done) {
    lcd.setCursor(0,1);
    lcd.print("201?");
    year = 2010;
  }
  done = false;
  ctr = 0;
  do {
    key = keypad.getKey();
    if (isDigit(key)) {   
      tmp = key - '0';
      if ((tmp + year) < 2031 && (tmp + year) > 2009) {       
        year += tmp;
        lcd.setCursor(0,1);
        lcd.print(String(year));
        done = true;
      }
    }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT)); 
  if (!done) {
    if (year < 2011) year = 2017;   
    lcd.setCursor(0,1);  
    lcd.print(String(year));
  }
  delay (DPDELAY);
  //set month 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Mon. 00-12");
  lcd.setCursor(0,1);
  lcd.print("??");
  done = false;
  ctr = 0;
  do {
    key = keypad.getKey();
    if (isDigit(key)) {   
      tmp = key - '0';
      if (tmp < 2) {
        lcd.setCursor(0,1);
        lcd.print(String(tmp)+"?");
        month =tmp * 10;
        done = true;
      }
    }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT)); 
  if (!done) lcd.print("0?");
  done = false;
  ctr = 0;
  do {
    key = keypad.getKey();
    if (isDigit(key)) {   
      tmp = key - '0';
      if (tmp + month < 13) {
        month +=tmp;
        if (month >9 ) {
          lcd.setCursor(0,1);
          lcd.print(String(month));
        } else {
          lcd.setCursor(0,1);
          lcd.print("0"+String(month));
        }
        done =true;
      }
    }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT));
  if (!done) {
    if (month > 9 ) {
      lcd.setCursor(0,1);
      lcd.print(String(month));
    } else {
      lcd.setCursor(0,1);
      lcd.print("0"+String(month));
    }   
  }
  delay (DPDELAY);
  //set day 
  lcd.clear();
  lcd.setCursor(0,0);
  switch (month){
    case 1://Jan    
    case 3://Mar
    case 5://May
    case 7://Jul
    case 8://Aug
    case 10://Oct
    case 12://Dec
      lcd.print("Enter Day 01-31");
      maxDays = 31;
      break;
    case 4://Apr
    case 6://Jun
    case 9://Sep
    case 11://Nov
      lcd.print("Enter Day 01-30");
      maxDays = 30;
      break;
    case 2://Feb
    //2012,2016,2020,2024,2028 are leap years
      if (year==2012 || year==2016 || year==2020 || year==2024 || year==2028){
        lcd.print("Enter Day 01-29");
        maxDays = 29;
      } else {
        lcd.print("Enter Day 01-28");
        maxDays = 28;
      }
      break;
  } 
  lcd.setCursor(0,1);
  lcd.print("??");
  done = false;
  ctr = 0;
  do {
    key = keypad.getKey();
    if (isDigit(key)) {   
      tmp = key - '0';
      if (tmp <=(maxDays/10)) {
        lcd.setCursor(0,1);
        lcd.print(String(tmp)+"?");
        day =tmp * 10;
        done = true;
      }
    }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT)); 
  if (!done){
    lcd.setCursor(0,1);
    lcd.print("0?");
  }
  done = false;
  ctr = 0;
  do {
    key = keypad.getKey();
    if (isDigit(key)) {   
      tmp = key  - '0';
      if (tmp + day <= maxDays) {
        day +=tmp;
        if (day >9 ) {
          lcd.setCursor(0,1);
          lcd.print(String(day));
        } else {
          lcd.setCursor(0,1);
          lcd.print("0"+String(day));
        }
        done =true;
      }
    }
    delay (50);
    ctr++;
  } while (!done && (ctr < TIMEOUT));
  if (!done) {
    if (day > 9 ) {
      lcd.setCursor(0,1);
      lcd.print(String(day));
    } else {
      lcd.setCursor(0,1);      
      lcd.print("0"+String(day));
    }   
  } 
  rtc.setDate(day,month,year);
  delay (DPDELAY);
}

boolean isDigit(char c) {
  return ((c >='0') && (c <= '9'));
} 
