#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/irc.hpp"

class Server
{
    private:
        std::vector<Channel> channels;
        std::string pass;
        char *port;
    public:
        Server(char *port,std::string pass);
        void addChannel(Channel channel);
        bool channelFound(Channel channel);
        void createChannel(std::string name,std::string pass);
};

bool validChar(char c);
bool checkPort(char *port);
bool validatePass(std::string pass);

#endif
