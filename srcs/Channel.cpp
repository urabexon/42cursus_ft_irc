/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:50:53 by urabex            #+#    #+#             */
/*   Updated: 2025/05/20 18:04:07 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name): _name(name), _topic(""), _password(""), _capacity(0), _clientList(), _invitedList(), _operatorList() {
	_mode.invite = false;
	_mode.topic = true;
	_mode.key = false;
	_mode.limit = false;
}

Channel::~Channel() {}

void Channel::setTopic(const std::string &topic) { 
    _topic = topic;
}

void Channel::setPassword(const std::string &password) {
    _password = password;
}

void Channel::setCapacity(int capacity) {
    _capacity = capacity;
}

void Channel::setMode(bool sign, const char mode) {
    switch (mode) {
        case 'i':
			_mode.invite = sign;
			break;
		case 't':
			_mode.topic = sign;
			break;
		case 'k':
			_mode.key = sign;
			break;
		case 'l':
			_mode.limit = sign;
			break;
		default:
			std::cerr << "invalid option" << std::endl;
			return ;
    }
}

const std::string &Channel::getName() const {
    return (_name);
}

const std::string &Channel::getTopic() const {
    return (_topic);
}

const std::string &Channel::getPassword() const {
    return (_password);
}

size_t Channel::getCapacity() const {
    return (_capacity);
}

std::map<const int, Client> &Channel::getClientList() {
    return (_clientList);
}

bool Channel::getMode(const std::string mode) const {
    if (mode == "i" || mode == "invite")
		return (_mode.invite);
	else if (mode == "t" || mode == "topic")
		return (_mode.topic);
	else if (mode == "k" || mode == "key")
		return (_mode.key);
	else if (mode == "l" || mode == "limit")
		return (_mode.limit);
	else
		return (false);
}

bool Channel::isClientInChannel(const int clientFd) {
    std::map<const int, Client>::iterator it = _clientList.find(clientFd);
	if (it == _clientList.end())
		return (false);
	return (true);
}

bool Channel::isInvited(const int clientFd) {
	for (std::vector<int>::iterator it = _invitedList.begin(); it != _invitedList.end(); ++it) {
		if (*it == clientFd)
			return (true);
	}
	return (false);
}

bool Channel::isOperator(const int clientFd) {
	for (std::vector<int>::iterator it = _operatorList.begin(); it != _operatorList.end(); ++it) {
		if (*it == clientFd)
			return (true);
	}
	return (false);
}

void Channel::addClientToChannel(Client &client) {
    _clientList.insert(std::make_pair(client.getClientFd(), client));
}

void Channel::addOperatorList(int clientFd) {
    _operatorList.push_back(clientFd);
}

void Channel::addInvitedList(int clientFd) {
    _invitedList.push_back(clientFd);
}

void Channel::removeClient(int clientFd) {
	_clientList.erase(clientFd);
	if (isOperator(clientFd))
		removeOperator(clientFd);
	if (isInvited(clientFd))
		removeInvited(clientFd);
}

void Channel::removeInvited(int clientFd) {
	for (std::vector<int>::iterator it = _invitedList.begin(); it != _invitedList.end(); ++it) {
		if (*it == clientFd) {
			_invitedList.erase(it);
			return ;
		}
	}
}

void Channel::removeOperator(int clientFd) {
	for (std::vector<int>::iterator it = _operatorList.begin(); it != _operatorList.end(); ++it) {
		if (*it == clientFd) {
			_operatorList.erase(it);
			return ;
		}
	}
}

std::string Channel::getClientListString() {
	std::string clientListString = "";
	std::map<const int, Client>::iterator it = _clientList.begin();
	
	while (it != _clientList.end()) {
		// チャンネルオペレーターの場合、@をつける
		if (isOperator(it->first))
			clientListString += "@";
		clientListString += it->second.getNickname();
		++it;
		if (it != _clientList.end())
			clientListString += " ";
	}
	return (clientListString);
}
