/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:10:59 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 08:55:41 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// user user 127.0.0.1 :real name
static std::string getRealname(std::string &cmdLine, std::vector<std::string> &words) {
	// ":"以降を取得する、なければ3番目以降を結合して取得
	std::size_t pos = cmdLine.find(":");
	if (pos == std::string::npos) {
		std::string realName;
		for (std::size_t i = 3; i < words.size(); i++) {
			if (i != 3)
				realName += " ";
			realName += words[i];
		}
		return (realName);
	}
	return (cmdLine.substr(pos + 1));
}

static bool	isValid(std::string username, std::string realname) {
	if (username.empty() || (username.size() > USERNAMELEN) || realname.empty() || (realname.size() > REALNAMELEN))
		return (false);
	
	for (std::size_t i = 0; i < username.size(); i++) {
		if (!isalnum(username[i]))
			return (false);
	}
	for (std::size_t i = 0; i < realname.size(); i++) {
		if (!isalnum(realname[i]) && realname[i] != ' ')
			return (false);
	}
	return (true);
}

static bool checkAndGetArguments(Server *server, int clientFd, std::string &cmdLine , std::string &username, std::string &realname) {
	std::vector<std::string> words = splitMessage(cmdLine);
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	if (words.size() < 4) {
		errMessage = ERR_NEEDMOREPARAMS(nickname, "USER");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << USER_USAGE << std::endl;
		return (false);
	}

	// ユーザー名と実名の取得
	username = words[0];
	realname = getRealname(cmdLine, words);

	// ユーザー名と実名の妥当性チェック
	if (!isValid(username, realname)) {
		errMessage = ERR_INVALID_PARM;
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << USER_REQUIREMENTS << std::endl;
		return (false);
	}
	return (true);
}

void user(Server *server, const int clientFd,s_ircCommand cmdInfo) {
	Client &client = retrieveClient(server, clientFd);
	std::string clientNick = client.getNickname();
	std::string errMessage = "";

	if (!client.getUserName().empty()) {
		errMessage = ERR_ALREADYREGISTERED(clientNick);
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}

	std::string username = "";
	std::string realname = "";

	if (!checkAndGetArguments(server, clientFd, cmdInfo.message, username, realname))
		return ;

	// ユーザー名と実名設定
	client.setUserName(username);
	client.setRealName(realname);
	client.incrementNmInfo();

	// 成功通知送信
	std::string notice = RPL_USER(IRC_PREFIX(clientNick, username), username, realname);
	addToClientSendBuf(server, clientFd, notice);
}
