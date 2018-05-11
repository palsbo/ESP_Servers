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

void ONWEB::add(const char * page, onwebtype func) {
  for (int i = 0; i < count; i++) {
    if (webs[i].page == page) {
      webs[i].func = func;
      return;
    }
  }
  if (count == MAXONS) return;
  webs[count].page = page;
  webs[count].func = func;
  count++;
}

bool ONWEB::lookup(String uri) {
  for (int i = 0; i < count; i++) {
    if (uri == webs[i].page) {
      webs[i].func();
      return true;
    }
  }
  return false;
}

ONWEB onweb;

bool ONARG::lookup (String arg, char * data) {
  for (int i = 0; i < count; i++) {
    if (arg == args[i].arg) {
      args[i].func(data);
      return true;
    }
  }
  return false;
}

void ONARG::add(const char * arg, onargtype func) {
  for (int i = 0; i < count; i++) {
    if (args[i].arg == arg) {
      args[i].func = func;
      return;
    }
  }
  if (count == MAXONS) return;
  args[count].arg = arg;
  args[count].func = func;
  count++;
}

ONARG onargs;

void WEBSERVER::handlenotfound() {
  for (uint8_t i = 0; i < wserver->args(); i++) {
    String val = wserver->arg(i);
    if (onargs.lookup(wserver->argName(i), &val[0])) {
      break;
    }
  }
  if (onweb.lookup(wserver->uri())) return;
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
  onweb.add(page, func);
}

void WEBSERVER::onarg(const char * arg, onargtype func) {
  onargs.add(arg, func);
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

