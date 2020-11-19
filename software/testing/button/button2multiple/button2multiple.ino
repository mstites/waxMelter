/////////////////////////////////////////////////////////////////

#include "Button2.h";

/////////////////////////////////////////////////////////////////
// Instantiate button objects with button pins //////////////////
/////////////////////////////////////////////////////////////////

Button2 up_button = Button2(A10);
Button2 select_button = Button2(A9);
Button2 down_button = Button2(A8);

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(20);
  }
  // set button handlers
  up_button.setTapHandler(upPress);
  select_button.setTapHandler(selectPress);
  down_button.setTapHandler(downPress);
}

/////////////////////////////////////////////////////////////////

void loop() {
  // initalize constant checking
  up_button.loop();
  select_button.loop();
  down_button.loop();
}

/////////////////////////////////////////////////////////////////
// Button Update Functions //////////////////////////////////////
/////////////////////////////////////////////////////////////////

void upPress(Button2& btn) {
    Serial.println("Up: Pressed");
}

void selectPress(Button2& btn) {
    unsigned int time = btn.wasPressedFor();
    if (time < 1000) {
        Serial.println("Select: Regular press");
    } 
    else {
        Serial.println("Select: Long press");
    }
}

void downPress(Button2& btn) {
    Serial.println("Down: Pressed");
}
/////////////////////////////////////////////////////////////////
