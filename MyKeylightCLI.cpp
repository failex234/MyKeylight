#include <iostream>
#include <vector>
#include "Keylight.h"

extern bool discoverKeylights(std::vector<Keylight>*);

int cli_main(int argc, char* argv[]) {
    std::vector<Keylight> keylights;

    std::cout << "Searching for elgato key lights..." << std::endl;
    bool discoveryresult = discoverKeylights(&keylights);

    if (!discoveryresult) {
        return EXIT_FAILURE;
    } else if (keylights.empty()) {
        std::cerr << "No elgato key lights were found!" << std::endl;
    } else {
        std::cout << "WELCOME TO MYKEYLIGHT" << std::endl;
        std::cout << "==============================" << std::endl;

        start:
        int selection = -1;
        std::cout << "The following keylights were found:" << std::endl;

        for (int i = 0; i < keylights.size(); i++) {
            std::cout << i << ": " << keylights[i].getName() << std::endl;
        }
        std::cout << std::endl << "99: Exit MyKeylight" << std::endl;

        std::cout << "==============================" << std::endl;
        std::cout << "Which keylight do you want to control? ";
        std::cin >> selection;

        if (selection == 99) {
            for (auto keylight : keylights) {
                keylight.destroyCurl();
            }
            return EXIT_SUCCESS;
        }

        if (selection >= keylights.size()) {
            std::cerr << "The selected keylight is invalid" << std::endl;
            goto start;
        }

        Keylight selected = keylights[selection];
        selected.initCurl();

        control:
        selected.refreshInfo();
        selected.initCurl();

        std::cout << "==============================" << std::endl;
        std::cout << "Selected keylight: " << selected.getName() << std::endl;
        std::cout << "==============================" << std::endl;
        std::cout << "Available options:" << std::endl;
        std::cout << "0: Get info" << std::endl;

        std::cout << "1: Set brightness" << std::endl;
        std::cout << "2: Set temperature" << std::endl;

        std::cout << "3: Increase brightness" << std::endl;
        std::cout << "4: Increase temperature" << std::endl;

        std::cout << "5: Decrease brightness" << std::endl;
        std::cout << "6: Decrease temperature" << std::endl;

        std::cout << "7: Toggle on/off" << std::endl;

        std::cout << std::endl << "99: Go back" << std::endl;

        std::cout << "Which option do you want to use? ";
        std::cin >> selection;

        if (selection == 99) {
            selected.destroyCurl();
            goto start;
        }

        if (selection > 7 || selection < 0) {
            std::cerr << "The entered option is not valid!" << std::endl;
            goto control;
        }

        switch(selection) {
            case 0:
                selected.refreshInfo();
                std::cout << "Status of " << selected.getName() << ":" << std::endl;
                std::cout << "Power state: " << (selected.isTurnedOn() ? "on" : "off") << std::endl;
                std::cout << "Current brightness: " << (int) selected.getBrightness() << "%" << std::endl;
                std::cout << "Current temperature: " << (int) convertToKelvin(selected.getTemperature()) << "K" << std::endl;
                break;
            case 1:
                std::cout << "What should be the new brightness? ";
                std::cin >> selection;

                //No value check is needed as the setBrightness method is responsible for this
                selected.setBrightness(selection);
                break;
            case 2:
                std::cout << "What should be the new temperature? ";
                std::cin >> selection;

                //No value check is needed as the setTemperature method is responsible for this
                selected.setTemperature(selection);
                break;
            case 3:
                selected.increaseBrightness();
                break;
            case 4:
                selected.increaseTemperature();
                break;
            case 5:
                selected.decreaseBrightness();
                break;
            case 6:
                selected.decreaseTemperature();
                break;
            case 7:
                selected.toggleStatus();
                break;
        }
        goto control;


    }

    return EXIT_SUCCESS;
}