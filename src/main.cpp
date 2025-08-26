#include "../includes/irc.hpp"
#include <iostream>
#include <vector>
#include <string>

void parseCommand(const std::string &line, std::string &cmd, std::vector<std::string> &args);

void runLine(Server &server, User &user, const std::string &line) {
    std::string cmd;
    std::vector<std::string> args;
    parseCommand(line, cmd, args);
    std::cout << "\n[INPUT] " << line << std::endl;
    server.handleCommand(user, cmd, args);
}

int main() {
    Server server("6667", "mypassword");
    User alice("Alice", ""); // nick will be set after NICK
    std::cout << "Server created and User Alice initialized" << std::endl;

    runLine(server, alice, "PASS mypassword");
    runLine(server, alice, "NICK Alice");
    runLine(server, alice, "USER alice 0 * :Alice RealName");
    runLine(server, alice, "JOIN #test");
    runLine(server, alice, "PRIVMSG #test :hello everyone");
    runLine(server, alice, "MODE #test +i");
    runLine(server, alice, "MODE #test +l 2");
    runLine(server, alice, "MODE #test +k secret");

    runLine(server, alice, "MODE #test +z");
    runLine(server, alice, "JOIN");               // missing arg
    runLine(server, alice, "PRIVMSG #test");      // missing msg
    runLine(server, alice, "USER alice");         // missing params
    runLine(server, alice, "NICK");               // missing nick
    runLine(server, alice, "PASS wrongpass");     // wrong password

    return 0;
}
