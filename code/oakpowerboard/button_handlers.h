//For below handlers, ALWAYS return false. If you don't, the handler will be called endlessly.

bool handle_b1() {
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
  return false; //always return false to indicate that we handled the button press.
}

bool handle_b2() {
  ui.nextFrame();
  return false; //always return false to indicate that we handled the button press.
}

bool handle_b3() {
  ui.previousFrame();
  return false; //always return false to indicate that we handled the button press.
}

bool handle_b4() {
  switch(current_state) {
    case CALIBRATE:
      analogVal.hi = 0;
      dtostrf(analogVal.hi, 4, 0, valStrings.hi);
      analogVal.lo = 1024;
      dtostrf(analogVal.lo, 4, 0, valStrings.lo);
      break;
  }
  return false; //always return false to indicate that we handled the button press.
}
