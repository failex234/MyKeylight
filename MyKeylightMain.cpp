//
// Created by Felix Naumann on 14.09.21.
//
#include <iostream>

extern int cli_main(int argc, char* argv[]);
extern int gui_main(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return cli_main(argc, argv);
    }

    if (!strcmp(argv[1], "gui")) {
        return gui_main(argc, argv);
    }

    return cli_main(argc, argv);
}
