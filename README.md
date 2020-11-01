# waxMelter
Custom designed board and software for a PID controlled thermocouple feedback wax melter.

## Board Plan
The board will regulate the container temperature and monitor the temperature of the wax to help it reach the right temperature and stay there. It will control a hot plate, which will heat a container containing the wax. The board will use temperature monitoring, temperature control via a hot plate, and remote control through a web platform.

## Parts List
* [Adafruit HUZZAH32](https://www.adafruit.com/product/3591)
* [Thermocouple Amplifier AD8495](https://www.adafruit.com/product/1778)
* [K Type Thermocouple](https://www.ebay.com/itm/1M-K-Type-Thermocouple-Sensor-Digital-Thermometer-Temperature-Meter-TK80-4JPL8/282896189061?_trkparms=ispr%3D1&hash=item41dded4685:g:auQAAOSwikdau~0I&amdata=enc%3AAQAFAAACYBaobrjLl8XobRIiIML1V4Imu%252Fn%252BzU5L90Z278x5ickk7PdDazAlGltMLJlUhrWsD4ypBzXDBrr3EGbvDCD%252BOTfiTG%252FV6xARDfS6DhxpOawtq0wAUHwzCXVNlrquDkax%252BHLcfCpjODsCClfysZNmLcjKNhUCyZwlivq7Ou%252BSmp%252FNan3zi86bNOppmqGIBMdWiTFE6aq0N55rJ5p610VKqpsEUguP16E0hxd5inmBD20248dYwxENTtrZPJzbteu98xpsFoA6NRRONNO8AFAXX89%252FktMA%252FMYhMde28GDtzsb4Q%252FstK%252BytZGRtmxx2GJeJIjNEnUTIb2ZtmPVtaehLIrKyByYNRhgJ3trD8Uzl1OL1G6Ov1BHTu3ct5z6%252BYjT5Gn1gHn1WTkyAUGmD96HCcMx2kF4PVEo171c0GnQmpCPSK%252Ba1t5ZcDgoZA4wjEhZSgphf9bAZoL%252BTzL9r2Q1FGZkeqwcEwrHTXMSumlt%252FEBQSmBNDBf%252BVZwOZV1DEa5zsk8uRSAahogXBhsBiTncrrOCZMk1M%252Fb6JNapN67fctFa4vXdXtx75DZtH7jcddYi4seuXT9uvLQ%252BGizccfA5TaPQpMoAGivS1TTwWy26Bz2oLF4qjKl8lhn9gjkRAodJqWBnukNI%252BdxfboGJRnL1tupUHN1vHOuuQglrpdetpjcCG2atFgNjEhzRBVdqyXI5UYWCN3p%252FCElSElYjv48XtG05oh%252F%252F0i%252BNC37r2TF%252BFe4q4%252FOPX%252BPEo%252BL9JVKTemFHr%252BIyor4iOTMGOTiiJJ6U9t5zFFEMdbP8pcCW20AqyD6I7%7Ccksum%3A282896189061fe03b50efa204da1abdccf1154f0cbb6%7Campid%3APL_CLK%7Cclp%3A2334524)
* [Hot Plate](https://www.amazon.com/gp/product/B07MMT2SC5/ref=ask_ql_qh_dp_hza)
* [Outlet Power Relay](https://www.amazon.com/Iot-Relay-Enclosed-High-Power-Raspberry/dp/B00WV7GMA2/ref=sr_1_2?dchild=1&keywords=outlet+power+relay&qid=1601856536&sr=8-2)
* [SPI OLED 128x64](https://www.amazon.com/UCTRONICS-SSD1306-Self-Luminous-Display-Raspberry/dp/B072Q2X2LL/ref=sr_1_3?dchild=1&keywords=oled+display+arduino&qid=1602386754&sr=8-3)
* [SPST Switch](digikey.com) x6
* USB Power Supply
* Micro USB Cable
* LD1117V33 x2	
* 0.1uF Cermic Capacitor x6
* 10uF Electrolytic Capacitor x4
* 4.7K Resistor 1/8W x6
* 2 Pin Screw Terminal x2
Sockets:
* 16 pin x2
* 12 pin x2
* 4 pin x2

## Implementation Plan
The core of the system is the Adafruit HUZZAH32. I chose this particular micro controller because of its integrated wifi module and common use in IoT device spaces. The ESP32 is an updated version of the popular ESP8266. And has more GPIOs and is faster, and the boards come in around the same price. This last part means there is lots of documentation and support online for communicating with the device over wifi and hosting a web page on the device.
The whole system will be controlled via a locally accessible web page broad-casted by the micro controller (my additional area of study). The website will contain the current temperature reading of the wax/container, the target temperature, and have a buttons for controlling the target temperature for the wax, as well as the power of the device. I am not currently planning on implementing a way to communicate with the micro controller remotely, as the use case of my board – making candles – means the user should be on the same local network.
A secondary user interface will be visible via an OLED panel. This will have temperature readings and target temperature information. The OLED will be connected over I2C. Buttons will be placed below the panel to interact via this secondary user interface.
The system will get temperature readings via the thermocouple connected via the thermocouple amplifier (AD8495). I am planning on building in two thermocouple channels, but only putting in components for one due to cost restrictions. With the one thermocouple, I can regulate the outside temperature of the container or the temperature of the wax. With a second thermocouple, I could also track the internal temperature of the wax in real time while tracking the temperature of the container.
The thermocouple will be connected via a 2 position screw terminal. The thermocouple amplifier will be connected to one ADC input channel. 
To control the temperature of the wax, the board will control a hot plate. This will be done by flipping the outlet on and off via the Outlet Power Relay, connected via a digital i/o pin. The specific software implementation will come with time, but a basic bang/bang controller could work with the limits for when to flip the switch determined in testing. Some sort of PID loop could be implemented if this did not work well enough.
All of the modules support 3.3V logic. All power will be supplied by the usb adapter. Power will be stepped down for the micro by the onboard voltage regulator. The LD1117V33 will be used to power the OLED and the thermocouple modules. 

## More Info
See the [Proposal](proposal/proposal.pdf) for more information about the project.
