# Testing
Information on testing the functionality of the HUZZAH32, to help with final implementation.

## Environment Setup
1. Install Arduino IDE
2. Install [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
3. Set board to "Arduino ESP32 Feather" and the upload speed to 921600

## Web Hosting
### To Do
* Explore having html files loaded in
### Example Files
* [rgbWeb](https://diyusthad.com/2019/10/controlling-rgb-lights-from-esp32-web-server.html) - RGB control via web server
* webControl - testing based of example files for moving towards iteration
* [cam](https://randomnerdtutorials.com/esp32-cam-take-photo-display-web-server/) - Camera control via web server, featuring a neater way of handeling html data and javascript comm with the unit

## OLED
I am using [SSD1306Ascii](https://github.com/greiman/SSD1306Ascii) for OLED control. It can be installed via the built in Arduino package manager. I chose this particular package because of its low size requirements. It only supports ASCII characters, but that is the use case of my project. I can explore adding graphics at a later point. 
* helloWorld - simple helloWorld program
* template - line by line display using the maximum number of characters 

## Button
I am using [Button2](https://github.com/LennartHennigs/Button2) for my button management. It can be installed via the built in Arduino package manager. I am using it as it allows me to make my button management code cleaner. It supports a number of button functions including: debouncing, single/double/triple press tracking, and long press tracking. This gives me more options for making a user interface
* buttonTest - basic button functionality test with only built in functions
* Both button2single and button2singleSimple are built in examples, with the correct pin for my board selected.
* button2multiple is my preliminary implementation of managing multiple buttons and tracking long presses on the state button, which I plan to use for power off functionality.

## Relay
Relies on the Button2 package as seen is Button2. Pressing the select button toggles the status of the relay, when connected. The example is "relayControl".
