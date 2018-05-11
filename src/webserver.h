#ifndef WEBSERVER_H
#define WEBSERVER_H

typedef void (*onwebtype)();
typedef void (*onargtype)(char * value);

#define MAXONS  10

class ONWEB {
  private:
    struct {
      String page;
      onwebtype func;
    } webs[MAXONS];
    int count = 0;
  public:
    void add(const char * page, onwebtype func);
    bool lookup(String uri);
};

class ONARG {
  private:
    int count = 0;
    struct {
      const char * arg;
      onargtype func;
    } args[MAXONS];
  public:
    bool lookup (String arg, char * data);
    void add(const char * arg, onargtype func);
};

class WEBSERVER {
  private:
    static void handlenotfound();
  public:
  WEBSERVER(int port);
  void begin();
  void on(const char * page, onwebtype func);
  void onarg(const char * topic, onargtype func);
  void ifarg(const char * topic, onargtype func);
  void send(int id, char * type, char * data);
  void loop();
};

#endif

