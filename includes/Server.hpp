#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <poll.h>
#include "User.hpp"
#include "Channel.hpp"

#define BUFFER_SIZE 512
#define MAX_CLIENTS 100

class Server {
private:
    std::string              port;
    std::string              pass;
    int                      server_fd;
    std::vector<User>        clients;   // users indexed by fd lookup helpers
    std::vector<Channel>     channels;
    std::vector<struct pollfd> pollfds;

    // helpers
    User*    findUserByFd(int fd);
    User*    findUserByNick(const std::string &nick);
    bool     nickInUse(const std::string &nick, int exceptFd) const;
    Channel* getChannelByName(const std::string &name);
    Channel* getOrCreateChannel(const std::string &name);
    void     sendToFd(int fd, const std::string &msg);

public:
    Server(std::string port, std::string pass);
    ~Server();

    void start();
    void acceptClient();
    void disconnectClient(int fd);
    void handleClientInput(int fd);
    void addChannel(Channel channel);
    bool channelFound(Channel channel);

    // command handling
    void handleCommand(User &user, const std::string &cmd, std::vector<std::string> &args);
    void handlePass(User &user, std::vector<std::string> &args);
    void handleNick(User &user, std::vector<std::string> &args);
    void handleUser(User &user, std::vector<std::string> &args);
    void handleJoin(User &user, std::vector<std::string> &args);
    void handlePrivmsg(User &user, std::vector<std::string> &args);
    void handleMode(User &user, std::vector<std::string> &args); // your existing logic hooks here
	void handleInvite(User &user, std::vector<std::string> &args);
	void handleKick(User &user, std::vector<std::string> &args);
	// In Server.hpp (private or public as you like)
	std::string pickNextOpNick(Channel* c, const std::string& excludeNick = "");
    // broadcast
    void broadcast(Channel *chan, const std::string &msg, int exceptFd = -1);
    void broadcastMode(Channel *chan, const std::string &setter, const std::string &modeString);
};

// parser (kept as you had)
void parseCommand(const std::string &line, std::string &cmd, std::vector<std::string> &args);

#endif
