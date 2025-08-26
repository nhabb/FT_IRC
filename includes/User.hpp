#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
private:
    int         fd;
    std::string nick;
    std::string username;
    std::string realname;
    bool        auth;        // PASS accepted
    bool        registered;  // after PASS+NICK+USER
    std::string inbuf;       // partial data accumulator per client

public:
    User();
    User(int fd);

    // identity / state
    int         getFd() const;
    void        setNick(const std::string &n);
    void        setUser(const std::string &u);
    void        setReal(const std::string &r);
    std::string getNick() const;
    std::string getUser() const;
    std::string getReal() const;
    void        setAuth(bool v);
    bool        isAuth() const;
    void        setRegistered(bool v);
    bool        isRegistered() const;

    // input buffering helpers (for \r\n framed commands)
    void        appendRaw(const std::string &chunk);
    bool        extractLine(std::string &line); // returns one full line without \r\n if available
};

#endif
