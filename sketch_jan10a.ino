#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
int pset = 30;

char auth[] = "5365a7cc8a6b45e7a8257d812173d21d";
char ssid[] = "walangwifi";
char pass[] = "takao06202013";

BlynkTimer timer;

void sendSensor()
{
  int sensorVal = analogRead(A0);           // READ SENSOR VALUE THRU ADC 1
  float voltage = (sensorVal*5.0)/1024.0;   // CONVERT SENSOR VALUE TO VOLTAGE
  int pressure_psi = (voltage-0.476)/(0.0262);
  Blynk.virtualWrite(V5, pressure_psi);
  pinMode(D5, OUTPUT);

 if(pset > pressure_psi){       // if desired pressure is greater than current pressure
      digitalWrite(D5,LOW);    // Turn on the compressor                                          
      }
    else if(pset < pressure_psi){                                                                     
      digitalWrite(D5,HIGH);     // else turn off the compressor                                          
      }
    else{
      digitalWrite(D5,HIGH);
      }

}


BLYNK_WRITE(V7)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.println(pinValue);

  if(pinValue == 1){
  pset++;
  Blynk.virtualWrite(V6, pset);
  }
  else{}
}


BLYNK_WRITE(V8)
{
  int pinValue2 = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.println(pinValue2);

  if(pinValue2 == 1){
  pset--;
  Blynk.virtualWrite(V6, pset);
  }
  else{}
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
