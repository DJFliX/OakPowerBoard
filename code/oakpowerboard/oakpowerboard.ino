#include <Wire.h>
//https://github.com/squix78/esp8266-oled-ssd1306
#include "SSD1306.h"
#include "SSD1306Ui.h"

//Resources included with the sketch
#include "font/roboto_10.h"
#include "font/roboto_12.h"
#include "font/roboto_thin_20.h"
#include "images.h"

#include <EEPROM.h>

#include "eepromanything.h"

#define PULSES_PER_KWH 375
#define WH_PER_PULSE (1000.0F / PULSES_PER_KWH)

struct config_t
{
  int state;
  float consumption_total;
  int pulse_count_current_kwh;
} storage;

#define BTN_1 9
#define BTN_2 8
#define BTN_3 7
#define BTN_4 6
#define PULSE_PIN 5
#define DEBUG_LED 10

#define EEPROM_READ_ADDR 0

char consumption_total_str[10] = "00000.00";

float consumption_current = 0.0F;
char consumption_current_str[6] = "Wait";

long time_since_last_pulse = millis();

// Initialize the oled display for address 0x3c
SSD1306   display(0x3c, 0, 2);
SSD1306Ui ui     ( &display );

/* Drawing Resources */
bool draw_totalcount_frame(SSD1306 *display, SSD1306UiState* state, int x, int y);
bool draw_currentconsumption_frame(SSD1306 *display, SSD1306UiState* state, int x, int y);
bool msOverlay(SSD1306 *display, SSD1306UiState* state);

float get_consumption_from_time_since_last_pulse(long timeSinceLastPulse);

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
bool (*frames[])(SSD1306 *display, SSD1306UiState* state, int x, int y) = { 
  draw_totalcount_frame, 
  draw_currentconsumption_frame
};

bool (*overlays[])(SSD1306 *display, SSD1306UiState* state) = { 
  msOverlay 
};

bool isConnected = false;

bool b1_pressed = false;
bool b2_pressed = false;
bool b3_pressed = false;
bool b4_pressed = false;
bool pulse_triggered = false;

void toggle_b1() {
  b1_pressed = true;
}

void toggle_b2() {
  b2_pressed = true;
}

void toggle_b3() {
  b3_pressed = true;
}

void toggle_b4() {
  b4_pressed = true;
}

void trigger_pulse() {
  pulse_triggered = true;
}

void setup() {
  EEPROM_readAnything(EEPROM_READ_ADDR, storage);
  if(storage.state != 0xF7) {
    storage.state = 0xF7;
    storage.consumption_total = 56712.0f;
    storage.pulse_count_current_kwh = 0;
    EEPROM_writeAnything(EEPROM_READ_ADDR, storage);
  }
  dtostrf(storage.consumption_total, 6, 2, consumption_total_str);

  ui.setTargetFPS(30);
  ui.setActiveSymbole(activeSymbole);
  ui.setInactiveSymbole(inactiveSymbole);
  ui.setIndicatorPosition(TOP);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  ui.setTimePerFrame(7000);
  // Add frames
  ui.setFrames(frames, 2);

  // Add overlays
  ui.setOverlays(overlays, 1);

  // Inital UI takes care of initalising the display too.
  ui.init();

  display.flipScreenVertically();
  
  // put your setup code here, to run once:
  // initialize the digital pin as an output.
  pinMode(1, OUTPUT); //LED on Model A   
  attachInterrupt(digitalPinToInterrupt(PULSE_PIN), trigger_pulse, FALLING);
  pinMode(DEBUG_LED, OUTPUT);//Debugging LED
  attachInterrupt(digitalPinToInterrupt(BTN_1), toggle_b1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_2), toggle_b2, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_3), toggle_b3, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_4), toggle_b4, FALLING);
}

void loop() {
  int remainingTimeBudget = ui.update();
  long deadline = millis() + remainingTimeBudget;
  
  if (remainingTimeBudget > 0) {
    yield();
    if(b1_pressed == true) {
      ui.nextFrame();
      b1_pressed = false;
    }
    
    if(b2_pressed == true) {
      ui.previousFrame();
      b2_pressed = false;
    }
    
    if(b3_pressed == true) {
      //handle b3
      b3_pressed = false;
    }
    
    if(b4_pressed = true) {
      //handle b4
      b4_pressed = false;
    }
    
    if(pulse_triggered == true) {
        storage.pulse_count_current_kwh++;
        if(storage.pulse_count_current_kwh == PULSES_PER_KWH) {
          storage.consumption_total++;
          storage.pulse_count_current_kwh = 0;
          EEPROM_writeAnything(EEPROM_READ_ADDR, storage);
        }
        consumption_current = get_consumption_from_time_since_last_pulse(time_since_last_pulse);
        dtostrf(storage.consumption_total + ((((float)storage.pulse_count_current_kwh) * WH_PER_PULSE) / 1000.0F), 6, 2, consumption_total_str);
        dtostrf(consumption_current, 4, 0, consumption_current_str);
        time_since_last_pulse = millis();
        pulse_triggered = false;
    }
    
    digitalWrite(DEBUG_LED, millis() < deadline ? HIGH : LOW);
    //  Particle.delay(millis() - deadline);
  }
}

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

bool msOverlay(SSD1306 *display, SSD1306UiState* state) {
  //display->drawXbm(32, 0, 8, 8, wifiActive);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(0, 0, String("20:41")); //This should be the current time
  //display->setTextAlignment(TEXT_ALIGN_RIGHT);
  //display->drawString(128, 0, String(millis()));
  return true;
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

