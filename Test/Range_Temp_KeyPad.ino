#include <Keypad.h>
#include <dht.h>
#include <NewPing.h>
  
#define TRIGGER_PIN  11
#define ECHO_PIN     12
#define MAX_DISTANCE 200
dht DHT;
#define DHT11_PIN 10

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

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

   
void setup() {
  Serial.begin(115200);
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
  }

 
