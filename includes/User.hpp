#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
class User 
{
    private:
        std::string nick;
        std::string name;
        bool    isAdmin;
    public:
        std::string getNick();
        std::string getName();
        bool isadmin();
        void setisadmin(bool admin);
        void setnick( std::string newNick);
        void setname( std::string newName);
        User(std::string name, std::string nick);
};

#endif
