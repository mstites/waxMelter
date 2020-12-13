# Testing
Information on preliminary testing I did on testing the functionality of the HUZZAH32, to help with final implementation.

## Environment Setup
1. Install Arduino IDE
2. Install [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
3. Set board to "Arduino ESP32 Feather" and the upload speed to 921600

## Web
I tried a number of different example codes to try to avoid using the ESPAsyncWebServer and AsyncTCP packages. However, none of these were very stable or had the flexibility to meet my needs. I wanted to avoid these packages as they were larger in size and more complex than I originally thought necessary. They also were more difficult to learn than the other ESP web hosting options. 

I left my webui only test code, which relies on the ESPAsyncWebServer and AsyncTCP packages I ended up using. I used Rui Santos's guides and documentation to help me through much of this portion of the project.

I also left webControl.ino which is a piece of test code I worked the longest with that only relied on a simple WiFi package. However, this turned out to be difficult to use because the code had to be continually relooped.

## OLED
I am using [SSD1306Ascii](https://github.com/greiman/SSD1306Ascii) for OLED control. It can be installed via the built in Arduino package manager. I chose this particular package because of its low size requirements. It only supports ASCII characters, but that is the use case of my project. I can explore adding graphics at a later point. 
* helloWorld - simple helloWorld program
* template - line by line display using the maximum number of characters 
* oledUI - OLED only ui for wax melter

## Button
I am using [Button2](https://github.com/LennartHennigs/Button2) for my button management. It can be installed via the built in Arduino package manager. I am using it as it allows me to make my button management code cleaner. It supports a number of button functions including: debouncing, single/double/triple press tracking, and long press tracking. This gives me more options for making a user interface
* buttonTest - basic button functionality test with only built in functions
* Both button2single and button2singleSimple are built in examples, with the correct pin for my board selected.
* button2multiple is my preliminary implementation of managing multiple buttons and tracking long presses on the state button, which I plan to use for power off functionality.

## Relay
Relies on the Button2 package as seen is button testing. Pressing the select button toggles the status of the relay, when connected. The example is "relayControl".

## Temperature
Relies on the Button2 package as seen in testing. Pressing the up button takes a temperature sample. The example is "tempTest". Will need to calibrate the sensor and the curve.
