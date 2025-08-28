#ifndef IRC_HPP
#define IRC_HPP

// #include "../src/Channel.hpp"
// #include "../src/User.hpp"
// #include "../src/Server.hpp"
// #include <iostream>
// #include <cstring>
// #include <cstdlib>
#include "Channel.hpp"
#include "User.hpp"
#include "Server.hpp"
bool checkChars(char c);
bool validatePass(std::string pass);
bool checkPort(char *port);

#endif
