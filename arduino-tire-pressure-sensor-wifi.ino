#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "5365a7cc8a6b45e7a8257d812173d21d";
char ssid[] = "walangwifi";
char pass[] = "takao06202013";

BlynkTimer timer;

void sendSensor()
{
  int sensorVal = analogRead(A0);           // READ SENSOR VALUE THRU ADC 1
  float voltage = (sensorVal*5.0)/1024.0;   // CONVERT SENSOR VALUE TO VOLTAGE
  float pressure_psi = voltage*22.22;   
  
  Blynk.virtualWrite(V5, pressure_psi);
 
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
