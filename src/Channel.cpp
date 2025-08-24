#include "Channel.hpp"

Channel::Channel(std::string pass)
    : pass(pass), name(""),
      inviteOnly(false), topicRestricted(false),
      key(""), userLimit(0), userCount(0)
{}

std::string Channel::getPass() {
    return pass;
}

std::string Channel::getName() {
    return name;
}

bool Channel::getInviteOnly() const {
    return inviteOnly;
}

bool Channel::getTopicRestricted() const {
    return topicRestricted;
}

std::string Channel::getKey() const {
    return key;
}

size_t Channel::getUserLimit() const {
    return userLimit;
}

bool Channel::isOperator(const std::string &nick) const {
    return operators.find(nick) != operators.end();
}

void Channel::setName(std::string name) {
    this->name = name;
}

void Channel::setInviteOnly(bool mode) {
    inviteOnly = mode;
}

void Channel::setTopicRestricted(bool mode) {
    topicRestricted = mode;
}

void Channel::setKey(const std::string &key) {
    this->key = key;
}

void Channel::setUserLimit(size_t limit) {
    userLimit = limit;
}

void Channel::addOperator(const std::string &nick) {
    operators.insert(nick);
}

void Channel::removeOperator(const std::string &nick) {
    operators.erase(nick);
}

void Channel::addUser(User user) {
    users.push_back(user);
    userCount++;
}

bool Channel::isFound(User user) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].getNick() == user.getNick()) {
            return true;
        }
    }
    return false;
}

bool Channel::canChangeTopic(const std::string &nick) const {
    if (!topicRestricted) return true;
    return isOperator(nick);
}