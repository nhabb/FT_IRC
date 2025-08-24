#include "Server.hpp"
#include <iostream>
#include <sstream>

Server::Server(std::string port, std::string pass) {
    this->pass = pass;
    this->port = port;
    std::cout << "Server created. Welcome to IRCSSSSSS" << std::endl;
}

bool Server::channelFound(Channel channel) {
    for (size_t i = 0; i < channels.size(); i++) {
        if (channel.getName() == channels[i].getName())
            return true;
    }
    return false;
}

void Server::addChannel(Channel channel) {
    if (!channelFound(channel)) {
        channels.push_back(channel);
        std::cout << "Added channel " << channel.getName() << std::endl;
    } else {
        std::cout << "Channel " << channel.getName() << " already exists" << std::endl;
    }
}

void Server::createChannel() {
}

Channel* Server::getChannelByName(const std::string &name) {
    for (size_t i = 0; i < channels.size(); i++) {
        if (channels[i].getName() == name)
            return &channels[i];
    }
    return NULL;
}

void Server::broadcastMode(Channel *channel, const std::string &setter, const std::string &modeString) {
    std::string message = ":" + setter + " MODE " + channel->getName() + " " + modeString + "\r\n";
    std::cout << "[BROADCAST] " << message;
}

void Server::handleMode(User &sender, std::vector<std::string> &args) {
    if (args.size() < 2) {
        std::cout << "Error: MODE requires <channel> <modes>" << std::endl;
        return;
    }

    std::string channelName = args[0];
    std::string modeString  = args[1];

    Channel *channel = getChannelByName(channelName);
    if (!channel) {
        std::cout << "Error: No such channel " << channelName << std::endl;
        return;
    }

    if (!channel->isOperator(sender.getNick())) {
        std::cout << "Error: User " << sender.getNick()
                  << " is not a channel operator" << std::endl;
        return;
    }

    bool adding = true;
    size_t argIndex = 2;

    for (size_t i = 0; i < modeString.size(); i++) {
        char mode = modeString[i];
        if (mode == '+') { adding = true; continue; }
        if (mode == '-') { adding = false; continue; }

        switch (mode) {
            case 'i': channel->setInviteOnly(adding); break;
            case 't': channel->setTopicRestricted(adding); break;
            case 'k':
                if (adding) {
                    if (args.size() <= argIndex) {
                        std::cout << "Error: MODE +k requires a key" << std::endl;
                        return;
                    }
                    channel->setKey(args[argIndex++]);
                } else {
                    channel->setKey("");
                }
                break;
            case 'l':
                if (adding) {
                    if (args.size() <= argIndex) {
                        std::cout << "Error: MODE +l requires a limit" << std::endl;
                        return;
                    }
                    size_t limit;
                    std::istringstream(args[argIndex++]) >> limit;
                    channel->setUserLimit(limit);
                } else {
                    channel->setUserLimit(0);
                }
                break;
            case 'o':
                if (args.size() <= argIndex) {
                    std::cout << "Error: MODE +o/-o requires a nickname" << std::endl;
                    return;
                }
                if (adding)
                    channel->addOperator(args[argIndex++]);
                else
                    channel->removeOperator(args[argIndex++]);
                break;
            default:
                std::cout << "Error: Unknown mode " << mode << std::endl;
                return;
        }
    }

    broadcastMode(channel, sender.getNick(), modeString);
}

void Server::handleCommand(User &user, const std::string &cmd, std::vector<std::string> &args) {
    if (cmd == "MODE") {
        handleMode(user, args);
    }
}
