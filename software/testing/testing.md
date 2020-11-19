# Testing
Information on testing the functionality of the HUZZAH32, to help with final implementation.

## Environment Setup
* Install Arduino IDE
* Install [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
* Set board to "Arduino ESP32 Feather" and the upload speed to 921600

## Web Hosting
### To Do
* Explore having html files loaded in
### Example Files
* [rgbWeb](https://diyusthad.com/2019/10/controlling-rgb-lights-from-esp32-web-server.html) - RGB control via web server
* webControl - testing based of example files for moving towards iteration
* [cam](https://randomnerdtutorials.com/esp32-cam-take-photo-display-web-server/) - Camera control via web server, featuring a neater way of handeling html data and javascript comm with the unit
#### Built in Examples
* WebUpdate - upload a new binary to load
* SDWebServer - host web server from SD card with full webserver

## OLED

## Button
* buttonTest - basic button functionality test with only built in functions
### Button2
I am using [Button2](https://github.com/LennartHennigs/Button2) for my button management. It can be installed via the built in Arduino package manager. I am using it as it allows me to make my button management code cleaner. It supports a number of button functions including: debouncing, single/double/triple press tracking, and long press tracking. This gives me more options for making a user interface
* Both button2single and button2singleSimple are built in examples, with the correct pin for my board selected.
* button2multiple is my preliminary implementation of managing multiple buttons and tracking long presses on the state button, which I plan to use for power off functionality.
