const int MainMotor = 2;  //pwm wiper enA

const int DrillMotor = 3; // pwm drill motor

const int HallSensor1 = 8; // Center drill sensor


const int buttonUP = 7;
const int buttonDN = 6;

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

 
  pinMode(HallSensor1,INPUT_PULLUP);
 pinMode(buttonUP,INPUT_PULLUP);
 pinMode(buttonDN,INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
   
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
 
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(3,OUTPUT);
 
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
    HallSense1 = digitalRead(HallSensor1);
    
    }

  while(1){

    Serial.println("Motor Stopped"); //delay(1000);
    
    Serial.println("Drill 1 Selected"); //delay(500);

    Serial.println("Drill Mode");// delay(500);
     Serial.println("actuator stop");
    
    int buttonUpSense = digitalRead(buttonUP);
    int buttonDnSense = digitalRead(buttonDN);

    while(!buttonUpSense){
      //digitalWrite(motor1_in1, LOW);
   //   digitalWrite(motor1_in2, HIGH);
      Serial.println("GOING UP");
      buttonUpSense = digitalRead(buttonUP);
      if(buttonUpSense == 1){break;}
      }
      
   while(!buttonDnSense){
    //  digitalWrite(motor1_in1, HIGH);
    //  digitalWrite(motor1_in2, LOW);
      Serial.println("GOING DOWN");
      buttonDnSense = digitalRead(buttonDN);
       if(buttonDnSense == 1){break;}
      }


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
     // digitalWrite(motor1_in1, LOW);
     // digitalWrite(motor1_in2, HIGH);
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("RETRACTING");
      lcd.setCursor(1, 1);
      lcd.print("Please Wait...");
      delay(1000);
      Serial.println("RETRACTING");
      //retract actuator
      
        break;
    }
  }}

 
void menuItem2() { // Function executes when you select the Green item from main menu
  int activeButton = 0;
 
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Green On");
 
  digitalWrite(12,HIGH);
 
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
        digitalWrite(12,LOW);
        break;
    }
  }
}
 
void menuItem3() { // Function executes when you select the Red item from main menu
  int activeButton = 0;
 
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Red On");
 
  digitalWrite(11,HIGH);
 
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
        digitalWrite(11,LOW);
        break;
    }
  }
}
 
void menuItem4() { // Function executes when you select the Purple item from main menu
  int activeButton = 0;
 
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Purple On");
 
  digitalWrite(3,HIGH);
 
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
        digitalWrite(3,LOW);
        break;
    }
  }
}
