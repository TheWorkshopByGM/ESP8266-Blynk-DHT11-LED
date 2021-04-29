/*  The Workhop - Arduino Blynk Control V1.0 - February 2021
 *   
 *  This sketch will allow you to control the brightness of a LED 
 *  and read the temperature and humidity on your smartphone
 *  by using the Blynk app.
 *  Blynk is a platform with iOS and Android apps to control
 *  Arduino, Raspberry Pi and the likes over the Internet.
 *  You can easily build graphic interfaces for all your
 *  projects by simply dragging and dropping widgets.
 *  
 *  Downloads, docs, tutorials: http://www.blynk.cc
 *  Sketch generator:           http://examples.blynk.cc 
 *  Blynk community:            http://community.blynk.cc
 *  
 *  Download latest Blynk library here:   
 *  https://github.com/blynkkk/blynk-library/releases/latest
 *  
 *  The needed libraries for the DHT11 can be downloaded here: 
 *  - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
 *  - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
 *  See guide for details on sensor wiring and usage: 
 *  https://learn.adafruit.com/dht/overview
 */

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define LEDPIN 12    // [pin D6] Digital pin connected to the LED
#define DHTPIN 2     // Digital pin connected to the DHT sensor 
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// You should get Auth Token in the Blynk App 
// or via the mail sent to you from Blynk.
// Go to the Project Settings (nut icon).
char auth[] = "Your-Auth-Token";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Your-SSID";
char pass[] = "Your-Pass";

BlynkTimer timer;

// This function writes data to virtual pins every second.
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent(){
  // Get temperature event and print its value.
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, h);
  Blynk.virtualWrite(V1, t);
}

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V2
BLYNK_WRITE(V2)
{
  int PWM_Value = param.asInt(); // assigning incoming value from pin V2 to a variable
  analogWrite(LEDPIN,PWM_Value); // process received value
}

void setup(){
  //Initialize LED to be OFF
  pinMode(LEDPIN,OUTPUT);
  digitalWrite(LEDPIN,LOW);
  
  // Debug console
  Serial.begin(9600);
  dht.begin();

  //Usually the function below is used:
  //Blynk.begin(auth, ssid, pass);
  //However I would recommend this function since you might 
  //encounter problems due to your wifi security settings.
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
}
