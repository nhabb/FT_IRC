#include "../includes/irc.hpp"

User::User(std::string name, std::string nick)
{
    this->name = name;
    this->nick = nick;
    this->isAdmin = false;
    std::cout<<"User " << name <<" created"<<std::endl;
}

std::string User::getNick()
{
    return nick;
}

std::string User::getName()
{
    return name;
}

bool User::isadmin()
{
    return this->isAdmin;
}

void User::setisadmin(bool isadmin)
{
    this->isAdmin = isadmin;
}

void User::setnick(std::string nick)
{
    this->nick = nick;
}

void User::setname(std:: string name)
{
    this->name = name;
}
