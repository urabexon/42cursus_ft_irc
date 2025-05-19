/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:19:36 by urabex            #+#    #+#             */
/*   Updated: 2025/05/19 21:50:20 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string port, std::string password, struct tm *timeinfo) : _serverSockFd(-1), _port(port), _password(password), _clientList(), _channelList(), _serverInfo(NULL) {
    std::cout << "[Server] Server running..." << std::endl;
    
    // 時間を文字列として取得
    char buf[80];
    strftime(buf, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    _dateTime = std::string(buf);

    // サーバー初期設定
    memset(&_addrInfo, 0, sizeof(_addrInfo));
    _addrInfo.ai_family = AF_INET;       // IPv4
    _addrInfo.ai_socktype = SOCK_STREAM; // TCP
    _addrInfo.ai_flags = AI_PASSIVE;     // ローカルホストで動作
}

Server::~Server() {}

// Getter
std::string &Server::getPassword() { return (_password); }
std::string &Server::getDateTime() { return (_dateTime); }
std::map<const int, Client>& Server::getClientList() { return (_clientList); }
std::map<std::string, Channel>& Server::getChannelList() { return (_channelList); }
