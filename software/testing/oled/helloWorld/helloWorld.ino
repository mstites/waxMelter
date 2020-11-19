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
  oled.print("LINE 1: A yellow line \n");
  oled.print("LINE 2: A yellow line \n");
  oled.println();
  oled.println("Temperature = ");
}
//------------------------------------------------------------------------------
void loop() {}
