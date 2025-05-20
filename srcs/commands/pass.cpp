/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:10:54 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 01:20:15 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkArguments(Server *server, const int clientFd, std::vector<std::string> &words) {
    std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	// 引数が1つであることを確認
	if (words.size() != 1 || words[0].empty()) {
		errMessage = ERR_NEEDMOREPARAMS(nickname, "PASS");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << PASS_USAGE << std::endl;
		return (false);
	}

	// パスワードが一致しない場合
	if (words[0] != server->getPassword()) {
		errMessage = ERR_PASSWDMISMATCH(nickname);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}
	return (true);  
}

void pass(Server *server, const int clientFd,s_ircCommand cmdInfo) {
    Client &client = retrieveClient(server, clientFd);
	std::string clientNick = client.getNickname();
	std::string errMessage = "";

	// 既にパスワード認証が成功している場合、エラーを返す
	if (client.getConnexionPassword()) {
		errMessage = ERR_ALREADYREGISTERED(clientNick);
		addToClientSendBuf(server, clientFd, errMessage);
		return ;
	}

	std::vector<std::string> words = splitMessage(cmdInfo.message);

	// 入力内容の妥当性チェック
	if (!checkArguments(server, clientFd, words))
		return ;

	// パスワード認証成功
	client.setConnexionPassword();
	client.incrementNmInfo();

	// 成功通知の送信
	addToClientSendBuf(server, clientFd, RPL_PASS);
}
