#ifndef HTTP_server_h
#define HTTP_server_h
#include <arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

extern ESP8266WebServer server;
extern ESP8266HTTPUpdateServer httpUpdater;

extern bool isDebug;


typedef enum {
   SERVER_STARTED,
   SERVER_STOPED,
   CMD_RELE
} HTTP_EVENT;


class HttpServer {
public:
    bool start(WiFiMode mode);
    bool stop();

    void loop();

    void addListener(void (*listener)(HTTP_EVENT));
    void fireEvent(HTTP_EVENT event);

    void httpHandleSwitchRele();
    void httpHandleSetTime();
    void httpHandleWiFIConfig();
    void httpHandleGetMetric();
};

void HTTP_begin(void);
void HTTP_loop(void);
void HTTP_handleRoot(void);

#endif
