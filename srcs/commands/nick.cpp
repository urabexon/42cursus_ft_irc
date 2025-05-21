/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:10:51 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 10:00:10 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool	isValid(std::string nickname) {
	if ((nickname.size() > 10))
		return (false);
	for (std::size_t i = 0; i < nickname.size(); i++) {
		if (!isalnum(nickname[i]))
			return (false);
	}
	return (true);
}

static bool	isAlreadyUsed(Server *server, const int clientFd, std::string nickname) {
	std::map<int, Client>::iterator it = server->getClientList().begin();
	for (; it != server->getClientList().end(); ++it) {
		if (it->first != clientFd && it->second.getNickname() == nickname)
			return (true);
	}
	return (false);
}

static bool checkAndGetArguments(Server *server, const int clientFd, std::vector<std::string> &words, std::string &newNick) {
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";
	if (words.empty() || words[0].empty()) {
		errMessage = ERR_NONICKNAMEGIVEN(nickname);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	// 引数が複数ある場合
	if (words.size() > 1)
		std::cout << NICK_USAGE << std::endl;
	// 不正なニックネームの場合
	newNick = words[0];
	if (!isValid(newNick)) {
		errMessage = ERR_ERRONEUSNICKNAME(nickname, newNick);
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << NICK_REQUIREMENTS << std::endl;
		return (false);
	}
	// 重複チェック（他のユーザーが既に使用しているか）
	if (isAlreadyUsed(server, clientFd, newNick)) {
		errMessage = ERR_NICKNAMEINUSE(nickname, newNick);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	return (true);
}

void nick(Server *server, const int clientFd, s_ircCommand cmdInfo) {
	Client &client = retrieveClient(server, clientFd);
	std::string oldNick = client.getOldNickname();
	std::string newNick = "";

	// ユーザー入力をスペースで分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	// 引数の数が正しいかチェック（ニックネームの文字数、文字種）
	if (!checkAndGetArguments(server, clientFd, words, newNick))
		return ;

	if (client.getNickname().empty()) {
        // 未設定の場合は新しいニックネームを設定
		client.setNickname(newNick);
		client.incrementNmInfo();
	} else {
        // 現在のニックネームを旧ニックネームとして保持し、新しいニックネームに更新
		oldNick = client.getNickname();
		client.setOldNickname(oldNick);
		client.setNickname(newNick);
	}

	// broadcastNick(server, client.getUserName(), oldNick, newNick);
	addToClientSendBuf(server, clientFd, RPL_NICK(IRC_PREFIX(oldNick, client.getUserName()), newNick));
}
