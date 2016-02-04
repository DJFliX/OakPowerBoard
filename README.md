# OakPowerBoard
A Power Consumption board using the TCRT5000 to count pulses. The board is intended as a shield for DigiStump's Oak board. Check it out [here](http://digistump.com/oak/)!

##How does it look?
Great question, I've made screenshots of the board I sent to OSH-Park:
![Image of board](https://cdn.rawgit.com/DJFliX/OakPowerBoard/master/board.png)

##Does it even work?
I don't know. I'm gonna find out as soon as my boards arrive :).

##Where's the code?
I'm currently working on a sketch that can drive the SSD1306 (using [esp8266-oled-ssd1306](https://github.com/squix78/esp8266-oled-ssd1306) by [squix78](https://github.com/squix78)). 
- [X] Drive the display
- [ ] Calculate current power consumption based on simulated pulse (using a button)
- [ ] Show the calculated consumption in one of the display frames

Next steps will be
- [ ] Show total consumption
- [ ] Set current counter position (assuming you've been using power before)
- [ ] Send consumption to some kind of server?
- [ ] Create some kind of menu for settings
- [ ] Calibration of pulse detection?

##Resources used
* [SparkFun Eagle Libraries](https://github.com/sparkfun/SparkFun-Eagle-Libraries) by [SparkFun](https://github.com/sparkfun)
* [TCRT5000 Eagle Library](https://github.com/mohamedpsx/Eagle-Libraries) by [mohamedpsx](https://github.com/mohamedpsx)