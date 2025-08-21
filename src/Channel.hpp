#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "User.hpp"

class Channel
{
    private:
        std::vector<User> users;
        std::string pass;
    public:
        static int userCount;
        Channel(std::string pass);
        std::string getPass();
        void addUser(User user);
};

#endif