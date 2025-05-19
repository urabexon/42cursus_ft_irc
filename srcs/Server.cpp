/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:19:36 by urabex            #+#    #+#             */
/*   Updated: 2025/05/20 00:31:05 by urabex           ###   ########.fr       */
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

bool Server::_signal = false;

void Server::signalHandler(int signal) {
	(void)signal;
    std::cout << "[Server] Server shutdown..." << std::endl;
	Server::_signal = true;
}

void Server::getServerInfo() {
    int status;

    // サーバーが利用するネットワークアドレス情報を取得
	if ((status = getaddrinfo(NULL, _port.c_str(), &_addrInfo, &_serverInfo)) != 0)
		throw ("Error: " + std::string(gai_strerror(status)));
}

void Server::launchServer() {
    // ソケットを作成
    if ((_serverSockFd = socket(_serverInfo->ai_family, _serverInfo->ai_socktype, _serverInfo->ai_protocol)) < 0)
        throw (ERROR_SERVER_SOCKET);
    
    // ソケットをノンブロッキングモードに設定
    if (fcntl(_serverSockFd, F_SETFL, O_NONBLOCK) == -1)
        throw (ERROR_SERVER_SETSOCKETOPT);
    int flags = fcntl(_serverSockFd, F_GETFL, 0);
    if (flags == -1)
        throw (ERROR_SERVER_SETSOCKETOPT);
    if (fcntl(_serverSockFd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw (ERROR_SERVER_SETSOCKETOPT);
    // ソケットオプション設定
    int socketOpt = 1;
	if (setsockopt(_serverSockFd, SOL_SOCKET, SO_REUSEADDR, &socketOpt, sizeof(socketOpt)) < 0)
		throw (ERROR_SERVER_SETSOCKETOPT);
    // ソケットにアドレスをバインド
	if (bind(_serverSockFd, _serverInfo->ai_addr, _serverInfo->ai_addrlen) < 0)
		throw (ERROR_SERVER_BIND);
    // 待ち受け状態にする
	if (listen(_serverSockFd, 10) < 0)
		throw (ERROR_SERVER_LISTEN);
    // 取得済みのアドレス情報は、ソケットのバインド後は不要となるため、メモリ解放
	freeaddrinfo(_serverInfo);
}
