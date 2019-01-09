#define RELAY 12                                     //compressor relay pin
byte pset = 29;                                      //default pressure

void setup()   {  
  Serial.begin(9600);                                //initialize serial com  
  pinMode(9,INPUT);                           //increase decrease input switch
  pinMode(10,INPUT);                          //increase decrease input switch
  pinMode(RELAY,OUTPUT);                             //compressor relay output mode  
}

void loop() {

//--------SENSOR READING STAGE-------//  
  int sensorVal = analogRead(A1);           // READ SENSOR VALUE THRU ADC 1
  double voltage = (sensorVal*5.0)/1024.0;   // CONVERT SENSOR VALUE TO VOLTAGE
//  float pressure_psi = voltage*23.53;       // CONVERT VOLTAGE TO PRESSURE IN PSI
// int pressure_psi = voltage*23.53;

// int pressure_psi = (voltage - 0.49)/(0.026);
int pressure_psi = (voltage-0.476)/(0.0262);

  
//--------SWITCH READING STAGE-------//
  int a = digitalRead(9);   // READ STATE OF PIN 9                                                     
  int b = digitalRead(10);  // READ STATE OF PIN 10

//--------ADJUST DESIRED PRESSURE STAGE-----//
    if(a == 1){                                                     
      pset++;//increment desired pressure
      delay(500);
      }
    else if (b == 1){                                               
      pset--;                      //decrement desired pressure  
      delay(500);
      }
    else{                                                                     
      pset = pset;                 //do nothing
      }

//--------CONDITIONING STAGE-----//

    if(pset > pressure_psi){       // if desired pressure is greater than current pressure
      digitalWrite(RELAY,LOW);    // Turn on the compressor                                          
      }
    else if(pset < pressure_psi){                                                                     
      digitalWrite(RELAY,HIGH);     // else turn off the compressor                                          
      }
    else{
      digitalWrite(RELAY,HIGH);
      }

//------------DEBUGING AREA-------------//
Serial.print(voltage);                  // raw sensor voltage
Serial.print("V  ");  
Serial.print("   Set Pressure: ");
Serial.print(pset);                     // desired pressure
Serial.print("   Current Pressure: ");
Serial.print(pressure_psi);             // current pressure
Serial.print("  ");
Serial.print(a);                        // switch a status
Serial.print("  ");
Serial.println(b);                      // switch b status
//----------END OF DEBUGING AREA--------//
delay(400);

}
