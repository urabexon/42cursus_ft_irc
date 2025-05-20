/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:19:36 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 00:59:39 by urabex           ###   ########.fr       */
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
std::string &Server::getPassword() {
    return (_password);
}

std::string &Server::getDateTime() {
    return (_dateTime);
}

std::map<const int, Client>& Server::getClientList() {
    return (_clientList);
}

std::map<std::string, Channel>& Server::getChannelList() {
    return (_channelList);
}

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
    
    // if (fcntl(_serverSockFd, F_SETFL, O_NONBLOCK) == -1)
    //     throw (ERROR_SERVER_SETSOCKETOPT);

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

// クライアントリストからクライアントFDに対応するクライアントが存在するかチェック
bool Server::isClientExist(int clientFd) {
    std::map<const int, Client>::iterator it = _clientList.find(clientFd);
	if (it == _clientList.end())
		return (false);
	return (true);
}

// クライアントリストからニックネームに対応するクライアントが存在するかチェック
bool Server::isClientExist(std::string &nickname) {
    std::map<const int, Client>::iterator it = _clientList.begin();
	while (it != _clientList.end()) {
		if (it->second.getNickname() == nickname)
			return (true);
		++it;
	}
	return (false);
}

// クライアントリストからクライアントのFDに対応するクライアントを取得
Client *Server::getClient(int clientFd) {
    std::map<const int, Client>::iterator it = _clientList.find(clientFd);
	// クライアントが見つからなかった場合、エラー文を出力してNULLを返す
	if (it == _clientList.end())
		return (NULL);
	return (&it->second);
}

// クライアントリストからニックネームに対応するクライアントのFDを取得
int Server::getClientFdByNick(std::string &nick) {
    std::map<const int, Client>::iterator it = _clientList.begin();
	while (it != _clientList.end()) {
		if (it->second.getNickname() == nick)
			return (it->first);
		++it;
	}
	return (-1);
}

// クライアントリストからクライアントのFDに対応するニックネームを取得
std::string Server::getNickname(int clientFd) {
    std::map<const int, Client>::iterator it = _clientList.find(clientFd);
	if (it == _clientList.end())
		return ("");
	return (it->second.getNickname());
}

// クライアントリストに追加
void Server::addClient(int clientFd, std::vector<pollfd> &tmpPollFds) {
    pollfd	clientPollFd;
	Client	newClient(clientFd);

	// クライアントが送受信できるように設定し、tmpPollFdsに格納
	clientPollFd.fd = clientFd;
    // 送受信
	clientPollFd.events = POLLIN | POLLOUT;
	tmpPollFds.push_back(clientPollFd);

	// クライアントリストに追加
	_clientList.insert(std::pair<int, Client>(clientFd, newClient));
	std::cout << SERVER_ADD_CLIENT << clientFd << std::endl;
}

// クライアントリストから削除し、ソケットを閉じる
void Server::deleteClient(std::vector<pollfd> &pollFds, std::vector<pollfd>::iterator &it, int clientFd) {
    std::cout << SERVER_DISCONNECT_CLIENT << clientFd << std::endl;

	// クライアントが参加しているチャンネルからクライアントを削除
	for (std::map<std::string, Channel>::iterator it = _channelList.begin(); it != _channelList.end(); ++it) {
		if (it->second.isClientInChannel(clientFd))
			it->second.removeClient(clientFd);
	}

	_clientList.erase(clientFd);
	close(clientFd);
	pollFds.erase(it);

	std::cout << SERVER_NUMBER_OF_CLIENTS << pollFds.size() - 1 << std::endl;
}

// クライアントにデータを送信する
void Server::sendServerReply(int clientFd, std::string &message) {
    std::istringstream iss(message);
	std::string line;

    // クライアントにデータを送信
	send(clientFd, message.c_str(), message.size(), 0);
    // 送信したデータを表示
	while (getline(iss, line))
		std::cout << "[Server] " << clientFd << " > " << line << std::endl; 
}

// 以下からチャンネル関連
// チャンネルを作成し、チャンネルリストに追加
void Server::addChannel(std::string &channelName) {
    // すでにチャンネルが存在する場合、エラー文を出力して何もしないで処理終了
	std::map<std::string, Channel>::iterator it = _channelList.find(channelName);
	if (it != _channelList.end()) {
		std::cerr << ERROR_CHANNEL_EXIST << channelName << std::endl;
		return ;
	}
	
	// チャンネルを作成し、チャンネルリストに追加
	Channel channel(channelName);
	_channelList.insert(std::make_pair(channelName, channel));
}

// チャンネルにクライアントを追加
void Server::addClientToChannel(std::string &channelName, Client &client) {
    
}

// チャンネルが存在するか確認
bool Server::isChannelExist(std::string &channelName) {
    
}