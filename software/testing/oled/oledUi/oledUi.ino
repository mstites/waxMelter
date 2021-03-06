/////////////////////////////////////////////////////////////////

#include "Button2.h";
#include "RunningMedian.h"

// OLED
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#define I2C_ADDRESS 0x3C // 7bit address, 8bit address is 0x78

/////////////////////////////////////////////////////////////////
// Pin Assignments //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const int PLATE_POWER_PIN = 26;
const int TEMP_SENSOR_PIN = A1;

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

// OLED //
SSD1306AsciiWire oled;

// MEDIAN FOR TEMPS //
RunningMedian t_samples = RunningMedian(10); // reset count every 10 data points

/////////////////////////////////////////////////////////////////
// State Values /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// variables currentTemp
// line up comments
// mode -> had another part to it
// selection -> new variable name (selectedMenuItem)

int currentTemp = 0;            // current temperature
int prevTemp = 0;               // last current temp
int targetTemp = 130;           // target temperature
int flipOffset = 2;             // temperature offset to flip power on/off

boolean heating = false;        // heating
boolean platePower = false;     // pwr to relay controlling hotplate
boolean heated = false;         // at temp

byte buttonMode = 1;            // 1 -> select, 2 -> adjust target temp
int lineSelection = 1;          // line selected on OLED
byte currentScreen = 0;         // 0-STANDBY, 1-HEATING

long currTime;                  // milis 

/////////////////////////////////////////////////////////////////

void setup() {
  // open serial
  Serial.begin(9600);

  // init display
  Wire.begin();
  Wire.setClock(400000l);
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  standbyScreen(1, 1, currentTemp, targetTemp);

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
  // variables camel case
  
  currTime = millis();
  
  // initalize constant checking
  up_button.loop();
  select_button.loop();
  down_button.loop();

  tempLogger();
  controlPlate();
  // if within five of target temp, update every 2 temp
  if ((abs(targetTemp - currentTemp) <= 5) && (abs(currentTemp - prevTemp) >= 2)) { 
    refreshScreen(0, currentTemp, targetTemp);
    prevTemp = currentTemp;
  }
  else if (abs(currentTemp - prevTemp) >= 5) { // otherwise only for steps of 5
    refreshScreen(0, currentTemp, targetTemp);
    prevTemp = currentTemp;
  }
//  refreshScreen(0, currentTemp, targetTemp);

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
        heating = false;
        platePower = false; // should be handled somewhere else
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
  if (buttonMode == 1){               // SELECT
    if (lineSelection == 1) {
       buttonMode = 2;                // change target temp
    }
    else if (currentScreen == 0) {
       changeScreen(1);               // go to heating screen
       heating = true; 
       heated = false;                // reset
    }
    else if (currentScreen == 1) {
       heating = false; 
       heated = false;                // reset
       changeScreen(0);               // go to heating screen     
    }
  }
  else if (buttonMode == 2){          // ADJUST
    buttonMode = 1;                   // back to select mode
  }
}

// BUTTOM/TOP BUTTON FUNCTIONALITY
void arrow(int dir){
  if (buttonMode == 1){               // SELECT
    refreshScreen(dir, currentTemp, targetTemp);
  }
  else if (buttonMode == 2){          // ADJUST
    targetTemp = targetTemp + dir;    // change target temp
    refreshScreen(0, currentTemp, targetTemp); // no selection change
  }
  Serial.println(lineSelection);
}

// CHANGE TO NEW SCREEN
void changeScreen(byte newScreen){
  if (currentScreen != newScreen) {
    currentScreen = newScreen;               // update global
    lineSelection = 1;                // reset counter
    refreshScreen(0, currentTemp, targetTemp);
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
  if (currentScreen == 0) { // STANDBY
    lineSelection = standbyScreen(lineSelection, dir, temp, target);
  }
  else if (currentScreen == 1) { // heating
    lineSelection = heatingScreen(lineSelection, dir, temp, target);
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

// 1 SCREEN - Screen 1
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
// Temperature Management ///////////////////////////////////////
/////////////////////////////////////////////////////////////////

int t_count = 0;
long next = 0;

// READ TEMP
float readTemp(){
  float vOut;
  vOut = analogRead(TEMP_SENSOR_PIN) * (3.3/4095); // 12 bit resolution
  return (vOut - 1.25)/0.009 + 32 + 50;
}

// UPDATES CURRENT TEMP
void tempLogger(){
  if (currTime > next) {
    t_samples.add(readTemp());
    t_count++;
    next = currTime + 100;
    if (t_count == 10) {
       currentTemp = t_samples.getMedian();
       t_count = 0;
    }
  }
}

/////////////////////////////////////////////////////////////////
// Plate Control ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// CONTROL POWER VIA RELAY
void controlPlate() {
  if (heating){
    if ((currentTemp - targetTemp) >= flipOffset) {
      setPlatePower(false);
      heated = true;
    }
    else if (currentTemp <= (targetTemp - flipOffset)) {
      setPlatePower(true);
      heated = true;
    }
  }
  else if (platePower){ // off with plate on
      setPlatePower(false);
  }
}

// SET DESIRED STATE
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
