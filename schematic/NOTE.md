In my initial design, I did not realize that ADC #2 cannot be used while wifi is on with the ESP32. Both of my analog reads for the thermocouples in my design were on ADC #2. To remedy this in my design, I ran a jumper from SENSOR_0 to A2 and used a knife to take out the A1 path. I did not use the other channel, though a similar remedy could have been implemented.

I recommend in future iterations moving to A2 and A4 instead of A1 and A5 to avoid this issue.
