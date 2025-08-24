#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "User.hpp"
#include <cstring>

class Channel
{
    private:
        std::vector<User> users;
        std::string pass;
        std::string name;
    public:
        int userCount;
        Channel(std::string pass);
        std::string getPass();
        void addUser(User user);
        bool isFound(User user);
        std::string getName();
        void setName(std::string name);
};


#endif