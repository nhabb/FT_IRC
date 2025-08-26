#include "../includes/irc.hpp"
#include <iostream>
#include <sstream>

Server::Server(std::string port, std::string pass) {
    this->pass = pass;
    this->port = port;
    std::cout << "Server created. Welcome to IRCSSSSSS" << std::endl;
}

bool Server::channelFound(Channel channel) {
    for (size_t i = 0; i < channels.size(); i++) {
        if (channel.getName() == channels[i].getName())
            return true;
    }
    return false;
}

void Server::addChannel(Channel channel) {
    if (!channelFound(channel)) {
        channels.push_back(channel);
        std::cout<<"Added channel "<<channel.getName()<<std::endl;
    }
    else
        std::cout<<"Channel "<<channel.getName()<<" already found"<<std::endl;

}

void Server::createChannel()
{
    
}