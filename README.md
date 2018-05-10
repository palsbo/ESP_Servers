# ESP Servers

General server libraries for Webserver and WebSocket.

##	Webserver library

Uses the followng libraries for ESP8266:

	ESP8266WebServer.h     included as part of ESP8266 hardware profile

Uses the followng libraries for ESP32:

	ESP32WebServer.h       https://github.com/chunter1/precipitationSensorESP32/blob/master/ESP32WebServer.h


	ArduinoJson.h          https://github.com/bblanchon/ArduinoJson/blob/master/ArduinoJson.h
	WebSocketsServer.h     https://github.com/brandenhall/Arduino-Websocket/blob/master/WebSocketServer.h


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
	WebSocketsServer.h     https://github.com/brandenhall/Arduino-Websocket/blob/master/WebSocketServer.h


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

