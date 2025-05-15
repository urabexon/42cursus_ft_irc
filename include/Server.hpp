/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:18:03 by urabex            #+#    #+#             */
/*   Updated: 2025/05/16 01:30:36 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <string>
#include <vector>
#include <netdb.h>

struct serverOperator {
    std::string name;
    std::string host;
    std::string password;
};

class Server {
    
};

// utils
void        addToClientSendBuf(Server *server, int clientFd, std::string message);
Client      &retrieveClient(Server *server, int clientSockFd);
std::string getChannelName(std::string &msgToParse);
std::string trim(const std::string &s);
int         getCommandType(std::string &cmd);

#endif
