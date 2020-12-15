# waxMelter
Custom designed board and software for a bangbang controlled thermocouple feedback wax melter. Please see [project page](https://www.mstites.com/items/wax_melter.html) for more information.

## Overview
The board heats and maintains the temperature of a container of candle wax via controlling a hot plate with a relay power control. All code was written in C in an Arduino based environment for an Adafruit ESP32 Huzzah. There is a fully functional OLED and button UI and web based UI.

## Files and Notes
* For final software iteration please see [software/dualUi](software/dualUi). Software depends on the [espressif-esp32](https://github.com/espressif/arduino-esp32#using-through-arduino-ide) and [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) libraries. Any other missing dependencies should be able to be installed through the Arduino IDE (eg [Button2](https://github.com/LennartHennigs/Button2), [RunningMedian](https://github.com/RobTillaart/RunningMedian)).
* Project proposal is located in [proposal](proposal). This was a project for [ENGR3499-02: Microcontrollers for the Real World](http://reifel.org/realworld/) taught by Stan Reifel at Olin College of engineering. Though this project was also a passion work as I [make many candles](http://trandleboutique.com).
* Final schematic, kicad files, and PCB files are located in [schematic](schematic). It should be noted that the final PCB I ended up with I had to make adjustments to. I had to disconnect the A1 to SENSOR_0 connection and reconnect SENSOR_0 to A2. The same would also have to be done with respect to A5 and SENSOR_1 (to some ADC1 pin). This is because the original pins, A1 and A5, I picked are connected to ADC2 on the ESP. Only ADC1 can be used while wifi is enabled ([see here for pin numbering](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather/pinouts)).


## Parts List
* [Adafruit HUZZAH32](https://www.adafruit.com/product/3591)
* [Thermocouple Amplifier AD8495](https://www.adafruit.com/product/1778)
* [K Type Thermocouple](https://www.ebay.com/itm/1M-K-Type-Thermocouple-Sensor-Digital-Thermometer-Temperature-Meter-TK80-4JPL8/282896189061?_trkparms=ispr%3D1&hash=item41dded4685:g:auQAAOSwikdau~0I&amdata=enc%3AAQAFAAACYBaobrjLl8XobRIiIML1V4Imu%252Fn%252BzU5L90Z278x5ickk7PdDazAlGltMLJlUhrWsD4ypBzXDBrr3EGbvDCD%252BOTfiTG%252FV6xARDfS6DhxpOawtq0wAUHwzCXVNlrquDkax%252BHLcfCpjODsCClfysZNmLcjKNhUCyZwlivq7Ou%252BSmp%252FNan3zi86bNOppmqGIBMdWiTFE6aq0N55rJ5p610VKqpsEUguP16E0hxd5inmBD20248dYwxENTtrZPJzbteu98xpsFoA6NRRONNO8AFAXX89%252FktMA%252FMYhMde28GDtzsb4Q%252FstK%252BytZGRtmxx2GJeJIjNEnUTIb2ZtmPVtaehLIrKyByYNRhgJ3trD8Uzl1OL1G6Ov1BHTu3ct5z6%252BYjT5Gn1gHn1WTkyAUGmD96HCcMx2kF4PVEo171c0GnQmpCPSK%252Ba1t5ZcDgoZA4wjEhZSgphf9bAZoL%252BTzL9r2Q1FGZkeqwcEwrHTXMSumlt%252FEBQSmBNDBf%252BVZwOZV1DEa5zsk8uRSAahogXBhsBiTncrrOCZMk1M%252Fb6JNapN67fctFa4vXdXtx75DZtH7jcddYi4seuXT9uvLQ%252BGizccfA5TaPQpMoAGivS1TTwWy26Bz2oLF4qjKl8lhn9gjkRAodJqWBnukNI%252BdxfboGJRnL1tupUHN1vHOuuQglrpdetpjcCG2atFgNjEhzRBVdqyXI5UYWCN3p%252FCElSElYjv48XtG05oh%252F%252F0i%252BNC37r2TF%252BFe4q4%252FOPX%252BPEo%252BL9JVKTemFHr%252BIyor4iOTMGOTiiJJ6U9t5zFFEMdbP8pcCW20AqyD6I7%7Ccksum%3A282896189061fe03b50efa204da1abdccf1154f0cbb6%7Campid%3APL_CLK%7Cclp%3A2334524)
* [Hot Plate](https://www.amazon.com/gp/product/B07MMT2SC5/ref=ask_ql_qh_dp_hza)
* [Outlet Power Relay](https://www.amazon.com/Iot-Relay-Enclosed-High-Power-Raspberry/dp/B00WV7GMA2/ref=sr_1_2?dchild=1&keywords=outlet+power+relay&qid=1601856536&sr=8-2)
* [SPI OLED 128x64](https://www.amazon.com/UCTRONICS-SSD1306-Self-Luminous-Display-Raspberry/dp/B072Q2X2LL/ref=sr_1_3?dchild=1&keywords=oled+display+arduino&qid=1602386754&sr=8-3)
* SPST Switch x6
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
