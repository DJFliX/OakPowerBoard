#include <Wire.h>
//https://github.com/squix78/esp8266-oled-ssd1306
#include "SSD1306.h"
#include "SSD1306Ui.h"

//Resources included with the sketch
#include "roboto_10.h"
#include "roboto_12.h"
#include "roboto_thin_20.h"
#include "images.h"

#define PULSES_PER_KWH 375
#define WH_PER_PULSE (1000.0F / PULSES_PER_KWH)

int pulse_count_current_kwh;

float consumption_total = 56712.0f;
char consumption_total_str[10] = "56712.00";

float consumption_current = 0.0F;
char consumption_current_str[6] = "00000";

long time_since_last_pulse = millis();

// Initialize the oled display for address 0x3c
SSD1306   display(0x3c, 0, 2);
SSD1306Ui ui     ( &display );

bool draw_totalcount_frame(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(0 + x, 20, "Total (kWh):");
  display->setFont(Roboto_Thin_Plain_20);
  display->drawString(0 + x, 32, consumption_total_str);
  return false;
}

bool draw_currentconsumption_frame(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(0 + x, 20, "Current: (W)");
  display->setFont(Roboto_Thin_Plain_20);
  display->drawString(0 + x, 34, consumption_current_str);
  return false;
}

bool draw_consumptiontoday_frame(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(0 + x, 20, "Today: (kWh)");
  display->setFont(Roboto_Thin_Plain_20);
  display->drawString(0 + x, 34, "  6.82 ");
  return false;
}

bool msOverlay(SSD1306 *display, SSD1306UiState* state) {
  display->drawXbm(32, 0, 8, 8, wifiActive);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0, 0, String("20:41")); //This should be the current time
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128, 0, String(millis()));
  return true;
}

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
bool (*frames[])(SSD1306 *display, SSD1306UiState* state, int x, int y) = { 
  draw_totalcount_frame, 
  draw_currentconsumption_frame, 
  draw_consumptiontoday_frame 
};

// how many frames are there?
int frameCount = 3;

bool (*overlays[])(SSD1306 *display, SSD1306UiState* state) = { 
  msOverlay 
};

int overlaysCount = 1;

bool isConnected = false;

void setup() {
  ui.setTargetFPS(30);
  ui.setActiveSymbole(activeSymbole);
  ui.setInactiveSymbole(inactiveSymbole);
  ui.setIndicatorPosition(TOP);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);

  // Inital UI takes care of initalising the display too.
  ui.init();

  display.flipScreenVertically();
  
  // put your setup code here, to run once:
  // initialize the digital pin as an output.
  pinMode(1, OUTPUT); //LED on Model A   
  pinMode(6, INPUT);//Button pin
}

float get_consumption_from_time_since_last_pulse(long timeSinceLastPulse) {
  long time = millis();
  long dsecs = (time - timeSinceLastPulse) / 100;
  if(dsecs < 5) { //5 dsecs = 0.5 sec: the highest supported interval for this sketch.
    time_since_last_pulse = time;
    return 0.0F;
  }
  float result = (WH_PER_PULSE / (dsecs / 36000.0F));
  return result;
}

bool btn_state = false;

void loop() {
  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
    bool curr_btn_state = (digitalRead(6) == LOW) ? false : true;
    if(curr_btn_state != btn_state) {
      if(curr_btn_state == true) { //PULSE
        
      } else {//PULSE FINISHED
        pulse_count_current_kwh++;
        if(pulse_count_current_kwh == PULSES_PER_KWH) {
          consumption_total++;
          pulse_count_current_kwh = 0;
        }
        consumption_current = get_consumption_from_time_since_last_pulse(time_since_last_pulse);
        dtostrf(consumption_total + ((pulse_count_current_kwh / PULSES_PER_KWH) * 1.0F), 6, 2, consumption_total_str);
        dtostrf(consumption_current, 4, 0, consumption_current_str);
        time_since_last_pulse = millis();
      }
      digitalWrite(1, curr_btn_state ? HIGH : LOW);
      btn_state = curr_btn_state;
    }
    delay(remainingTimeBudget);
  }
}



