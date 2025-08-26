#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include "Channel.hpp"
#include "User.hpp"

class Server {
private:
    std::string pass;
    std::string port;
    std::vector<Channel> channels;

public:
    Server(std::string port, std::string pass);

    bool channelFound(Channel channel);
    void addChannel(Channel channel);
    void createChannel();

    Channel* getChannelByName(const std::string &name);

    void handleMode(User &sender, std::vector<std::string> &args);
    void handleCommand(User &user, const std::string &cmd, std::vector<std::string> &args);

    void broadcastMode(Channel *channel, const std::string &setter, const std::string &modeString);
};

#endif
