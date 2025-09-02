#include "../includes/irc.hpp"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    if (!checkPort(argv[1]))
    {
        std::cout<<"Invalid port"<<std::endl;
        return 1;
    }
    else if (!validatePass(argv[2]))
    {
        std::cout<<"Invalid pass"<<std::endl;
        return 1;
    }
    Server server(argv[1], argv[2]);
    server.start();
    return 0;
}