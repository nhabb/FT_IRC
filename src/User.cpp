#include "../includes/User.hpp"

User::User() : fd(-1), auth(false), registered(false) {}
User::User(int f) : fd(f), auth(false), registered(false) {}

int         User::getFd() const { return fd; }
void        User::setNick(const std::string &n) { nick = n; }
void        User::setUser(const std::string &u) { username = u; }
void        User::setReal(const std::string &r) { realname = r; }
std::string User::getNick() const { return nick; }
std::string User::getUser() const { return username; }
std::string User::getReal() const { return realname; }
void        User::setAuth(bool v) { auth = v; }
bool        User::isAuth() const { return auth; }
void        User::setRegistered(bool v) { registered = v; }
bool        User::isRegistered() const { return registered; }

void User::appendRaw(const std::string &chunk) { inbuf += chunk; }

bool User::extractLine(std::string &line) {
    std::string::size_type pos = inbuf.find("\r\n");
    if (pos == std::string::npos) return false;
    line = inbuf.substr(0, pos);
    inbuf.erase(0, pos + 2);
    return true;
}
