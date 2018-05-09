#ifndef MYAPP_H
#define MYAPP_H

#include "webserver.h"
#include "socserver.h"
#include "pages.h"

WEBSERVER webserver(80);
SOCSERVER socserver(81);

char * url = "http://streammp3.retro-radio.dk/retro-mp3?type=.mp3";
int volume = 30;

class MYAPP {
  private:
  public:
    void begin() {
      webserver.on("/", []() {
        webserver.send(200, "text/html", (char*)index_html);
      });
      webserver.on("/wssocket.js", []() {
        webserver.send(200, "application/javascript", (char*)wssocket_js);
      });
      webserver.begin();
      socserver.on("connect", [](uint8_t num, char * data) {
        char buf[100];
        sprintf(buf, "{\"url\":\"%s\", \"gain\":%d}\0", url, volume);
        socserver.send(num, buf);
      });
      socserver.on("url", [](uint8_t num, char * value) {
        Serial.printf("onsock url: '%s'\n", value);
        url = value;
      });
      socserver.begin();
    }
    void loop() {
      socserver.loop();
      webserver.loop();
    }
} myApp;

#endif
