#include <Wire.h>
#include <EEPROM.h>

//https://github.com/squix78/esp8266-oled-ssd1306
#include "SSD1306.h"
#include "SSD1306Ui.h"

//Resources included with the sketch
#include "roboto_10.h"
#include "roboto_12.h"
#include "roboto_thin_20.h"
#include "images.h"

#include "eepromanything.h"

#define PULSES_PER_KWH 375
#define WH_PER_PULSE (1000.0F / PULSES_PER_KWH)

#define BTN_1 9
#define BTN_2 8
#define BTN_3 7
#define BTN_4 6
#define PULSE_PIN A0
#define DEBUG_LED 10

//#define DEBUG

#define EEPROM_READ_ADDR 0

struct config_t
{
  int state;
  double consumption_total;
  int pulse_count_current_kwh;
} storage;

double particle_total_kwh = 0;
char consumption_total_str[10] = "00000.00";

double particle_consumption_current = 0;
char consumption_current_str[6] = "Wait";

long time_since_last_pulse = millis();

// Initialize the oled display for address 0x3c
SSD1306   display(0x3c, 0, 2);
SSD1306Ui ui     ( &display );

struct measurement_values {
  int counter = 0;
  int value = 0;
  int hi = 0;
  int lo = 1024;
} analogVal;

struct measurement_strings {
  char current[5] = "0000";
  char hi[5] = "0000";
  char lo[5] = "0000";
} valStrings;

enum app_state {
  HOME,
  CALIBRATE
};

app_state current_state = HOME;

/* Drawing Resources */
bool draw_totalcount_frame(SSD1306 *display, SSD1306UiState* state, int x, int y);
bool draw_currentconsumption_frame(SSD1306 *display, SSD1306UiState* state, int x, int y);
bool draw_menu_frame(SSD1306 *display, SSD1306UiState* state, int x, int y);
bool msOverlay(SSD1306 *display, SSD1306UiState* state);

float get_consumption_from_time_since_last_pulse(long timeSinceLastPulse);

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
bool (*frames[])(SSD1306 *display, SSD1306UiState* state, int x, int y) = { 
  draw_totalcount_frame, 
  draw_currentconsumption_frame
};

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
bool (*others[])(SSD1306 *display, SSD1306UiState* state, int x, int y) = { 
  draw_menu_frame,
  draw_calibration_frame
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

void toggle_b1() { b1_pressed = true; }
void toggle_b2() { b2_pressed = true; }
void toggle_b3() { b3_pressed = true; }
void toggle_b4() { b4_pressed = true; }
void trigger_pulse() { pulse_triggered = true; }

void setup() {
  Particle.variable("total", particle_total_kwh);
  Particle.variable("current", particle_consumption_current);
  EEPROM_readAnything(EEPROM_READ_ADDR, storage);
  if(storage.state != 0xFA) {
    storage.state = 0xFA;
    storage.consumption_total = 56713.0f;
    storage.pulse_count_current_kwh = 370;
    EEPROM_writeAnything(EEPROM_READ_ADDR, storage);
  }
  
  dtostrf(storage.consumption_total, 6, 2, consumption_total_str);
  
  ui.setTargetFPS(30);
  ui.setActiveSymbole(activeSymbole);
  ui.setInactiveSymbole(inactiveSymbole);
  ui.setIndicatorPosition(TOP);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setTimePerFrame(7000);
  ui.setFrames(frames, 2);
  ui.setOverlays(overlays, 1);
  ui.init();

  display.flipScreenVertically();
  
#ifdef DEBUG
  pinMode(DEBUG_LED, OUTPUT);//Debugging LED
#endif
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
      switch(current_state) {
        case HOME:
          ui.setFrames(others, 2);
          ui.disableAutoTransition();
          current_state = CALIBRATE;
          break;
        case CALIBRATE:
          ui.setFrames(frames, 2);
          ui.enableAutoTransition();
          current_state = HOME;
          break;
      }
      //handle b1
      b1_pressed = false;
      yield();
    }
    
    if(b2_pressed == true) {
      //handle b2
      ui.nextFrame();
      b2_pressed = false;
      yield();
    }
    
    if(b3_pressed == true) {
      //handle b3
      ui.previousFrame();
      b3_pressed = false;
      yield();
    }
    
    if(b4_pressed == true) {
      //handle b4
      switch(current_state) {
        case CALIBRATE:
          analogVal.hi = 0;
          dtostrf(analogVal.hi, 4, 0, valStrings.hi);
          analogVal.lo = 1024;
          dtostrf(analogVal.lo, 4, 0, valStrings.lo);
          break;
      }
      b4_pressed = false;
      yield();
    }

    analogVal.counter++;
    if(analogVal.counter == 5) {
      analogVal.value = analogRead(PULSE_PIN);
      if(analogVal.value > analogVal.hi) {
        analogVal.hi = analogVal.value;
        dtostrf(analogVal.hi, 4, 0, valStrings.hi);
      }
      if(analogVal.value < analogVal.lo) {
        analogVal.lo = analogVal.value;
        dtostrf(analogVal.lo, 4, 0, valStrings.lo);
      }
      dtostrf(analogVal.value, 4, 0, valStrings.current);
      analogVal.counter = 0;
    }
    
    /*if(pulse_triggered == true) {
        storage.pulse_count_current_kwh++;
        if(storage.pulse_count_current_kwh == PULSES_PER_KWH) {
          storage.consumption_total++;
          storage.pulse_count_current_kwh = 0;
          EEPROM_writeAnything(EEPROM_READ_ADDR, storage);
        }
        yield();
        particle_consumption_current = get_consumption_from_time_since_last_pulse(time_since_last_pulse);
        particle_total_kwh = storage.consumption_total + ((((float)storage.pulse_count_current_kwh) * WH_PER_PULSE) / 1000.0F);
        dtostrf(particle_total_kwh, 6, 2, consumption_total_str);
        dtostrf(particle_consumption_current, 4, 0, consumption_current_str);
        time_since_last_pulse = millis();
        pulse_triggered = false;
        yield();
    }*/
#ifdef DEBUG
    digitalWrite(DEBUG_LED, millis() < deadline ? HIGH : LOW);
#endif
    yield();
    if(millis() < deadline) delay(deadline - millis());
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

bool draw_menu_frame(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(0 + x, 20, "MENU: (W)");
  return false;
}

bool draw_calibration_frame(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(12 + x, 16, "Current: ");
  display->drawString(12 + x, 28, "High: ");
  display->drawString(12 + x, 40, "Low: ");
  display->drawString(x, 52, "S4: RST S3: BCK S2: Ok");
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128 + x, 16, valStrings.current);
  display->drawString(128 + x, 28, valStrings.hi);
  display->drawString(128 + x, 40, valStrings.lo);
  display->drawString(128 + x, 52, analogVal.value > ((analogVal.hi + analogVal.lo) / 2) ? "X" : " ");
  return false;
}

bool msOverlay(SSD1306 *display, SSD1306UiState* state) {
  //display->drawXbm(32, 0, 8, 8, wifiActive);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(0, 0, valStrings.current); //This should be the current time
  //display->setTextAlignment(TEXT_ALIGN_RIGHT);
  //display->drawString(128, 0, String(millis()));
  return true;
}

float get_consumption_from_time_since_last_pulse(long timeSinceLastPulse) {
  long time = millis();
  long dsecs = (time - timeSinceLastPulse) / 100;
  if(dsecs < 4) { //4 dsecs = 0.4 sec: the highest supported interval for this sketch.
    time_since_last_pulse = time;
    return 0.0F;
  }
  float result = (WH_PER_PULSE / (dsecs / 36000.0F));
  return result;
}

