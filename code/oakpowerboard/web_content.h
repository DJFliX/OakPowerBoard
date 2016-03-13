#ifndef _WEB_CONTENT_H_
#define _WEB_CONTENT_H_

const char json_ok[] PROGMEM = "{ \"status\": \"OK\" }";

const char bootstrapcdn[] PROGMEM = "<html>\
  <head>\
    <title>Oak PowerBoard WebConfig</title>\
  </head>\
  <body>\
    <h1>Bootstrapping...</h1>\
    <div id=\"bc\"></div>\
    <script src=\"//rawgit.com/DJFliX/OakPowerBoard/add-webserver/webapp/main.js\"></script>\
  </body>\
</html>";
#endif
