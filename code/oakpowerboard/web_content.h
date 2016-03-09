#ifndef _WEB_CONTENT_H_
#define _WEB_CONTENT_H_

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
      localStorage.bootstrap = 'Y29uc29sZS5sb2coJ2hlbGxvIHdvcmxkJyk7'\
    }\
    var script = document.createElement('script');\
    script.src = 'data.text/javascript,' + encodeURI(localStorage.bootstrap);\
    script.onload = function() {\
      console.log('hi');\
    };\
    document.body.appendChild(script);\
    </script>\
  </head>\
  <body>\
    <h1>Bootstrapping...</h1>\
  </body>\
</html>";

#endif
