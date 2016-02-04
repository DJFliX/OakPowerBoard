# OakPowerBoard
A Power Consumption board using the TCRT5000 to count pulses. The board is intended as a shield for DigiStump's Oak board. Check it out [here](http://digistump.com/oak/)!

##How does it look?
Great question, I've made screenshots of the board I sent to OSH-Park:
![Image of board](https://cdn.rawgit.com/DJFliX/OakPowerBoard/master/images/board.png)

##Does it even work?
I don't know. I'm gonna find out as soon as my boards arrive :).

##Where's the code?
I've made some progress on the code, but unfortunately to test the real sensor I'll have to wait until the board from oshpark arrive. For now, I'm simulating the pulse meter with a button.

![Image of firmware running on the Oak](https://cdn.rawgit.com/DJFliX/OakPowerBoard/master/images/DSC_1045.JPG)

Next on my to-do list are:
- [ ] Show (& calc) total consumption

Future wishes:
- [ ] Add NTP time to sketch
- [ ] Implement timezone offset, apply to current time
- [ ] Keep track of total consumption for the day (and display this)
- [ ] Create some kind of menu for settings
- [ ] Set counter starting position
- [ ] Send consumption to some kind of server?
- [ ] Calibration of pulse detection?

##Resources used
* [SparkFun Eagle Libraries](https://github.com/sparkfun/SparkFun-Eagle-Libraries) by [SparkFun](https://github.com/sparkfun)
* [TCRT5000 Eagle Library](https://github.com/mohamedpsx/Eagle-Libraries) by [mohamedpsx](https://github.com/mohamedpsx)

* [esp8266-oled-ssd1306](https://github.com/squix78/esp8266-oled-ssd1306) by [squix78](https://github.com/squix78))

* [AirSensor Energy Monitor tutorial](http://www.airsensor.co.uk/component/zoo/item/energy-monitor.html)
* [nongnu.org dtostrf](http://www.nongnu.org/avr-libc/user-manual/group__avr__stdlib.html#ga060c998e77fb5fc0d3168b3ce8771d42)
* [SSD1306 Tools Font Converter](http://oleddisplay.squix.ch)