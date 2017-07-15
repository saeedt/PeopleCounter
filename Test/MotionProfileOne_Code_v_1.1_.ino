#include <elapsedMillis.h>
#include <LiquidCrystal.h>
#include <NewPing.h>
#include <DS1307.h>
#include <dht.h>


#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define MAX_DISTANCE 200
dht DHT;
#define DHT11_PIN 10



#define SDA A4
#define SCL A5

#define MID (MAX_DISTANCE/2)
#define ATH MAX_DISTANCE - (MAX_DISTANCE*.10)

#define TSEXPIRE 10000
elapsedMillis timeElapsed;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
DS1307 rtc(SDA, SCL);
Time t = rtc.getTime();

double countSamp = 0;


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 rtc.begin();
 rtc.halt(true);
 // The following lines can be uncommented to set the time
  //rtc.setDOW(FRIDAY);        // Set Day-of-Week to SUNDAY
  //rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(1, 1, 2016);   // Set the date to October 3th, 2010
}


void loop() {
  // Start time
  StartTime();


    //Start Actual Sampling
  Sample();
  
  //Begin Temporary Sampling
  Temp_Sampling();

  //Add Count number of samples collected 
  countSamp = countSamp +1;
}

double Sample()
{
  double x;
  x = 0;
  return x;

}


struct TSC
{
  //Temporary Sampling Count Structure
  //variable holds temp
  float tmp;
  //Variable  holds time
  Time t;
  //variable holds distance
  float d;
  //variable holds zone number
  int z;
  //variable houlds temporary sample count
  int c;
};


void Temp_Sampling()
{

  
  float temp = DHT.temperature;
  float dist = sonar.ping_cm_tc(temp);
  int zone = ZoneDetector(dist);


  TSC samp = {temp, t, dist, zone, countSamp};
  
  if(samp.z ==1 && samp.c==0 && samp.z != 1 && timeElapsed >= TSEXPIRE)
  {
    //start new tempSampling
    TempSample(samp.c);
    samp.z = 1;
  }
  else if(samp.z == 2 && samp.c==0 && samp.z != 2 && timeElapsed >= TSEXPIRE)
  {
    //start new tempSample
    TempSample(samp.c);
    samp.z = 2;
  }  

  
}

void TempSample(double n)
{
  
}



double ZoneDetector(double distance)
{
  double testZone;
  
    if(MID < distance && distance <= ATH)
    { 
      testZone = 1;
    }
    else if(MID >= distance && distance > 0)
    {
      testZone = 2;
    }
    else 
    {
      testZone = 0;
    }
  return testZone;
}











void StartTime()
{
 // Get data from the DS1307
  t = rtc.getTime();
  
  // Send date over serial connection
  Serial.print("Today is the ");
  Serial.print(t.date, DEC);
  Serial.print(". day of ");
  Serial.print(rtc.getMonthStr());
  Serial.print(" in the year ");
  Serial.print(t.year, DEC);
  Serial.println(".");
  
  // Send Day-of-Week and time
  Serial.print("It is the ");
  Serial.print(t.dow, DEC);
  Serial.print(". day of the week (counting monday as the 1th), and it has passed ");
  Serial.print(t.hour, DEC);
  Serial.print(" hour(s), ");
  Serial.print(t.min, DEC);
  Serial.print(" minute(s) and ");
  Serial.print(t.sec, DEC);
  Serial.println(" second(s) since midnight.");

  // Send a divider for readability
  Serial.println("  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -");
  
  // Wait one second before repeating :)
  delay (1000);
  
}
