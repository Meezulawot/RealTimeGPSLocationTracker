//Realtime GPS Tracker with Nodemcu ESP8266

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// GPIO 4=D2(connect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS)
static const int RXPin = 4, TXPin = 5;   
static const uint32_t GPSBaud = 9600; 

TinyGPSPlus gps; 
WidgetMap myMap(V0);

SoftwareSerial ss(RXPin, TXPin); 
BlynkTimer timer;

float spd;      
float sats;     
String bearing;  

char auth[] = "4Xxdxoj9mAcsU9q3uzQ67OHdMKLLwq69";   //Project authentication key
char ssid[] = "hello";                              // Network
char pass[] = "password1";                          // Corresponding Password

//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now

void setup()
{
  Serial.begin(115200);
  Serial.println();
  ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkGPS);
}

void checkGPS() {
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    Blynk.virtualWrite(V4, "GPS ERROR");
  }
}

void loop()
{
  float latitude = (gps.location.lat());  //Storing the Lat. and Lon.
  float longitude = (gps.location.lng());
 
  Serial.println(latitude);
  Serial.println(latitude);
  
  while (ss.available() > 0)
  {
    // sketch displays information every time a new sentence is correctly encoded.
    if (gps.encode(ss.read()))
      displayInfo();
  }
  Blynk.run();
  timer.run();
}

void displayInfo()
{
  if (gps.location.isValid() )
  {
    float latitude = (gps.location.lat());  
    float longitude = (gps.location.lng());

    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));
    Blynk.virtualWrite(V2, String(longitude, 6));
    
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    spd = gps.speed.kmph();              
    Blynk.virtualWrite(V3, spd);

    sats = gps.satellites.value();    //get number of satellites response
    Blynk.virtualWrite(V4, sats);

    bearing = TinyGPSPlus::cardinal(gps.course.value());  // get the direction
    Blynk.virtualWrite(V5, bearing);
  }

  Serial.println();
}
