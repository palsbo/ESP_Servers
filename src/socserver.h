#ifndef SOCSERVER_H
#define SOCSERVER_H


typedef void (*streamtype)(uint8_t num, char * data);

#define MAXONS  15

class WSSONS {
  private:
    struct ON {
      String id;
      streamtype func;
    } ons[MAXONS];
    int count = 0;
  public:
    void add(String id, streamtype func);
    void lookup (String id, uint8_t num, char * data);
};

class SOCSERVER {
  private:
  public:
    SOCSERVER(uint8_t _port);
    IPAddress ip(uint8_t num);
    void begin();
    void on(char * id, streamtype func);
    void broadcast(String data);
    void send(uint8_t num, String data);
    void loop();
};


#endif

