#ifndef SOCSERVER_H
#define SOCSERVER_H


typedef void (*streamtype)(uint8_t num, char * data);

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

