#include "Channel.hpp"


Channel::Channel(std::string pass)
{
    this->userCount = 0;
    this->pass = pass;
}

std::string Channel::getPass()
{
    return this->pass;
}

void Channel::addUser(User user)
{
    userCount++;
    this->users.push_back(user);
    std::cout<<"Added " << user.getNick()<<std::endl;
}
