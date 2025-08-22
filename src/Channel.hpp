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
    public:
        // static int userCount;
        Channel(std::string pass);
        std::string getPass();
        void addUser(User user);
        bool isFound(User user);
        bool validatePass(std::string pass);
};

bool checkChars(std::string pass,int size);
bool validChar(char c);
        bool validatePass(std::string pass);;
#endif