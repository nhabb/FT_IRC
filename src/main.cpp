#include "Server.hpp"
#include <iostream>
#include <vector>
#include <string>

void runTest(Server &server, User &user, std::vector<std::string> args, const std::string &desc) {
    std::cout << "\n[TEST] " << desc << std::endl;
    server.handleMode(user, args);
}

int main() {
    Server server("6667", "testpass");
    User alice("Alice", "AliceNick");
    alice.setisadmin(true);
    Channel chan("chanpass");
    chan.setName("#test");
    chan.addOperator(alice.getNick());
    server.addChannel(chan);

    User bob("Bob", "BobNick");

    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+i");
        runTest(server, alice, args, "Enable invite-only");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("-i");
        runTest(server, alice, args, "Disable invite-only");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+t");
        runTest(server, alice, args, "Restrict topic to ops");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("-t");
        runTest(server, alice, args, "Unrestrict topic");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+k");
        args.push_back("secret");
        runTest(server, alice, args, "Set channel key");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("-k");
        runTest(server, alice, args, "Remove channel key");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+l");
        args.push_back("3");
        runTest(server, alice, args, "Set user limit");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("-l");
        runTest(server, alice, args, "Remove user limit");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+o");
        args.push_back("BobNick");
        runTest(server, alice, args, "Give Bob operator");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("-o");
        args.push_back("BobNick");
        runTest(server, alice, args, "Remove Bob operator");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+itkl");
        args.push_back("secret");
        args.push_back("3");
        args.push_back("BobNick");
        runTest(server, alice, args, "Multiple modes at once");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+i");
        runTest(server, bob, args, "Non-operator trying MODE");
    }
    {
        std::vector<std::string> args;
        runTest(server, alice, args, "MODE with no args");
    }
    {
        std::vector<std::string> args;
        args.push_back("#ghost");
        args.push_back("+i");
        runTest(server, alice, args, "MODE on non-existent channel");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+k");
        runTest(server, alice, args, "MODE +k without key");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+l");
        runTest(server, alice, args, "MODE +l without limit");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+o");
        runTest(server, alice, args, "MODE +o without nickname");
    }
    {
        std::vector<std::string> args;
        args.push_back("#test");
        args.push_back("+z");
        runTest(server, alice, args, "MODE with unknown flag");
    }

    return 0;
}
