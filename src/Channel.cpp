#include "../includes/Channel.hpp"
#include <algorithm>

Channel::Channel() : inviteOnly(false), topicRestricted(false), userLimit(-1) {}
Channel::Channel(const std::string &n) : name(n), inviteOnly(false), topicRestricted(false), userLimit(-1) {}

void        Channel::setName(const std::string &n) { name = n; }
std::string Channel::getName() const { return name; }

void Channel::addUserFd(int fd) {
    if (!hasUserFd(fd)) usersFds.push_back(fd);
}

void Channel::removeUserFd(int fd) {
    for (size_t i = 0; i < usersFds.size(); ++i) {
        if (usersFds[i] == fd) { usersFds.erase(usersFds.begin() + i); break; }
    }
}

bool Channel::hasUserFd(int fd) const {
    for (size_t i = 0; i < usersFds.size(); ++i) if (usersFds[i] == fd) return true;
    return false;
}

std::vector<int>&       Channel::getUserFds()       { return usersFds; }
const std::vector<int>& Channel::getUserFds() const { return usersFds; }

void Channel::addOperator(const std::string &nick) {
    if (std::find(operators.begin(), operators.end(), nick) == operators.end())
        operators.push_back(nick);
}

bool Channel::isOperator(const std::string &nick) const {
    return std::find(operators.begin(), operators.end(), nick) != operators.end();
}

void Channel::setInviteOnly(bool v) { inviteOnly = v; }
bool Channel::isInviteOnly() const { return inviteOnly; }

void Channel::setTopicRestricted(bool v) { topicRestricted = v; }
bool Channel::isTopicRestricted() const { return topicRestricted; }

void Channel::setKey(const std::string &k) { key = k; }
std::string Channel::getKey() const { return key; }

void Channel::setUserLimit(int l) { userLimit = l; }
int  Channel::getUserLimit() const { return userLimit; }

void Channel::addInvite(const std::string &nick) {
    for (size_t i = 0; i < inviteList.size(); ++i)
        if (inviteList[i] == nick) return;
    inviteList.push_back(nick);
}

bool Channel::isInvited(const std::string &nick) const {
    for (size_t i = 0; i < inviteList.size(); ++i)
        if (inviteList[i] == nick) return true;
    return false;
}

void Channel::clearInvite(const std::string &nick) {
    for (size_t i = 0; i < inviteList.size(); ++i) {
        if (inviteList[i] == nick) {
            inviteList.erase(inviteList.begin() + i);
            break;
        }
    }
}

