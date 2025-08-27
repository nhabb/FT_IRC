#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "User.hpp"

class Channel {
private:
    std::string        name;
    std::vector<int>   usersFds;       // store FDs (stable) instead of User* (invalidated by vector reallocs)
    std::vector<std::string> operators; // nicks of operators
    bool               inviteOnly;
    bool               topicRestricted;
    std::string        key;
    int                userLimit;      // -1 means no limit
    std::vector<std::string> inviteList;
    int                userCount;
    std::string        topic;

public:
    Channel();
    Channel(const std::string &n);

    std::string getName() const;
    void        setName(std::string n);

    int getNumOperators();
    std::string getNewop();
    // membership
    void addUserFd(int fd);
    void removeUserFd(int fd);
    bool hasUserFd(int fd) const;
    std::vector<int>&       getUserFds();
    const std::vector<int>& getUserFds() const;
    void removeOp(std::string nick);

    // operators
    void addOperator(const std::string &nick);
    bool isOperator(const std::string &nick) const;

    // modes
    void setInviteOnly(bool v);
    bool isInviteOnly() const;
    void setTopicRestricted(bool v);
    bool isTopicRestricted() const;
    void setKey(const std::string &k);
    std::string getKey() const;
    void setUserLimit(int l);
    int  getUserLimit() const;
	void addInvite(const std::string &nick);
    bool isInvited(const std::string &nick) const;
    void clearInvite(const std::string &nick);
    bool getInviteOnly() const;
    bool getTopicRestricted() const;
    std::string getTopic() const;
    void setTopic(const std::string &t);
    std::string findNick(std::vector<std::string> operators, const std::string &nick) const;
    void removeOperator(const std::string &nick);
    // void addUser(User user);
    // bool isFound(User user);
};

#endif
