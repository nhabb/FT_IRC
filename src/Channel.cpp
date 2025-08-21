#include "Channel.hpp"


Channel::Channel(std::string pass)
{
    // this->userCount = 0;
    this->pass = pass;
}

std::string Channel::getPass()
{
    return this->pass;
}

void Channel::addUser(User user)
{
    // userCount++;
    this->users.push_back(user);
    std::cout<<"Added " << user.getNick()<<std::endl;
}

//pass rules
/*
    * min len 10 characters
    * alphanumeric
    * _ - $ @ are the only allowed non alphanumeric characters
*/


bool checkChars(char c) 
{
    return isalnum(c) || validChar(c);
}

bool validatePass(std::string pass) 
{
    int size = (int)pass.size();
    if (size < 10)
        return false;

    int alpha = 0;
    int digit = 0;

    for (int i = 0; i < size; ++i)
    {
        if (isalpha(pass[i]))
            ++alpha;
        if (isdigit(pass[i]))
            ++digit;
        if (!checkChars(pass[i]))
            return false;
    }
    if (alpha == 0 || digit == 0)
        return false;
    return true;
}

bool validChar(char c)
{
    return c == '_' || c == '-' || c == '$' || c == '@';
}