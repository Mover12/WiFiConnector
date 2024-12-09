#include "WiFiConnector.h"

#include <cstring>

void WiFiConnectorClass::setName(const char* APssid) {
    memcpy(_APssid, APssid, strlen(APssid));
}

void WiFiConnectorClass::setPass(const char* APpass) {
    memcpy(_APpass, APpass, strlen(APpass));
}

void WiFiConnectorClass::onConnect(ConnectorCallback cb) {
    _conn_cb = cb;
}

void WiFiConnectorClass::onError(ConnectorCallback cb) {
    _err_cb = cb;
}

void WiFiConnectorClass::connect(const char* ssid, const char* pass) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(_APssid, _APpass);
    WiFi.begin(ssid, pass);
}

void WiFiConnectorClass::begin(const char* APssid, const char* APpass) {
    setName(APssid);
    setPass(APpass);

    WiFi.onEvent([](WiFiEvent_t event) {
        WiFi.enableAP(false);
        if (WiFiConnector._conn_cb) WiFiConnector._conn_cb();
    }, WIFI_EVENT_STAMODE_GOT_IP);

    WiFi.onEvent([](WiFiEvent_t event) {
        WiFi.enableSTA(false);
        if (WiFiConnector._err_cb) WiFiConnector._err_cb();
    }, WIFI_EVENT_STAMODE_DISCONNECTED);
}

WiFiConnectorClass WiFiConnector;