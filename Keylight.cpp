#include "Keylight.h"
#include "thirdparty/json.hpp"
using json = nlohmann::json;

size_t curlCallback(char *contents, size_t size, size_t nmemb, Keylight *keylight) {
    json response = json::parse(contents);

    int turnedon = response["lights"][0]["on"];
    int brightness = response["lights"][0]["brightness"];
    int temperature = response["lights"][0]["temperature"];

    keylight->brightness = brightness;
    keylight->temperature = temperature;
    keylight->turnedon = turnedon;

    return size * nmemb;
}

std::string Keylight::getName() {
    return this->name;
}

std::string Keylight::getAdress() {
    return this->address;
}

uint8_t Keylight::getBrightness() {
    return this->brightness;
}

uint16_t Keylight::getTemperature() {
    return this->temperature;
}


void Keylight::setBrightness(uint8_t brightn) {
    uint8_t newbrightness = brightn;

    if (brightness > 100) {
        newbrightness = 100;
    }

    json body;
    body["numberOfLights"] = 1;
    body["lights"] = {{
                              {"brightness", brightn},
                      }};
    auto json_str = body.dump();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
    res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        this->brightness = brightn;
    }


}

void Keylight::setTemperature(int temp) {
    uint16_t newtemp = convertFromKelvin(temp);

    if (newtemp > 344) {
        newtemp = 344;
    } else if (newtemp < 143) {
        newtemp = 143;
    }

    //Construct json
    json body;
    body["numberOfLights"] = 1;
    body["lights"] = {{
                              {"temperature", newtemp},
                      }};

    auto json_str = body.dump();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
    res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        this->temperature = temp;
    }
}

void Keylight::increaseBrightness() {
    setBrightness(this->brightness + 10);
}

void Keylight::decreaseBrightness() {
    setBrightness(this->brightness - 10);
}

void Keylight::increaseTemperature() {
    setTemperature(this->temperature + 50);
}

void Keylight::decreaseTemperature() {
    setTemperature(this->temperature - 50);
}

bool Keylight::isTurnedOn() {
    return this->turnedon;
}

void Keylight::toggleStatus() {
    //Construct json
    json body;
    body["numberOfLights"] = 1;
    body["lights"] = {{
        {"on", !this->turnedon},
    }};
    
    auto json_str = body.dump();

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
    res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        this->turnedon = !this->turnedon;
    }
}

void Keylight::refreshInfo() {
    //curl get everything
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

    curl_easy_perform(curl);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
}

uint16_t Keylight::convertFromKelvin(int kelvinval) {
    return (uint16_t) round(987007 * pow(kelvinval, -.999));
}

int Keylight::convertToKelvin(uint16_t keylightval) {
    //TODO: perform python like rounding
    return (int) round(1000000 * pow(keylightval, -1));
}

void Keylight::initCurl() {
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, requesturl);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

}

void Keylight::destroyCurl() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}



