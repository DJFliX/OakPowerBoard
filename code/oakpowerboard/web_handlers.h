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

void handleRoot() {
  server.send ( 200, "text/html", bootstrapcdn );
}
