#include "../includes/Server.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

// ---------- helpers (finders) ----------

User *Server::findUserByFd(int fd)
{
	for (size_t i = 0; i < clients.size(); ++i)
		if (clients[i].getFd() == fd)
			return &clients[i];
	return 0;
}

User *Server::findUserByNick(const std::string &nick)
{
	for (size_t i = 0; i < clients.size(); ++i)
		if (clients[i].getNick() == nick)
			return &clients[i];
	return 0;
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
        std::cout<<"Added channel "<<channel.getName()<<std::endl;
    }
    else
        std::cout<<"Channel "<<channel.getName()<<" already found"<<std::endl;
}

bool Server::nickInUse(const std::string &nick, int exceptFd) const
{
	for (size_t i = 0; i < clients.size(); ++i)
		if (clients[i].getNick() == nick && clients[i].getFd() != exceptFd)
			return true;
	return false;
}

Channel *Server::getChannelByName(const std::string &name)
{
	for (size_t i = 0; i < channels.size(); ++i)
		if (channels[i].getName() == name)
			return &channels[i];
	return 0;
}

Channel *Server::getOrCreateChannel(const std::string &name)
{
	Channel *c = getChannelByName(name);
	if (c)
		return c;
	channels.push_back(Channel(name));
	return &channels.back();
}

void Server::sendToFd(int fd, const std::string &msg)
{
	if (fd >= 0 && !msg.empty())
		send(fd, msg.c_str(), msg.size(), 0);
}

// ---------- lifecycle ----------

Server::Server(std::string p, std::string pw) : port(p), pass(pw), server_fd(-1)
{
	std::cout << "Server created. Welcome to IRCSSSSSS" << std::endl;
}

Server::~Server()
{
	if (server_fd != -1)
		close(server_fd);
}

void Server::start()
{
	struct sockaddr_in address;
	int opt = 1;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket");
		exit(1);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(port.c_str()));

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind");
		exit(1);
	}

	if (listen(server_fd, MAX_CLIENTS) < 0)
	{
		perror("listen");
		exit(1);
	}

	struct pollfd pfd;
	pfd.fd = server_fd;
	pfd.events = POLLIN;
	pollfds.push_back(pfd);

	std::cout << "IRC Server listening on port " << port << std::endl;

	while (true)
	{
		if (poll(&pollfds[0], pollfds.size(), -1) < 0)
		{
			perror("poll");
			break;
		}

		for (size_t i = 0; i < pollfds.size(); ++i)
		{
			if (pollfds[i].revents & POLLIN)
			{
				if (pollfds[i].fd == server_fd)
				{
					acceptClient();
				}
				else
				{
					handleClientInput(pollfds[i].fd);
				}
			}
		}
	}
}

void Server::acceptClient()
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
	if (client_fd < 0)
	{
		perror("accept");
		return;
	}

	clients.push_back(User(client_fd));

	struct pollfd pfd;
	pfd.fd = client_fd;
	pfd.events = POLLIN;
	pollfds.push_back(pfd);

	std::cout << "New client connected (User " << client_fd - 3 << ")" << std::endl;

	// Step-by-step onboarding
	sendToFd(client_fd, "NOTICE * :Welcome to IRCssssssssssss.\nPlease authenticate: PASS <password>\r\n");
	sendToFd(client_fd, "Hala bel khamis: ");
}

void Server::disconnectClient(int fd)
{
	std::cout << "Client disconnected (User " << fd - 3 << ")" << std::endl;
	// Remove from channels
	for (size_t c = 0; c < channels.size(); ++c)
		channels[c].removeUserFd(fd);

	// Remove from clients
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i].getFd() == fd)
		{
			clients.erase(clients.begin() + i);
			break;
		}
	}
	// Remove from pollfds
	for (size_t i = 0; i < pollfds.size(); ++i)
	{
		if (pollfds[i].fd == fd)
		{
			pollfds.erase(pollfds.begin() + i);
			break;
		}
	}
	close(fd);
}

void parseCommand(const std::string &line, std::string &cmd, std::vector<std::string> &args)
{
	args.clear();
	cmd.clear();
	std::istringstream iss(line);
	std::string token;
	if (!(iss >> cmd))
		return;
	while (iss >> token)
	{
		if (!token.empty() && token[0] == ':')
		{
			std::string trailing = token.substr(1);
			std::string rest;
			std::getline(iss, rest);
			if (!rest.empty() && rest[0] == ' ')
				rest.erase(0, 1);
			args.push_back(trailing + (rest.empty() ? "" : " " + rest));
			break;
		}
		else
			args.push_back(token);
	}
}

void Server::handleClientInput(int fd)
{
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);

	int n = recv(fd, buf, BUFFER_SIZE - 1, 0);
	if (n <= 0)
	{
		disconnectClient(fd);
		return;
	}

	User *u = findUserByFd(fd);
	if (!u)
		return;

	u->appendRaw(std::string(buf, n));

	// process all complete lines (\r\n)
	std::string line, cmd;
	std::vector<std::string> args;
	while (u->extractLine(line))
	{
		if (line.empty())
			continue;
		cmd.clear();
		args.clear();
		parseCommand(line, cmd, args);
		if (!cmd.empty())
			handleCommand(*u, cmd, args);
		// Send prompt after processing each line
		sendToFd(fd, "Hala bel khamis: ");
	}
}

void Server::handleInvite(User &user, std::vector<std::string> &args)
{
	if (!user.isRegistered())
	{
		sendToFd(user.getFd(), "ERROR :You are not registered\r\n");
		return;
	}
	if (args.size() < 2)
	{
		sendToFd(user.getFd(), "ERROR :INVITE requires <nick> <channel>\r\n");
		return;
	}

	std::string targetNick = args[0];
	std::string chanName = args[1];

	// Helpful hint if user reversed args
	if (!targetNick.empty() && targetNick[0] == '#')
	{
		sendToFd(user.getFd(), "ERROR :INVITE usage is INVITE <nick> <channel>\r\n");
		return;
	}

	Channel *c = getChannelByName(chanName);
	if (!c)
	{
		sendToFd(user.getFd(), "ERROR :No such channel\r\n");
		return;
	}

	if (!c->isOperator(user.getNick()))
	{
		sendToFd(user.getFd(), "ERROR :You're not a channel operator\r\n");
		return;
	}

	User *target = findUserByNick(targetNick);
	if (!target)
	{
		sendToFd(user.getFd(), "ERROR :No such nick\r\n");
		return;
	}

	c->addInvite(targetNick);
	sendToFd(user.getFd(), "NOTICE " + user.getNick() + " :Invited " + targetNick + " to " + chanName + "\r\n");
	sendToFd(target->getFd(), ":" + user.getNick() + " INVITE " + targetNick + " :" + chanName + "\r\n");
}

// ---------- command dispatch ----------

void Server::handleKick(User &user, std::vector<std::string> &args)
{
    if (!user.isRegistered())
    { sendToFd(user.getFd(), "ERROR :You are not registered\r\n"); return; }

    if (args.size() < 2)
    { sendToFd(user.getFd(), "ERROR :KICK requires <channel> <nick> [:reason]\r\n"); return; }

    const std::string chanName   = args[0];
    const std::string targetNick = args[1];
    const std::string reason     = (args.size() >= 3 ? args[2] : "Kicked");

    Channel *c = getChannelByName(chanName);
    if (!c)
    { sendToFd(user.getFd(), "ERROR :No such channel " + chanName + "\r\n"); return; }

    if (!c->isOperator(user.getNick()))
    { sendToFd(user.getFd(), "ERROR :You're not a channel operator\r\n"); return; }

    User *target = findUserByNick(targetNick);
    if (!target || !c->hasUserFd(target->getFd()))
    {
        sendToFd(user.getFd(), "ERROR :User " + targetNick + " is not on " + chanName + "\r\n");
        return;
    }

    // 1) Broadcast the KICK (from acting user)
    std::string kickLine = ":" + user.getNick() + " KICK " + chanName + " " + targetNick + " :" + reason + "\nSallemleeeeee\n";
    broadcast(c, kickLine);

    // 2) Track if target was an op, then remove from channel and op list
    const bool targetWasOp = c->isOperator(targetNick);
    c->removeUserFd(target->getFd());
    if (targetWasOp) c->removeOp(targetNick);

    // 3) If we removed the last operator, auto-promote next eligible member
    if (targetWasOp && c->getNumOperators() == 0)
    {
        std::string newOp = pickNextOpNick(c, targetNick);
        if (!newOp.empty())
        {
            c->addOperator(newOp);
            // Announce as server-origin MODE (you've used ":server" elsewhere already)
            std::string modeLine = ":server MODE " + chanName + " +o " + newOp + "\r\n";
            broadcast(c, modeLine);
        }
        // If no eligible member remains, channel stays with zero ops (allowed)
    }

    // 4) Optionally notify kicked user directly (they likely already got the broadcast)
    // sendToFd(target->getFd(), kickLine);
}


void Server::handleCommand(User &user, const std::string &cmd, std::vector<std::string> &args)
{
	if (cmd == "PASS")
		handlePass(user, args);
	else if (cmd == "NICK")
		handleNick(user, args);
	else if (cmd == "USER")
		handleUser(user, args);
	else if (cmd == "JOIN")
		handleJoin(user, args);
	else if (cmd == "PRIVMSG")
		handlePrivmsg(user, args);
	else if (cmd == "MODE")
		handleMode(user, args); // your modes handler
	else if (cmd == "INVITE")
		handleInvite(user, args);
	else if (cmd == "KICK")
		handleKick(user, args);
	else
		sendToFd(user.getFd(), "ERROR :Unknown command " + cmd + "\r\n");
}

void Server::handlePass(User &user, std::vector<std::string> &args)
{
	if (args.empty())
	{
		sendToFd(user.getFd(), "NOTICE * :Usage: PASS <password>\r\n");
		return;
	}
	if (args[0] == pass)
	{
		user.setAuth(true);
		sendToFd(user.getFd(), "NOTICE * :Password accepted. Now send NICK <nickname>.\r\n");
	}
	else
	{
		// Do NOT disconnect. Keep asking.
		sendToFd(user.getFd(), "NOTICE * :Invalid password. Try again with PASS <password>.\r\n");
	}
}

void Server::handleNick(User &user, std::vector<std::string> &args)
{
	if (args.empty())
	{
		sendToFd(user.getFd(), "ERROR :NICK requires a nickname\r\n");
		return;
	}
	if (nickInUse(args[0], user.getFd()))
	{
		sendToFd(user.getFd(), "ERROR :Nickname is already in use\r\n");
		return;
	}
	user.setNick(args[0]);
	if (!user.isAuth())
		sendToFd(user.getFd(), "NOTICE * :Now authenticate with PASS <password>.\r\n");
	else
		sendToFd(user.getFd(), "NOTICE * :Nick set. Now send USER <username> 0 * :<realname>.\r\n");
}

void Server::handleUser(User &user, std::vector<std::string> &args)
{
	if (args.size() < 4)
	{
		sendToFd(user.getFd(), "ERROR :USER requires 4 params\r\n");
		return;
	}
	user.setUser(args[0]);
	user.setReal(args[3]);

	if (user.isAuth() && !user.getNick().empty() && !user.isRegistered())
	{
		user.setRegistered(true);
		sendToFd(user.getFd(), ":server 001 " + user.getNick() + " :Welcome to IRCSSSSSSSSSSS\r\n");
	}
	else if (!user.isAuth())
	{
		sendToFd(user.getFd(), "NOTICE * :Please PASS <password> first.\r\n");
	}
	else if (user.getNick().empty())
	{
		sendToFd(user.getFd(), "NOTICE * :Please set NICK <nickname> first.\r\n");
	}
}

void Server::handleJoin(User &user, std::vector<std::string> &args)
{
	if (!user.isRegistered())
	{
		sendToFd(user.getFd(), "ERROR :You are not registered\r\n");
		return;
	}
	if (args.empty())
	{
		sendToFd(user.getFd(), "ERROR :JOIN requires a channel name\r\n");
		return;
	}

	const std::string chanName = args[0];
	const std::string providedKey = (args.size() >= 2 ? args[1] : "");

	Channel *c = getChannelByName(chanName);
	if (!c)
	{
		// first JOIN creates channel
		c = getOrCreateChannel(chanName);
		if (!c->hasUserFd(user.getFd()))
		{
			c->addUserFd(user.getFd());
			if (c->getUserFds().size() == 1)
				c->addOperator(user.getNick());
			broadcast(c, ":" + user.getNick() + " JOIN " + chanName + "\r\n");
		}
		return;
	}

	// +i: allow only if invited
	if (c->isInviteOnly() && !c->isInvited(user.getNick()))
	{
		sendToFd(user.getFd(), "ERROR :Cannot join channel (+i)\r\n");
		return;
	}

	// +k: check key
	if (!c->getKey().empty() && c->getKey() != providedKey)
	{
		sendToFd(user.getFd(), "ERROR :Cannot join channel (+k) — bad key\r\n");
		return;
	}

	// +l: check limit
	if (c->getUserLimit() >= 0 && (int)c->getUserFds().size() >= c->getUserLimit())
	{
		sendToFd(user.getFd(), "ERROR :Cannot join channel (+l) — channel is full\r\n");
		return;
	}

	if (!c->hasUserFd(user.getFd()))
	{
		c->addUserFd(user.getFd());
		if (c->getUserFds().size() == 1)
			c->addOperator(user.getNick());
		broadcast(c, ":" + user.getNick() + " JOIN " + chanName + "\r\n");
		// consume invite on success
		c->clearInvite(user.getNick());
	}
}

void Server::handlePrivmsg(User &user, std::vector<std::string> &args)
{
	if (!user.isRegistered())
	{
		sendToFd(user.getFd(), "ERROR :You are not registered\r\n");
		return;
	}
	if (args.size() < 2)
	{
		sendToFd(user.getFd(), "ERROR :PRIVMSG requires a target and message\r\n");
		return;
	}

	std::string target = args[0];
	std::string message = args[1];

	if (!target.empty() && target[0] == '#')
	{
		Channel *c = getChannelByName(target);
		if (!c)
		{
			sendToFd(user.getFd(), "ERROR :No such channel " + target + "\r\n");
			return;
		}
		if (!c->hasUserFd(user.getFd()))
		{
			sendToFd(user.getFd(), "ERROR :You're not on " + target + "\r\n");
			return;
		}
		std::string wire = ":" + user.getNick() + " PRIVMSG " + target + " :" + message + "\r\n";
		broadcast(c, wire, user.getFd());
	}
	else
	{
		// direct message to nick
		User *dst = findUserByNick(target);
		if (!dst)
		{
			sendToFd(user.getFd(), "ERROR :No such nick " + target + "\r\n");
			return;
		}
		std::string wire = ":" + user.getNick() + " PRIVMSG " + target + " :" + message + "\r\n";
		sendToFd(dst->getFd(), wire);
	}
}

void Server::handleMode(User &user, std::vector<std::string> &args)
{
	if (args.empty())
	{
		sendToFd(user.getFd(), "ERROR :MODE requires <channel> [modes]\r\n");
		return;
	}

	const std::string chanName = args[0];
	Channel *c = getChannelByName(chanName);
	if (!c)
	{
		sendToFd(user.getFd(), "ERROR :No such channel " + chanName + "\r\n");
		return;
	}

	// MODE #chan  → show current modes
	if (args.size() == 1)
	{
		std::string modes = "+";
		if (c->isInviteOnly())
			modes += "i";
		if (c->isTopicRestricted())
			modes += "t";
		if (!c->getKey().empty())
			modes += "k";
		if (c->getUserLimit() >= 0)
			modes += "l";

		std::string tail;
		if (!c->getKey().empty())
			tail += " " + c->getKey();
		std::ostringstream oss;
		if (c->getUserLimit() >= 0)
			tail += " " + static_cast<std::ostringstream &>(oss << c->getUserLimit()).str();

		// Minimal, human-friendly reply (not full RFC numerics):
		sendToFd(user.getFd(), ":server 324 " + user.getNick() + " " + chanName + " " + modes + tail + "\r\n");
		return;
	}

	// From here on, we’re changing modes → must be operator
	if (!c->isOperator(user.getNick()))
	{
		sendToFd(user.getFd(), "ERROR :You're not a channel operator\r\n");
		return;
	}

	const std::string modesArg = args[1];
	int sign = +1;	 // + add, - remove
	size_t argi = 2; // extra params (for k,l,o)

	for (size_t i = 0; i < modesArg.size(); ++i)
	{
		char ch = modesArg[i];
		if (ch == '+')
		{
			sign = +1;
			continue;
		}
		if (ch == '-')
		{
			sign = -1;
			continue;
		}

		if (ch == 'i')
		{
			c->setInviteOnly(sign == +1);
		}
		else if (ch == 't')
		{
			c->setTopicRestricted(sign == +1);
		}
		else if (ch == 'k')
		{
			if (sign == +1)
			{
				if (argi >= args.size())
				{
					sendToFd(user.getFd(), "ERROR :MODE +k requires a key\r\n");
					continue;
				}
				c->setKey(args[argi++]);
			}
			else
			{
				c->setKey("");
			}
		}
		else if (ch == 'l')
		{
			if (sign == +1)
			{
				if (argi >= args.size())
				{
					sendToFd(user.getFd(), "ERROR :MODE +l requires a limit\r\n");
					continue;
				}
				c->setUserLimit(atoi(args[argi++].c_str()));
			}
			else
			{
				c->setUserLimit(-1);
			}
		}
		else if (ch == 'o')
		{
			if (argi >= args.size())
			{
				sendToFd(user.getFd(), "ERROR :MODE +o/-o requires a nickname\r\n");
				continue;
			}
			const std::string opnick = args[argi++];
			if (sign == +1)
			{
				c->addOperator(opnick);
				broadcastMode(c, user.getNick(), "+o " + opnick);
			}
			else // -o
			{
				if (!c->isOperator(opnick))
				{
					sendToFd(user.getFd(), "ERROR :User " + opnick + " is not an operator\r\n");
					continue;
				}
				c->removeOp(opnick);
				broadcastMode(c, user.getNick(), "-o " + opnick);

				if (c->getNumOperators() == 0)
				{
					std::string newOp = pickNextOpNick(c, opnick);
					if (!newOp.empty())
					{
						c->addOperator(newOp);
						broadcast(c, ":server MODE " + c->getName() + " +o " + newOp + "\r\n");
					}
				}
			}
		}
	}

	broadcastMode(c, user.getNick(), modesArg);
}

// ---------- broadcast helpers ----------

void Server::broadcast(Channel *chan, const std::string &msg, int exceptFd)
{
	std::vector<int> &fds = chan->getUserFds();
	for (size_t i = 0; i < fds.size(); ++i)
	{
		if (fds[i] != exceptFd)
			sendToFd(fds[i], msg);
	}
}

void Server::broadcastMode(Channel *chan, const std::string &setter, const std::string &modeString)
{
	broadcast(chan, ":" + setter + " MODE " + chan->getName() + " " + modeString + "\r\n");
}


std::string Server::pickNextOpNick(Channel* c, const std::string& excludeNick)
{
    if (!c) return std::string();
    std::vector<int>& fds = c->getUserFds();   // your existing container
    for (size_t i = 0; i < fds.size(); ++i)
    {
        User* u = findUserByFd(fds[i]);
        if (!u) continue;
        const std::string& nick = u->getNick();
        if (nick.empty()) continue;
        if (nick == excludeNick) continue;
        if (!c->isOperator(nick))
            return nick;                       // first eligible by join order
    }
    return std::string();
}
