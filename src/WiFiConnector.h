#pragma once
#include <Arduino.h>

#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include <WiFi.h>
#endif

class WiFiConnectorClass {
    typedef std::function<void()> ConnectorCallback;

    public:
        // установить имя AP
        void setName(const char* APssid);

        // установить пароль AP
        void setPass(const char* APpass);

        // подключить обработчик успешного подключения
        void onConnect(ConnectorCallback cb);

        // подключить обработчик ошибки подключения, вызовется после старта AP
        void onError(ConnectorCallback cb);

        // подключиться. Вернёт false если ssid не задан, будет запущена AP
        void connect(const char* ssid, const char* pass="");

        // вызвать в setup один раз
        void begin(const char* APssid="", const char* APpass="");
    private:
        char _APssid[32];
        char _APpass[64];

        ConnectorCallback _conn_cb = nullptr;
        ConnectorCallback _err_cb = nullptr;
};

extern WiFiConnectorClass WiFiConnector;