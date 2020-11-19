// Test for minimum program size.

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define I2C_ADDRESS 0x3C // 7bit address, 8bit address is 0x78

SSD1306AsciiWire oled;
//------------------------------------------------------------------------------
void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
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
//------------------------------------------------------------------------------
void loop() {}
