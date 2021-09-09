#ifndef MYKEYLIGHT_KEYLIGHT_H
#define MYKEYLIGHT_KEYLIGHT_H

#include <string>
#include <cmath>
#include <curl/curl.h>
#include <cstring>
#include <iostream>

class Keylight {
public:
    std::string getName();
    std::string getAdress();
    uint8_t getBrightness();
    uint16_t getTemperature();
    bool isTurnedOn();

    //DONT USE! only public for curl callback
    uint8_t brightness;
    uint16_t temperature;
    bool turnedon;

    void setBrightness(uint8_t);
    void increaseBrightness();
    void decreaseBrightness();
    void setTemperature(int);
    void increaseTemperature();
    void decreaseTemperature();
    void toggleStatus();

    void refreshInfo();

    void initCurl();
    void destroyCurl();

    Keylight(std::string name, std::string address) {
        this->name = name;
        this->address = address;

        this->brightness = 0;
        this->temperature = 0;
        this->turnedon = false;

        //Build the request url and convert it to a c string for curl
        std::string requesturltemp = "http://" + this->address + ":9123/elgato/lights";
        size_t urllen = requesturltemp.size();

        requesturl = (char*) malloc(urllen + 1);
        memcpy(requesturl, requesturltemp.c_str(), urllen);

        requesturl[urllen] = '\0';
    }

private:
    std::string name;
    std::string address;
    char *requesturl;

    CURL *curl;
    CURLcode res;

    uint16_t convertFromKelvin(int);
    int convertToKelvin(uint16_t);

};

#endif //MYKEYLIGHT_KEYLIGHT_H
