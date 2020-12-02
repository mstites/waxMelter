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

const int UP_BUTTON_PIN = A8;
const int SELECT_BUTTON_PIN = A9;
const int DOWN_BUTTON_PIN = A10;

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

boolean HEATING = false; // heating
boolean PLATE_POWER = false; // pwr to relay controlling hotplate
byte MODE = 1; // 1 -> select, 2 -> adjust target temp
int SEL = 1; // selection
int CURRENT_TEMP = 0; // current temperature
int TARGET_TEMP = 130; // target temperature
byte SCREEN = 0; // CURRENT SCREEN 0-STANDBY, 1-HEATING

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
  standbyScreen(1, 1, CURRENT_TEMP, TARGET_TEMP);

  delay(1000);
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

  // loop to update temperature
  // loop to manage power plate

  // global state value to track current state, can be updated by buttons?
  
}

/////////////////////////////////////////////////////////////////
// Button Loops /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void upPress(Button2& btn) {
    Serial.println("Up: Pressed");
    arrow(1);
}

void selectPress(Button2& btn) {
    unsigned int time = btn.wasPressedFor();
    if (time < 1000) {
        Serial.println("Select: Regular press");
        middle();
    } 
    else {
        Serial.println("Select: long press");
        Serial.println("Turning OFF");
        HEATING = false;
        PLATE_POWER = false; // should be handled somewhere else
    }
}

void downPress(Button2& btn) {
    Serial.println("Down: Pressed");
    arrow(-1);
}

/////////////////////////////////////////////////////////////////
// Button Func/State Handler ////////////////////////////////////
/////////////////////////////////////////////////////////////////

// MIDDLE BUTTON
void middle(){
  if (MODE == 1){ // SELECT
    if (SCREEN == 0) { // standby
      if (SEL == 1) {
        MODE = 2; // change target temp
      }
      else {
        changeScreen(1); // go to heating screen
        HEATING = true; 
      }
    }
    else if (SCREEN == 1) { // heating
      if (SEL == 1) {
        MODE = 2; // change target temp
      }
      else {
        HEATING = false;
        changeScreen(0); // go to standby screen 
      }
    }
  }
  else if (MODE == 2){ // ADJUST
    MODE = 1; // back to select mode
  }
}

// BUTTOM/TOP BUTTON FUNCTIONALITY
void arrow(int dir){
  if (MODE == 1){ // SELECT
    refreshScreen(dir, CURRENT_TEMP, TARGET_TEMP);
  }
  else if (MODE == 2){ // ADJUST
    TARGET_TEMP = TARGET_TEMP + dir; // change target temp
    refreshScreen(0, CURRENT_TEMP, TARGET_TEMP); // no selection change
  }
  Serial.println(SEL);
}

// CHANGE TO NEW SCREEN
void changeScreen(byte newScreen){
  if (SCREEN != newScreen) {
    SCREEN = newScreen; // update global
    SEL = 1; // reset counter
    refreshScreen(0, CURRENT_TEMP, TARGET_TEMP);
  }
}

/////////////////////////////////////////////////////////////////
// OLED Display /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// WRITE SCREEN
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


// COMPUTE LOCATION (selction with limits)
int compSelection (int maxSel, int prevSel, int dir) {
    int sel;
    sel = prevSel - dir;
    if (sel > maxSel) {
      sel = maxSel;
    }
    else if (sel < 1) {
      sel = 1;
    }
    return(sel);
}

// REFRESH SCREEN
void refreshScreen(int dir, int temp, int target) { // should be done immediately
  if (SCREEN == 0) { // STANDBY
    SEL = standbyScreen(SEL, dir, temp, target);
  }
  else if (SCREEN == 1) { // HEATING
    SEL = heatingScreen(SEL, dir, temp, target);
  }
}

// STANDBY SCREEN - Screen 0
int standbyScreen(int prevSel, int dir, int temp, int target) {
    int maxSel = 2; 
    int selection = compSelection(maxSel, prevSel, dir);
    String arrow = String("`");
    String tempCurrent = String(temp);
    String tempTarget = String(target);
    String blank = String("");
    String L1 = String("STATUS: STANDBY");
    String L3 = String("Current temp      " + tempCurrent);
    String L4 = String("Temp target       " + tempTarget);
    if (selection == 1) {
        String L8 = String("Start heating       ");
        String L6 = String("Change Target Temp  " + arrow);
        printScreen(L1, blank, L3, L4, blank, L6, blank, L8);
    }
    else {
        String L8 = String("Start heating       " + arrow);
        String L6 = String("Change Target Temp  ");   
        printScreen(L1, blank, L3, L4, blank, L6, blank, L8);
    }
    return(selection);
}

// HEATING SCREEN - Screen 1
int heatingScreen(int prevSel, int dir, int temp, int target) {
    int maxSel = 2; 
    int selection = compSelection(maxSel, prevSel, dir);
    String arrow = String("`");
    String tempCurrent = String(temp);
    String tempTarget = String(target);
    String blank = String("");
    String L1 = String("STATUS: HEATING");
    String L3 = String("Current temp      " + tempCurrent);
    String L4 = String("Temp target       " + tempTarget);
    if (selection == 1) {
        String L6 = String("Change Target Temp  " + arrow);
        String L8 = String("Cancel/Exit         ");
        printScreen(L1, blank, L3, L4, blank, L6, blank, L8);
    }
    else {
        String L6 = String("Change Target Temp  ");
        String L8 = String("Cancel/Exit         " + arrow);
        printScreen(L1, blank, L3, L4, blank, L6, blank, L8);
    }
    return(selection);
}

/////////////////////////////////////////////////////////////////
// Plate Control ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// flip state of wax heater
void togglePlatePower() {
  if (PLATE_POWER) {
    setPlatePower(false);
  }
  else {
    setPlatePower(true);
  }
}

// set the state of wax heater to desired
void setPlatePower(boolean desired) {
   if (PLATE_POWER == desired) {
       ; // do nothing
   }
   else if (desired == false){
    // set false, turn off 
    PLATE_POWER = false;
    digitalWrite(PLATE_POWER_PIN, LOW);
   }
   else {
    // set true, turn on
    PLATE_POWER = true;
    digitalWrite(PLATE_POWER_PIN, HIGH);    
   }
   Serial.print("Plate power: ");
   Serial.println(PLATE_POWER);
}


/////////////////////////////////////////////////////////////////
