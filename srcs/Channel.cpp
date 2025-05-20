/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:50:53 by urabex            #+#    #+#             */
/*   Updated: 2025/05/20 12:51:42 by urabex           ###   ########.fr       */
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