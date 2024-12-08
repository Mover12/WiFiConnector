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
        // имя AP, пароля нет, таймаут 60 секунд, отключать AP при успешном подключении к STA (иначе AP_STA)
        WiFiConnectorClass(const String& APname = "ESP AP", const String& APpass = "", uint16_t timeout = 60, bool closeAP = true);

        // установить имя AP
        void setName(const String& name);

        // установить пароль AP
        void setPass(const String& pass);

        // установить таймаут в секундах
        void setTimeout(uint16_t timeout);

        // автоматически отключать AP при подключении к STA (умолч. вкл)
        void closeAP(bool close);

        // подключить обработчик успешного подключения
        void onConnect(ConnectorCallback cb);

        // подключить обработчик ошибки подключения, вызовется после старта AP
        void onError(ConnectorCallback cb);

        // подключиться. Вернёт false если ssid не задан, будет запущена AP
        bool connect(const String& ssid, const String& pass = emptyString);

        // вызывать в loop. Вернёт true при смене состояния
        bool tick();

        // состояние подключения. true - подключен, false - запущена АР
        bool connected();

        // в процессе подключения
        bool connecting();

        // подключить обработчик таймаута подключения
        void onTimeout(ConnectorCallback cb) __attribute__((deprecated));

    private:
        String _APname, _APpass;
        uint32_t _tmr = 0, _tout;
        bool _closeAP = false;
        bool _tryConnect = false;

        ConnectorCallback _conn_cb = nullptr;
        ConnectorCallback _err_cb = nullptr;

        void _startAP();
};

extern WiFiConnectorClass WiFiConnector;