#include "WiFiConnector.h"

#include <cstring>

void WiFiConnectorClass::setName(const char* ssid) {
    memcpy(_ssid, ssid, strlen(ssid));
}

void WiFiConnectorClass::setName(const String& ssid) {
    setName(ssid.c_str());
}

void WiFiConnectorClass::setPass(const char* pass) {
    memcpy(_pass, pass, strlen(pass));
}

void WiFiConnectorClass::setPass(const String& pass) {
    setPass(pass.c_str());
}

void WiFiConnectorClass::onConnect(WiFiConnectorCallback cb) {
    _conn_cb = cb;
}

void WiFiConnectorClass::onError(WiFiConnectorCallback cb) {
    _err_cb = cb;
}

void WiFiConnectorClass::connect(const char* ssid, const char* pass) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(_ssid, _pass);
    WiFi.begin(ssid, pass);
}

void WiFiConnectorClass::connect(const String& ssid, const String& pass) {
    connect(ssid.c_str(), pass.c_str());
}

void WiFiConnectorClass::begin(const char* ssid, const char* pass) {
    setName(ssid);
    setPass(pass);

    WiFi.onEvent([](WiFiEvent_t event) {
        WiFi.enableAP(false);
        if (WiFiConnector._conn_cb) WiFiConnector._conn_cb();
    }, WIFI_EVENT_STAMODE_GOT_IP);

    WiFi.onEvent([](WiFiEvent_t event) {
        WiFi.enableSTA(false);
        if (WiFiConnector._err_cb) WiFiConnector._err_cb();
    }, WIFI_EVENT_STAMODE_DISCONNECTED);
}

void WiFiConnectorClass::begin(const String& ssid, const String& pass) {
    begin(ssid.c_str(), pass.c_str());
}

WiFiConnectorClass WiFiConnector;