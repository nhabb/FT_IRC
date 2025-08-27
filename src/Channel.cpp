#include "../includes/Channel.hpp"
#include <algorithm>

Channel::Channel() : inviteOnly(false), topicRestricted(false), userLimit(-1) {}
Channel::Channel(const std::string &n) : name(n), inviteOnly(false), topicRestricted(false), userLimit(-1) {}

std::string Channel::getName() const { return name; }

void Channel::addUserFd(int fd) {
    if (!hasUserFd(fd)) usersFds.push_back(fd);
}

void Channel::setName(std::string n) {
    name = n;
}

bool Channel::getInviteOnly() const {
    return inviteOnly;
}

bool Channel::getTopicRestricted() const {
    return topicRestricted;
}

void Channel::addOperator(const std::string &nick) {
    // avoid duplicates
    if (std::find(operators.begin(), operators.end(), nick) == operators.end())
        operators.push_back(nick);
}

void Channel::removeOperator(const std::string &nick) {
    std::vector<std::string>::iterator it =
        std::find(operators.begin(), operators.end(), nick);
    if (it != operators.end())
        operators.erase(it);
}

// void Channel::addUser(User user) {
//     users.push_back(user);
//     userCount++;
// }

// bool Channel::isFound(User user) {
//     for (size_t i = 0; i < users.size(); i++) {
//         if (users[i].getNick() == user.getNick()) {
//             return true;
//         }
//     }
//     return false;
// }

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

bool Channel::isOperator(const std::string &nick) const {
    return std::find(operators.begin(), operators.end(), nick) != operators.end();
}

bool Channel::isInviteOnly() const { return inviteOnly; }

void Channel::setInviteOnly(bool mode) {
    inviteOnly = mode;
}

bool Channel::isTopicRestricted() const { return topicRestricted; }

void Channel::setTopicRestricted(bool mode) {
    topicRestricted = mode;
}

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
