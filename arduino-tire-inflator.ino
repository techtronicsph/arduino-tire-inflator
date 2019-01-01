#define RELAY 13                                     //compressor relay pin
byte pset = 30;                                      //default pressure

void setup()   {  
  Serial.begin(9600);                                //initialize serial com  
  pinMode(9,INPUT_PULLUP);                           //increase decrease input switch
  pinMode(10,INPUT_PULLUP);                          //increase decrease input switch
  pinMode(RELAY,OUTPUT);                             //compressor relay output mode  
}

void loop() {

//--------SENSOR READING STAGE-------//  
  int sensorVal = analogRead(A1);           // READ SENSOR VALUE THRU ADC 1
  float voltage = (sensorVal*5.0)/1024.0;   // CONVERT SENSOR VALUE TO VOLTAGE
  float pressure_psi = voltage*22.22;       // CONVERT VOLTAGE TO PRESSURE IN PSI

//--------SWITCH READING STAGE-------//
  int a = digitalRead(9);   // READ STATE OF PIN 9                                                     
  int b = digitalRead(10);  // READ STATE OF PIN 10

//--------ADJUST DESIRED PRESSURE STAGE-----//
    if(a == 0 && b == 1){                                                     
      pset++;                      //increment desired pressure
      }
    else if (a == 1 && b == 0){                                               
      pset--;                      //decrement desired pressure  
      }
    else{                                                                     
      pset = pset;                 //do nothing
      }

//--------CONDITIONING STAGE-----//

    if(pset > pressure_psi){       // if desired pressure is greater than current pressure
      digitalWrite(RELAY,HIGH);    // Turn on the compressor                                          
      }
    else{                                                                    
      digitalWrite(RELAY,LOW);     // else turn off the compressor                                          
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
}
