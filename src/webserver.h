#ifndef WEBSERVER_H
#define WEBSERVER_H

typedef void (*onwebtype)();
typedef void (*onargtype)(char * value);

class WEBSERVER {
  private:
    static void handlenotfound();
  public:
  WEBSERVER(int port);
  void begin();
  void on(const char * page, onwebtype func);
  void onarg(const char * topic, onargtype func);
  void ifarg(const char * topic, onargtype cb);
  void send(int id, char * type, char * data);
  void loop();
};

#endif

