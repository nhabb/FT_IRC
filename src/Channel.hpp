#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <set>
#include "User.hpp"

class Channel {
private:
    std::vector<User> users;
    std::string pass;
    std::string name;
    bool inviteOnly;
    bool topicRestricted;
    std::string key;
    size_t userLimit;
    std::set<std::string> operators;

public:
    int userCount;

    Channel(std::string pass);

    std::string getPass();
    std::string getName();
    bool getInviteOnly() const;
    bool getTopicRestricted() const;
    std::string getKey() const;
    size_t getUserLimit() const;
    bool isOperator(const std::string &nick) const;

    void setName(std::string name);
    void setInviteOnly(bool mode);
    void setTopicRestricted(bool mode);
    void setKey(const std::string &key);
    void setUserLimit(size_t limit);
    void addOperator(const std::string &nick);
    void removeOperator(const std::string &nick);

    void addUser(User user);
    bool isFound(User user);

    bool canChangeTopic(const std::string &nick) const;
};

#endif
