#ifndef MYAPP_H
#define MYAPP_H

#include "webserver.h"
#include "socserver.h"
#include "pages.h"

WEBSERVER webserver(80);
SOCSERVER socserver(81);

class COLORS {
  private:
    String letter = "RGB";
    char buf[40];
  public:
    uint8_t cols[3];
    char * set(uint8_t idx, uint8_t val) {
      cols[idx] = val;
      sprintf_P(buf, "{\"%c\":\"%d\"}", letter[idx], val);
      return &buf[0];
    }
    char * json() {
      sprintf_P(buf, "{\"R\":%d,\"G\":%d,\"B\":%d}", cols[0], cols[1], cols[2]);
      return &buf[0];
    }
} led;

class MYAPP {
  private:
    static char buf[100];
  public:
    void begin() {
      socserver.on("connect", [](uint8_t num, char * data) {
        Serial.printf("[%d] Connected! %s\n", num, data);
        //  set iopins
        socserver.send(num, led.json());
      });
      socserver.on("R", [](uint8_t num, char * value) {
        char * msg = led.set(0, atoi(value));
        //  set iopin
        socserver.broadcast(msg);
      });
      socserver.on("G", [](uint8_t num, char * value) {
        char * msg = led.set(1, atoi(value));
        //  set iopin
        socserver.broadcast(msg);
      });
      socserver.on("B", [](uint8_t num, char * value) {
        char * msg = led.set(2, atoi(value));
        //  set iopin
        socserver.broadcast(msg);
      });
      socserver.begin();
      webserver.on("/", []() {
        webserver.send(200, "text/html", (char*)index_html);
      });
      webserver.on("/jscolor.min.js", []() {
        webserver.send(200, "application/javascript", (char*)jscolor_js);
      });
      webserver.on("/wssocket.js", []() {
        webserver.send(200, "application/javascript", (char*)wssocket_js);
      });
      webserver.begin();
    };
    void loop() {
      socserver.loop();
      webserver.loop();
    };
} myApp;

#endif
