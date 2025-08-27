#include "../includes/irc.hpp"
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    Server server(argv[1], argv[2]);
    server.start();

    return 0;
}
