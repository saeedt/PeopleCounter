#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1307.h>
#include <Keypad.h>
#include <dht.h>
#include <NewPing.h>
  
#define TRIGGER_PIN  7
#define ECHO_PIN     8
#define MAX_DISTANCE 200
dht DHT;
#define DHT11_PIN 9

const byte rows = 4; //four rows
const byte cols = 4; //four columns
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[rows] = {A3, A2, A1, A0}; //connect to the row pinouts of the keypad
byte colPins[cols] = {13, 12, 11, 10}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
DS1307 rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
   
void setup() {
  Serial.begin(115200);
  rtc.begin();
  rtc.halt(false);
  lcd.begin(20,4);   // initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.backlight();
   }
     
void loop() {
  delay(50);
  Serial.print("Ping: ");
  int chk = DHT.read11(DHT11_PIN);
  double tmp = DHT.temperature;  
  Serial.print(sonar.ping_cm_tc(tmp));
  Serial.println("cm");
  Serial.println("Temperature: " + (String)tmp);
  Serial.println("Humidity: " + (String)DHT.humidity);
  char key = keypad.getKey();
  if (key != NO_KEY){
    Serial.println("KeyBoard Activity: "+(String)key);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Date: "+(String)rtc.getDateStr());
  lcd.setCursor(0,1);
  lcd.print("Time: "+(String)rtc.getTimeStr());
  }
 
