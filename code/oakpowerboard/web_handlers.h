void ws_b1() {
  b1_pressed = true;
  server.send ( 200, "application/json", json_ok );
}

void ws_b2() {
  b2_pressed = true;
  server.send ( 200, "application/json", json_ok );
}

void ws_b3() {
  b3_pressed = true;
  server.send ( 200, "application/json", json_ok );
}

void ws_b4() {
  b4_pressed = true;
  server.send ( 200, "application/json", json_ok );
}

void ws_status() {
  char buffer[26];
  snprintf(buffer, 26, "{\"total\": \"%s\"}", consumption_total_str);
  server.send ( 200, "application/json", buffer );
}

void ws_total_kwh() {
  char buffer[26];
  snprintf(buffer, 26, "{\"total\": \"%s\"}", consumption_total_str);
  server.send ( 200, "application/json", buffer );
}

void handleRoot() {
  server.send ( 200, "text/html", bootstrapcdn );
}
