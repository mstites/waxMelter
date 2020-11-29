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
const int PlATE_POWER_PIN = 26;

const int UP_BUTTON_PIN = A10;
const int SElECT_BUTTON_PIN = A9;
const int DOWN_BUTTON_PIN = A8;

/////////////////////////////////////////////////////////////////
// Instantiate objects  /////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// BUTTONS //
Button2 up_button = Button2(UP_BUTTON_PIN);
Button2 select_button = Button2(SElECT_BUTTON_PIN);
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
  displayStandby();
  
  // set button handlers
  up_button.setTapHandler(upPress);
  select_button.setTapHandler(selectPress);
  down_button.setTapHandler(downPress);

  // configure pins
  pinMode(PlATE_POWER_PIN, OUTPUT);
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
    oled.println("lINE 1: A yellow line");
    oled.println("lINE 2: A yellow line");
    oled.println("lINE 3: A blue line ~");
    oled.println("lINE 4: A blue line -");
    oled.println("lINE 5: A blue line *");
    oled.println("lINE 6: A blue line ~");
    oled.println("lINE 7: A blue line -");
    oled.println("lINE 8: A blue line *");
}

// display values
void printScreen(String l1, String l2, String l3, String l4, String l5, String l6, String l7, String l8) {
    oled.clear();
    oled.println("lINE 1: A yellow line");
    oled.println("lINE 2: A yellow line");
    oled.println("lINE 3: A blue line ~");
    oled.println("lINE 4: A blue line -");
    oled.println("lINE 5: A blue line *");
    oled.println("lINE 6: A blue line ~");
    oled.println("lINE 7: A blue line -");
    oled.println("lINE 8: A blue line *");
}

// move numeric selection arrow
int updateSel (int maxSel, int prevSel, int dir) {
    int selection;
    selection = prevSel + dir;
    if (selection > maxSel) {
      selection = maxSel;
    }
    else if (selection < 1) {
      selection = 1;
    }
    return(selection);
}

// standby screen
int displayStandby(int prevSel, int dir) {
    // pprevSel <- previous selection
    // dir <- direction to move selection in
    // eventually want to return current line selected
    // should pass values into here
    int maxSel = 2; 
    selection = updateSel(maxSel, prevSel, dir);

    String 
    oled.clear();
    oled.println("STATUS: STANDBY");
    oled.println("");
    oled.println("Current temp      000");
    oled.println("Temp target       000");
    oled.println("");
    oled.println("Start heating");
    oled.println("Change Target Temp");
    oled.println("");
    return selection;
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
