#pragma once
#include <Arduino.h>

#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include <WiFi.h>
#endif

class WiFiConnectorClass {
    typedef std::function<void()> WiFiConnectorCallback;

    public:
        // установить имя AP
        void setName(const char* ssid);
        void setName(const String& ssid);

        // установить пароль AP
        void setPass(const char* pass);
        void setPass(const String& pass);

        // подключить обработчик успешного подключения
        void onConnect(WiFiConnectorCallback cb);

        // подключить обработчик ошибки подключения, вызовется после старта AP
        void onError(WiFiConnectorCallback cb);

        // вызвать в setup один раз
        void begin(const char* ssid="", const char* pass="");
        void begin(const String& ssid, const String& pass);

        // подключиться. Вернёт false если ssid не задан, будет запущена AP
        void connect(const char* ssid, const char* pass="");
        void connect(const String& ssid, const String& pass="");
    private:
        char _ssid[32];
        char _pass[64];

        WiFiConnectorCallback _conn_cb = nullptr;
        WiFiConnectorCallback _err_cb = nullptr;
};

extern WiFiConnectorClass WiFiConnector;