#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <FS.h>
#else
#include <ESP32WebServer.h>
#include <SPIFFS.h>
#include <FS.h>
#endif
#include "webserver.h"

#define MAXONS  10

#ifdef ESP8266
#define WEBSERVEROBJ ESP8266WebServer
#else
#define WEBSERVEROBJ ESP32WebServer
#endif

WEBSERVEROBJ * wserver;

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

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
}

String WEBSERVER::getContentType(String filename) {
  if (wserver->hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool WEBSERVER::handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    if (!file) {
      Serial.println("Failed to open file for reading");
      return false;
    }
    wserver->streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void WEBSERVER::handlenotfound() {
  for (uint8_t i = 0; i < wserver->args(); i++) {
    String val = wserver->arg(i);
    if (onargs.lookup(wserver->argName(i), &val[0])) {
      break;
    }
  }
  if (onweb.lookup(wserver->uri())) return;
  if (handleFileRead(wserver->uri())) return;
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
  wserver = new WEBSERVEROBJ(port);
}

void WEBSERVER::begin() {
  if (!SPIFFS.begin()) {
    Serial.println("Card Mount Failed");
  }
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

