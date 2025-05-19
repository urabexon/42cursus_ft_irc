/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:22:13 by urabex            #+#    #+#             */
/*   Updated: 2025/05/19 22:23:30 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Server.hpp"

void addToClientSendBuf(Server *server, int clientFd, std::string message) {
    
}

Client &retrieveClient(Server *server, int clientFd) {
    
}

std::string getChannelName(std::string &msgToParse) {

}

// 文字列の先頭と末尾の空白（スペース、タブ）を除去する
std::string trim(const std::string &s) {
    
}

int getCommandType(std::string &cmd) {
    
}
