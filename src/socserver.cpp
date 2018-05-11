
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "socserver.h"

WebSocketsServer * sserver;

void WSSONS::add(String id, streamtype func) {
  for (int i = 0; i < count; i++) {
    if (ons[i].id == id) {
      ons[i].func = func;
      return;
    }
  }
  if (count >= MAXONS) return;
  ons[count].id = id;
  ons[count].func = func;
  count++;
}
void WSSONS::lookup (String id, uint8_t num, char * data) {
  for (int i = 0; i < count; i++) {
    if (id == ons[i].id) {
      ons[i].func(num, data);
      return;
    }
  }
}

WSSONS wssons;

static void ondata(uint8_t num, char * data) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(data);
  for (auto field : root) {
    String value = root[field.key];
    wssons.lookup(field.key, num, (char*)value.c_str());
  }
};

void decode(char * json) { }

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  String data = String((char*)payload);
  char s1[length + 1];
  strncpy_P(s1, (char*)payload, length);
  s1[length] = 0;
  switch (type) {
    case WStype_DISCONNECTED:
      wssons.lookup("disconnect", num, &s1[0]);
      break;
    case WStype_CONNECTED: {
        Serial.println("Connecting");
        wssons.lookup("connect", num, &s1[0]);
      }
      break;
    case WStype_TEXT:
      if (data == "__ping__") {
        sserver->sendTXT(num, "__pong__");
      } else {
        wssons.lookup("data", num, &s1[0]);
      }
      break;
    case WStype_BIN:
      break;
    default:
#ifdef DEBUG
      Serial.printf("Invalid WStype [%d]\r\n", type);
#endif
      break;
  }
}

SOCSERVER::SOCSERVER(uint8_t port) {
  sserver = new WebSocketsServer(port);
  on("connect", [](uint8_t num, char * data) { });
  on("disconnect", [](uint8_t num, char * data) { });
  on("data", ondata);
}

IPAddress SOCSERVER::ip(uint8_t num) {
  return sserver->remoteIP(num);
}

void SOCSERVER::begin() {
  sserver->begin();
  sserver->onEvent(webSocketEvent);
};

void SOCSERVER::on(char * id, streamtype func) {
  wssons.add(id, func);
}

void SOCSERVER::broadcast(String data) {
  sserver->broadcastTXT(data);
}

void SOCSERVER::send(uint8_t num, String data) {
  sserver->sendTXT(num, data);
}

void SOCSERVER::loop() {
  sserver->loop();
}

