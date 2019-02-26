const int PWM = 12;
const int motorDir = 11;

const int powerRelay = 10;
const int drillEnable = 9; // drill enable relay
const int startButton = 8;

const int HallSensor1 = 7;
const int HallSensor2 = 6;
const int HallSensor3 = 5;
const int HallSensor4 = 4;

const int motor1_in1 = 22; // Actuator motor1 Direction  
const int motor1_in2 = 24; // Actuator motor1 Direction
const int motor2_in3 = 26; // Actuator motor2 Direction
const int motor2_in4 = 28; // Actuator motor2 Direction

const int motor3_in1 = 23; // Actuator motor3 Direction
const int motor3_in2 = 25; // Actuator motor3 Direction
const int motor4_in3 = 27; // Actuator motor4 Direction
const int motor4_in4 = 29; // Actuator motor4 Direction

const int relay1 = 14; // Center Drill motor relay 
const int relay2 = 15; // Drill bit 1 motor relay
const int relay3 = 16; // Drill bit 2 motor relay
const int relay4 = 17; // Chamfering Drill motor relay

const int relay5 = 18; // Drill Mode & Main motor for the driver
const int relay6 = 19;
 
const int buttonUP = 20;
const int buttonDN = 21;

int PotValue = 0;
int drillSpeed = 0;


String menuItems[] = {"Center Drill", "Drill Bit 1", "Drill Bit 2", "Chamfering","TEST"};
 
// Navigation button variables
int readKey;
int savedDistance = 0;
 
// Menu control variables
int menuPage = 0;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5);
int cursorPosition = 0;
 
// Creates 3 custom characters for the menu display
byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};
 
byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};
 
byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};
 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// Setting the LCD shields pins
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
 
void setup() {
  pinMode(PWM, OUTPUT);
  pinMode(motorDir, OUTPUT);
  
  pinMode(motor1_in1,OUTPUT);
  pinMode(motor1_in2,OUTPUT);
  pinMode(motor2_in3,OUTPUT);
  pinMode(motor2_in4,OUTPUT);
  pinMode(motor3_in1,OUTPUT);
  pinMode(motor3_in2,OUTPUT);
  pinMode(motor4_in3,OUTPUT);
  pinMode(motor4_in4,OUTPUT);

  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  pinMode(relay4,OUTPUT);
  pinMode(relay5,OUTPUT);
  pinMode(relay6,OUTPUT);

  pinMode(powerRelay, OUTPUT);
  pinMode(drillEnable, OUTPUT);

  pinMode(HallSensor1,INPUT_PULLUP);
  pinMode(HallSensor2,INPUT_PULLUP);
  pinMode(HallSensor3,INPUT_PULLUP);
  pinMode(HallSensor4,INPUT_PULLUP);
 
  pinMode(buttonUP,INPUT_PULLUP);
  pinMode(buttonDN,INPUT_PULLUP);

  pinMode(startButton, INPUT_PULLUP);
      
  // Initializes serial communication
  Serial.begin(9600);
 
  // Initializes and clears the LCD screen
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
 
  // Creates the byte for the 3 custom characters
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);
 
}
 
void loop() {
  mainMenuDraw();
  drawCursor();
  operateMainMenu();
}
 
// This function will generate the 2 menu items that can fit on the screen. They will change as you scroll through your menu. Up and down arrows will indicate your current menu position.
void mainMenuDraw() {
  Serial.print(menuPage);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(menuItems[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}
 
// When called, this function will erase the current cursor and redraw it based on the cursorPosition and menuPage variables.
void drawCursor() {
  for (int x = 0; x < 2; x++) {     // Erases current cursor
    lcd.setCursor(0, x);
    lcd.print(" ");
  }
 
  // The menu is set up to be progressive (menuPage 0 = Item 1 & Item 2, menuPage 1 = Item 2 & Item 3, menuPage 2 = Item 3 & Item 4), so
  // in order to determine where the cursor should be you need to see if you are at an odd or even menu page and an odd or even cursor position.
  if (menuPage % 2 == 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is even and the cursor position is even that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is even and the cursor position is odd that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is odd and the cursor position is even that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is odd and the cursor position is odd that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}
 
 
void operateMainMenu() {
  int activeButton = 0;
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 0: // When button returns as 0 there is no action taken
        break;
      case 1:  // This case will execute if the "forward" button is pressed
        button = 0;
        switch (cursorPosition) { // The case that is selected here is dependent on which menu page you are on and where the cursor is.
          case 0:
            menuItem1();
            break;
          case 1:
            menuItem2();
            break;
          case 2:
            menuItem3();
            break;
          case 3:
            menuItem4();
            break;
         
        }
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case 2:
        button = 0;
        if (menuPage == 0) {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
 
        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
 
        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
 
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
      case 3:
        button = 0;
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
 
        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
 
        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
    }
  }
}
 
// This function is called whenever a button press is evaluated. The LCD shield works by observing a voltage drop across the buttons all hooked up to A0.
int evaluateButton(int x) {
  int result = 0;
  if (x < 50) {
    result = 1; // right
  } else if (x < 195) {
    result = 2; // up
  } else if (x < 380) {
    result = 3; // down
  } else if (x < 790) {
    result = 4; // left
  }
  return result;
}
 
// If there are common usage instructions on more than 1 of your menu items you can call this function from the sub
// menus to make things a little more simplified. If you don't have common instructions or verbage on multiple menus
// I would just delete this void. You must also delete the drawInstructions()function calls from your sub menu functions.
void drawInstructions() {
  lcd.setCursor(0, 1); // Set cursor to the bottom line
  lcd.print("Use ");
  lcd.write(byte(1)); // Up arrow
  lcd.print("/");
  lcd.write(byte(2)); // Down arrow
  lcd.print(" buttons");
}
 
void menuItem1() { // Function executes when you select the Yellow item from main menu
  int activeButton = 0;
 
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Center Drill");

  int HallSense1 = digitalRead(HallSensor1);

  while(HallSense1){

    Serial.println("MOTOR ROTATING");
    //Select rotate mode
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
    //Set motor direction and speed
      digitalWrite(motorDir, LOW);
      analogWrite(PWM, 100); 
    
    HallSense1 = digitalRead(HallSensor1);
    }

    Serial.println("Motor Stopped"); 
    digitalWrite(PWM, LOW);
    delay(100);

  while(1){

    Serial.println("Drill Mode Selected"); 
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);

    Serial.println("Drill 1 Selected");
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);

    PotValue = analogRead(A1);
    drillSpeed = map(PotValue, 0, 1023, 0, 255);
    analogWrite(PWM, drillSpeed);
    
    int buttonUpSense = digitalRead(buttonUP);
    int buttonDnSense = digitalRead(buttonDN);
  //  int startSense = digitalRead(startButton);

  //  if(startSense == 0){digitalWrite(drillEnable, LOW);}
  //  else{digitalWrite(drillEnable, HIGH);}

    while(!buttonUpSense){
      Serial.println("GOING UP");
      
      digitalWrite(motor1_in1, LOW);
      digitalWrite(motor1_in2, HIGH);
    
      buttonUpSense = digitalRead(buttonUP);
      if(buttonUpSense == 1){break;}
      }
      
   while(!buttonDnSense){
      Serial.println("GOING DOWN");
     
      digitalWrite(motor1_in1, HIGH);
      digitalWrite(motor1_in2, LOW);
     
      buttonDnSense = digitalRead(buttonDN);
      if(buttonDnSense == 1){break;}
      }

    Serial.println("Actuator stopped");
    digitalWrite(motor1_in1, LOW);
    digitalWrite(motor1_in2, LOW);
    
  readKey = analogRead(0);
    if (readKey > 400 && readKey < 590) {break;}

  }  
 
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
      Serial.println("Rectracting");
      digitalWrite(motor1_in1, LOW);
      digitalWrite(motor1_in2, HIGH);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("RETRACTING");
      lcd.setCursor(1, 1);
      lcd.print("Please Wait...");
      delay(5000);
          
        break;
    }
  }}

 
void menuItem2() { // Function executes when you select the Green item from main menu
  int activeButton = 0;
 
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Drill Bit 1");

 int HallSense2 = digitalRead(HallSensor2);

  while(HallSense2){

    //Select rotate mode
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, LOW);
    //Set motor direction and speed
    digitalWrite(motorDir, HIGH);
    analogWrite(PWM, 120); 

    Serial.println("MOTOR ROTATING");
    HallSense2 = digitalRead(HallSensor2);
    
    }

  while(1){

    Serial.println("Motor Stopped"); 
    digitalWrite(PWM, LOW);

    Serial.println("Drill Mode Selected"); 
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);

    Serial.println("Drill 2 Selected");
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    
    PotValue = analogRead(A1);
    drillSpeed = map(PotValue, 0, 1023, 0, 255);
    analogWrite(PWM, drillSpeed);
    
    int buttonUpSense = digitalRead(buttonUP);
    int buttonDnSense = digitalRead(buttonDN);
    int startSense = digitalRead(startButton);

    if(startSense == 0){digitalWrite(drillEnable, LOW);}
    else{digitalWrite(drillEnable, HIGH);}

    while(!buttonUpSense){
      Serial.println("GOING UP");
      
      digitalWrite(motor2_in3, LOW);
      digitalWrite(motor2_in4, HIGH);
    
      buttonUpSense = digitalRead(buttonUP);
      if(buttonUpSense == 1){break;}
      }
      
   while(!buttonDnSense){
      Serial.println("GOING DOWN");
     
      digitalWrite(motor2_in3, HIGH);
      digitalWrite(motor2_in4, LOW);
     
      buttonDnSense = digitalRead(buttonDN);
      if(buttonDnSense == 1){break;}
      }

    Serial.println("Actuator stopped");
    digitalWrite(motor2_in3, LOW);
    digitalWrite(motor2_in4, LOW);
    
  readKey = analogRead(0);
    if (readKey > 400 && readKey < 590) {break;}

  }
  
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
      Serial.println("RETRACTING");
      digitalWrite(motor2_in3, LOW);
      digitalWrite(motor2_in4, HIGH);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("RETRACTING");
      lcd.setCursor(1, 1);
      lcd.print("Please Wait...");
      delay(4000);
      
      
        break;
    }
  }
}
 
void menuItem3() { // Function executes when you select the Red item from main menu
  int activeButton = 0;
 
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Drill Bit 2");

 int HallSense3 = digitalRead(HallSensor3);

  while(HallSense3){

    //Select rotate mode
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, LOW);
    //Set motor direction and speed
    digitalWrite(motorDir, LOW);
    analogWrite(PWM, 120); 

    Serial.println("MOTOR ROTATING");
    HallSense3 = digitalRead(HallSensor3);
    
    }

  while(1){

    Serial.println("Motor Stopped"); 
    digitalWrite(PWM, LOW);

    Serial.println("Drill Mode Selected"); 
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);

    Serial.println("Drill 3 Selected");
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, HIGH);
    
    PotValue = analogRead(A1);
    drillSpeed = map(PotValue, 0, 1023, 0, 255);
    analogWrite(PWM, drillSpeed);
    
    int buttonUpSense = digitalRead(buttonUP);
    int buttonDnSense = digitalRead(buttonDN);
    int startSense = digitalRead(startButton);

    if(startSense == 0){digitalWrite(drillEnable, LOW);}
    else{digitalWrite(drillEnable, HIGH);}
    
    while(!buttonUpSense){
      Serial.println("GOING UP");
      
      digitalWrite(motor3_in1, LOW);
      digitalWrite(motor3_in2, HIGH);
    
      buttonUpSense = digitalRead(buttonUP);
      if(buttonUpSense == 1){break;}
      }
      
   while(!buttonDnSense){
      Serial.println("GOING DOWN");
     
      digitalWrite(motor3_in1, HIGH);
      digitalWrite(motor3_in2, LOW);
     
      buttonDnSense = digitalRead(buttonDN);
      if(buttonDnSense == 1){break;}
      }

    Serial.println("Actuator stopped");
    digitalWrite(motor3_in1, LOW);
    digitalWrite(motor3_in2, LOW);
    
  readKey = analogRead(0);
    if (readKey > 400 && readKey < 590) {break;}

  }
 
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
      Serial.println("RETRACTING");
      digitalWrite(motor3_in1, LOW);
      digitalWrite(motor3_in2, HIGH);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("RETRACTING");
      lcd.setCursor(1, 1);
      lcd.print("Please Wait...");
      delay(4000);
      
        break;
    }
  }
}
 
void menuItem4() { // Function executes when you select the Purple item from main menu
  int activeButton = 0;
 
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Chamfering");
 
  int HallSense4 = digitalRead(HallSensor4);

  while(HallSense4){

    Serial.println("MOTOR ROTATING");
    //Select rotate mode
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
    //Set motor direction and speed
      digitalWrite(motorDir, HIGH);
      analogWrite(PWM, 120); 
    
    HallSense4 = digitalRead(HallSensor4);
    }

  while(1){

    Serial.println("Motor Stopped"); 
    digitalWrite(PWM, LOW);

    Serial.println("Drill Mode Selected"); 
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);

    Serial.println("Drill 4 Selected");
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, LOW);

    PotValue = analogRead(A1);
    drillSpeed = map(PotValue, 0, 1023, 0, 255);
    analogWrite(PWM, drillSpeed);
    
    int buttonUpSense = digitalRead(buttonUP);
    int buttonDnSense = digitalRead(buttonDN);
    int startSense = digitalRead(startButton);

    if(startSense == 0){digitalWrite(drillEnable, LOW);}
    else{digitalWrite(drillEnable, HIGH);}

    while(!buttonUpSense){
      Serial.println("GOING UP");
      
      digitalWrite(motor4_in3, LOW);
      digitalWrite(motor4_in4, HIGH);
    
      buttonUpSense = digitalRead(buttonUP);
      if(buttonUpSense == 1){break;}
      }
      
   while(!buttonDnSense){
      Serial.println("GOING DOWN");
     
      digitalWrite(motor4_in3, HIGH);
      digitalWrite(motor4_in4, LOW);
     
      buttonDnSense = digitalRead(buttonDN);
      if(buttonDnSense == 1){break;}
      }

    Serial.println("Actuator stopped");
    digitalWrite(motor4_in3, LOW);
    digitalWrite(motor4_in4, LOW);
    
  readKey = analogRead(0);
    if (readKey > 400 && readKey < 590) {break;}

  }  
 
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
      Serial.println("RETRACTING");
      digitalWrite(motor3_in1, LOW);
      digitalWrite(motor3_in2, HIGH);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("RETRACTING");
      lcd.setCursor(1, 1);
      lcd.print("Please Wait...");
      delay(4000);
              break;
    }
  }
}
