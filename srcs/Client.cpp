/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:19:33 by urabex            #+#    #+#             */
/*   Updated: 2025/05/19 22:04:29 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int clientFd) : _clientFd(clientFd), _userName(""), _realName(""), _nickname(""), _oldNickname(""), _readBuf(""), _sendBuf(""), _connexion_password(false), _nmInfo(0), _toDeconnect(false), _registrationDone(false) {}

Client::~Client() {}

