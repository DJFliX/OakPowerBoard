#ifndef _WEB_CONTENT_H_
#define _WEB_CONTENT_H_

const char json_ok[] PROGMEM = "{ \"status\": \"OK\" }";

const char web_content_uptime[] PROGMEM = "<html>\
  <head>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #09F; font-family: Arial, Helvetica, Sans-Serif; Color: #008; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from OakPowerBoard!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
</html>";

const char bootstrap[] PROGMEM = "<html>\
  <head>\
    <title>Bootstrap</title>\
    <script>\
    if (!localStorage.bootstrap) {\
      localStorage.bootstrap = 'Y29uc29sZS5sb2coJ2hlbGxvIHdvcmxkJyk7';\
      console.log('no bootstrap yet!');\
    } else {\
      console.log('bootstrap present!');\
    }\
      var head = document.getElementsByTagName(\"head\")[0];\
      var js = document.createElement(\"script\");\
      js.src = 'data:text/javascript;base64,' + localStorage.bootstrap;\
      head.appendChild(js);\
    </script>\
  </head>\
  <body>\
    <h1>Bootstrapping...</h1>\
    <div id=\"bc\"></div>\
  </body>\
</html>";

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
