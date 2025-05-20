/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:50:53 by urabex            #+#    #+#             */
/*   Updated: 2025/05/20 16:13:28 by urabex           ###   ########.fr       */
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

