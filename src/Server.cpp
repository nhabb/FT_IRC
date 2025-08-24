#include "Server.hpp"


Server::Server(char *port,std::string pass)
{
    this->pass = pass;
    this->port = port;
    std::cout<<"Sever created. Welcome to IRCSSSSSS"<<std::endl;
}

bool Server::channelFound(Channel channel)
{
    for (int i = 0; i < (int)channels.size(); i++)
    {
        if (channel.getName() == channels[i].getName())
            return true;
    }
    return false;
}

void Server::addChannel(Channel channel)
{
    if (!channelFound(channel))
    {
        channels.push_back(channel);
        std::cout<<"Added channel "<<channel.getName()<<std::endl;
    }
    else
        std::cout<<"Channel "<<channel.getName()<<" already found"<<std::endl;

}

void Server::createChannel()
{
    
}