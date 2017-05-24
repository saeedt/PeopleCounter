#include <dht.h>
#include <NewPing.h>
  
#define TRIGGER_PIN  3
#define ECHO_PIN     2
#define MAX_DISTANCE 200
dht DHT;
#define DHT11_PIN 4

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
  }

 
