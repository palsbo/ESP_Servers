# ESP Servers

General server libraries for Webserver and WebSocket.

##	Webserver library

The library will first look for arguments and return values with handlers defined in 'onarg'.
Nex it will look for uri pages and transmit defined pages with handlers defined in 'on'.
If a handler is not defined for an uri pages, the librafy will look in the SPIFFS for the file.

Uses the followng libraries for ESP8266:

	ESP8266WebServer.h     included as part of ESP8266 hardware profile

Uses the followng libraries for ESP32:

	ESP32WebServer.h       https://github.com/Pedroalbuquerque/ESP32WebServer


###	Include

	#include <webserver.h>

###	Define object

	WEBSERVER webserver

###	Methods

	void begin();
	void on(const char * page, onwebtype func);       //	func = [] () {};
	void onarg(const char * topic, onargtype func);   //	func = [] (char * value) {};
	void ifarg(const char * topic, onargtype func);   //  func = [] (char * value) {};
	void send(int id, char * type, char * data);
	void loop();

##	WebSocket server library.

Uses the followng libraries:

The websocket expects to receive a JSON string. Each key in the JSON array will generate an 'on' event.

	ArduinoJson.h          https://github.com/bblanchon/ArduinoJson/blob/master/ArduinoJson.h
	WebSocketsServer.h     https://github.com/brandenhall/Arduino-Websocket/


###	Include

	#include <socserver.h>

###	Define object

	SOCSERVER socserver(uint_8 port);

###	Methods

	IPAddress ip(uint8_t num);
	void begin();
	void on(char * id, streamtype func);           //	func = [] (uint8_t num, char * data) {};
	void broadcast(String data);
	void send(uint8_t num, String data);
	void loop();

