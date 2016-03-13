bool draw_totalcount_frame(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(0 + x, 20, F("Total (kWh):"));
  display->setFont(Roboto_Thin_Plain_20);
  display->drawString(0 + x, 32, consumption_total_str);
  return false;
}

bool draw_currentconsumption_frame(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(0 + x, 20, F("Current: (W)"));
  display->setFont(Roboto_Thin_Plain_20);
  display->drawString(0 + x, 34, consumption_current_str);
  return false;
}

bool draw_menu_frame(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(0 + x, 20, F("MENU: (W)"));
  display->drawString(0 + x, 32, timestamp_string);
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

bool draw_calibration_frame(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Roboto_Plain_10);
  display->drawString(12 + x, 16, F("Current: "));
  display->drawString(12 + x, 28, F("High: "));
  display->drawString(12 + x, 40, F("Low: "));
  display->drawString(x, 52, F("S4: RST S3: BCK S2: Ok"));
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128 + x, 16, valStrings.current);
  display->drawString(128 + x, 28, valStrings.hi);
  display->drawString(128 + x, 40, valStrings.lo);
  display->drawString(128 + x, 52, analogVal.value > ((analogVal.hi + analogVal.lo) / 2) ? F("X") : F(" "));
  return false;
}
