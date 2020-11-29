/////////////////////////////////////////////////////////////////

#include "Button2.h";

// OlED
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#define I2C_ADDRESS 0x3C // 7bit address, 8bit address is 0x78

/////////////////////////////////////////////////////////////////
// Pin Assignments //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const int PLATE_POWER_PIN = 26;

const int UP_BUTTON_PIN = A10;
const int SELECT_BUTTON_PIN = A9;
const int DOWN_BUTTON_PIN = A8;

/////////////////////////////////////////////////////////////////
// Instantiate objects  /////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// BUTTONS //
Button2 up_button = Button2(UP_BUTTON_PIN);
Button2 select_button = Button2(SELECT_BUTTON_PIN);
Button2 down_button = Button2(DOWN_BUTTON_PIN);

// OlED //
SSD1306AsciiWire oled;

/////////////////////////////////////////////////////////////////
// State Values /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

boolean platePower = false; // pwr to relay controlling hotplate

/////////////////////////////////////////////////////////////////

void setup() {
  // open serial
  Serial.begin(9600);
  while (!Serial) {
    delay(20);
  }
  // init display
  Wire.begin();
  Wire.setClock(400000l);
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  displayStandby(1, 1);
  
  // set button handlers
  up_button.setTapHandler(upPress);
  select_button.setTapHandler(selectPress);
  down_button.setTapHandler(downPress);

  // configure pins
  pinMode(PLATE_POWER_PIN, OUTPUT);
}

/////////////////////////////////////////////////////////////////

void loop() {
  // initalize constant checking
  up_button.loop();
  select_button.loop();
  down_button.loop();

  // global state value to track current state, can be updated by buttons?
  
}

/////////////////////////////////////////////////////////////////
// OlED Display /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void displayTemplate() {
    oled.clear();
    oled.println("LINE 1: A yellow line");
    oled.println("LINE 2: A yellow line");
    oled.println("LINE 3: A blue line ~");
    oled.println("LINE 4: A blue line -");
    oled.println("LINE 5: A blue line *");
    oled.println("LINE 6: A blue line ~");
    oled.println("LINE 7: A blue line -");
    oled.println("LINE 8: A blue line *");
}

// display values
void printScreen(String L1, String L2, String L3, String L4, String L5, String L6, String L7, String L8) {
    oled.clear();
    oled.println(L1);
    oled.println(L2);
    oled.println(L3);
    oled.println(L4);
    oled.println(L5);
    oled.println(L6);
    oled.println(L7);
    oled.println(L8);
}

// move numeric selection arrow
int updateSel (int maxSel, int prevSel, int dir) {
    int sel;
    sel = prevSel + dir;
    if (sel > maxSel) {
      sel = maxSel;
    }
    else if (sel < 1) {
      sel = 1;
    }
    return(sel);
}

// standby screen
int displayStandby(int prevSel, int dir) {
    // pprevSel <- previous selection
    // dir <- direction to move selection in
    // eventually want to return current line selected
    // should pass values into here
    int maxSel = 2; 
    int selection = updateSel(maxSel, prevSel, dir);
    String arrow = String("`");
    String L1 = String("STATUS: STANDBY");
    String blank = String("");
    String L3 = String("Current temp      000");
    String L4 = String("Temp target       000");
    if (selection == 2) {
        String L6 = String("Start heating       ");
        String L7 = String("Change Target Temp  " + arrow);   
        printScreen(L1, blank, L3, L4, blank, L6, L7, blank);
    }
    else {
        String L6 = String("Start heating       " + arrow);
        String L7 = String("Change Target Temp  ");
        printScreen(L1, blank, L3, L4, blank, L6, L7, blank);
    }
    return(selection);
}

/////////////////////////////////////////////////////////////////
// Button Update ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void upPress(Button2& btn) {
    Serial.println("Up: Pressed");
}

void selectPress(Button2& btn) {
    unsigned int time = btn.wasPressedFor();
    if (time < 1000) {
        Serial.println("Select: Regular press");
        togglePlatePower();
    } 
    else {
        Serial.println("Select: long press");
        Serial.println("Turning OFF");
        setPlatePower(false);
    }
}

void downPress(Button2& btn) {
    Serial.println("Down: Pressed");
}

/////////////////////////////////////////////////////////////////
// Plate Control ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// flip state of wax heater
void togglePlatePower() {
  if (platePower) {
    setPlatePower(false);
  }
  else {
    setPlatePower(true);
  }
}

// set the state of wax heater to desired
void setPlatePower(boolean desired) {
   if (platePower == desired) {
       ; // do nothing
   }
   else if (desired == false){
    // set false, turn off 
    platePower = false;
    digitalWrite(PLATE_POWER_PIN, LOW);
   }
   else {
    // set true, turn on
    platePower = true;
    digitalWrite(PLATE_POWER_PIN, HIGH);    
   }
   Serial.print("Plate power: ");
   Serial.println(platePower);
}


/////////////////////////////////////////////////////////////////
