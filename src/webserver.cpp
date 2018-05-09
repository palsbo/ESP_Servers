#ifdef ESP8266
#include <ESP8266WebServer.h>
#else
#include <ESP32WebServer.h>
#endif
#include "webserver.h"

#define MAXONS  10

#ifdef ESP8266
ESP8266WebServer * wserver;
#else
ESP32WebServer * wserver;
#endif
int onwebcount = 0;
int onargcount = 0;

struct onwebstruct {
  String page;
  onwebtype func;
};

struct onargstruct {
  const char * arg;
  onargtype func;
};

onwebstruct onwebs[MAXONS];
onargstruct onargs[MAXONS];

bool weblookup(String uri) {
  for (int i = 0; i < onwebcount; i++) {
    if (uri == onwebs[i].page) {
      onwebs[i].func();
      return true;
    }
  }
  return false;
}

bool arglookup (String arg, char * data) {
  for (int i = 0; i < onargcount; i++) {
    if (arg == onargs[i].arg) {
      onargs[i].func(data);
      return true;
    }
  }
  return false;
}

void WEBSERVER::handlenotfound() {
  for (uint8_t i = 0; i < wserver->args(); i++) {
    String val = wserver->arg(i);
    if (arglookup(wserver->argName(i), &val[0])) {
      break;
    }
  }
  if (weblookup(wserver->uri())) return;
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += wserver->uri();
  message += "\nMethod: ";
  message += (wserver->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += wserver->args();
  message += "\n";
  for (uint8_t i = 0; i < wserver->args(); i++) {
    message += " " + wserver->argName(i) + ": " + wserver->arg(i) + "\n";
  }
  wserver->send(404, "text/plain", message);
}

WEBSERVER::WEBSERVER(int port) {
#ifdef ESP8266
  wserver = new ESP8266WebServer(port);
#else
  wserver = new ESP32WebServer(port);
#endif
}

void WEBSERVER::begin() {
  wserver->begin();
  wserver->onNotFound(handlenotfound);
}

void WEBSERVER::on(const char * page, onwebtype func) {
  for (int i = 0; i < onwebcount; i++) {
    if (onwebs[i].page == page) {
      onwebs[i].func = func;
      return;
    }
  }
  if (onwebcount == MAXONS) return;
  onwebs[onwebcount].page = page;
  onwebs[onwebcount].func = func;
  onwebcount++;
}

void WEBSERVER::onarg(const char * arg, onargtype func) {
  for (int i = 0; i < onargcount; i++) {
    if (onargs[i].arg == arg) {
      onargs[i].func = func;
      return;
    }
  }
  if (onargcount == MAXONS) return;
  onargs[onargcount].arg = arg;
  onargs[onargcount].func = func;
  onargcount++;
}
void WEBSERVER::ifarg(const char * topic, onargtype func) {
  String val = (wserver->arg(topic));
  if (val != "") func(&val[0]);
};

void WEBSERVER::send(int id, char * type, char * data) {
 wserver->send(id, type, data);
}

void WEBSERVER::loop() {
  wserver->handleClient();
}

