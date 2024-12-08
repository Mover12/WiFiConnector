#include "WiFiConnector.h"

WiFiConnectorClass::WiFiConnectorClass(const String& APname, const String& APpass, uint16_t timeout, bool closeAP) :
    _APname(APname),
    _APpass(APpass),
    _tout(timeout * 1000ul),
    _closeAP(closeAP)
{

}

void WiFiConnectorClass::setName(const String& name) {
    _APname = name;
}

void WiFiConnectorClass::setPass(const String& pass) {
    _APpass = pass;
}

void WiFiConnectorClass::setTimeout(uint16_t timeout) {
    _tout = timeout * 1000ul;
}

void WiFiConnectorClass::closeAP(bool close) {
    _closeAP = close;
}

void WiFiConnectorClass::onConnect(ConnectorCallback cb) {
    _conn_cb = cb;
}

void WiFiConnectorClass::onError(ConnectorCallback cb) {
    _err_cb = cb;
}

bool WiFiConnectorClass::connect(const String& ssid, const String& pass) {
    if (ssid.length()) {
        _tryConnect = true;
        WiFi.mode(WIFI_AP_STA);
        _startAP();
        WiFi.begin(ssid.c_str(), pass.c_str());
        _tmr = millis();
        return 1;
    } else {
        _tryConnect = false;
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        _startAP();
        if (_err_cb) _err_cb();
    }
    return 0;
}

bool WiFiConnectorClass::tick() {
    if (_tryConnect) {
        if (WiFi.status() == WL_CONNECTED) {
            _tryConnect = false;
            if (_conn_cb) _conn_cb();
            if (_closeAP) WiFi.softAPdisconnect(true);
            return 1;
        } else if (millis() - _tmr >= _tout) {
            _tryConnect = false;
            WiFi.disconnect();
            WiFi.mode(WIFI_AP);
            _startAP();
            if (_err_cb) _err_cb();
            return 1;
        }
    }
    return 0;
}

bool WiFiConnectorClass::connected() {
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiConnectorClass::connecting() {
    return _tryConnect;
}

// void WiFiConnectorClass::onTimeout(ConnectorCallback cb) __attribute__((deprecated)) {
//     _err_cb = cb;
// }

void WiFiConnectorClass::_startAP() {
    WiFi.softAP(_APname.c_str(), _APpass.c_str());
}