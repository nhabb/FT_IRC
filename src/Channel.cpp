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

std::string Channel::getName()
{
    return this->name;
}

void Channel::setName(std::string name)
{
    this->name = name;
}

void Channel::addUser(User user)
{
    userCount++;
    if (!isFound(user))
    {
        this->users.push_back(user);
        std::cout<<"Added " << user.getNick()<<std::endl;
    }
    else
    {
        std::cout<<"User "<<user.getNick()<<" already in channel"<<std::endl;
    }
}

bool Channel::isFound(User user)
{
    for (int i = 0 ; i < (int)users.size(); i++)
    {
        if (users[i].getName() == user.getName() && users[i].getNick() == user.getNick())
            return true;
    }
    return false;
}
