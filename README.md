# ESP Servers

General server libraries for Webserver and WebSocket.

##	Webserver library

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

The websocket expects to receive a JSON string. Each key in the JSON array will generate an 'on' event.

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

