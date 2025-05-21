/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:10:48 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 12:47:38 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkAndGetArguments(Server *server, int clientFd, std::string &issuerNick, std::string &argument, std::string &channelName, std::string &targetNick, std::string &comment) {
	std::string errMessage = "";
	if (argument.empty() || argument.find(' ') == std::string::npos) {
		errMessage = ERR_NEEDMOREPARAMS(issuerNick, "KICK");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << KICK_USAGE << std::endl;
		return (false);
	}

	// チャンネル名とユーザー名とコメントを取得
	size_t pos = argument.find(' ');
	channelName = argument.substr(0, pos);
	channelName = getChannelNameFromWord(channelName);
	argument.erase(0, pos + 1);
	getTargetAndText(argument, targetNick, comment);

	if (comment.empty()) {
		comment = DEFAULT_KICK_COMMENT;
	} else if (comment.size() > KICKLEN) {
		comment = comment.substr(0, KICKLEN);
		comment += "[CUT]";
	}
	return (true);
}

static bool checkKickEligibility(Server *server, int const clientFd, std::string &issuerNick, std::string &channelName, std::string &targetNick) {
	std::string errMessage = "";

	// 対象チャンネルが存在することを確認
	if (!server->isChannelExist(channelName)) {
		errMessage = ERR_NOSUCHCHANNEL(issuerNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;

	// 対象クライアントが存在することを確認
	if (!server->isClientExist(targetNick)) {
		errMessage = ERR_NOSUCHNICK(issuerNick, targetNick);
		addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(issuerNick, targetNick));
		return (false);
	}

	// 発行者が対象チャンネルに参加していることを確認
	if (!channel.isClientInChannel(clientFd)) {
		errMessage = ERR_NOTONCHANNEL(issuerNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 対象ユーザーがチャンネルに参加していることを確認
	int targetFd = server->getClientFdByNick(targetNick);
	if (!channel.isClientInChannel(targetFd)) {
		errMessage = ERR_USERNOTINCHANNEL(issuerNick, targetNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 発行者がチャンネルオペレーターであるか確認
	if (!channel.isOperator(clientFd)) {
		errMessage = ERR_CHANOPRIVSNEEDED(issuerNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	return (true);
}

static void broadcastKick(Server *server, Channel &channel, Client &client, std::string &issuerNick, std::string &target, std::string &comment) {
	// チャンネルメンバー全員に追放を通知
	std::map<const int, Client> &clientList = channel.getClientList();
	std::string notice = RPL_KICK(IRC_PREFIX(issuerNick, client.getUserName()), channel.getName(), target, comment);

	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
		addToClientSendBuf(server, it->second.getClientFd(), notice);
}

void kick(Server *server, int const clientFd, s_ircCommand cmdInfo) {
	Client &client = retrieveClient(server, clientFd);
	std::string issuerNick = client.getNickname();
	std::string channelName = "";
	std::string targetNick = "";
	std::string comment = "";

	// ユーザー入力から引数を取得し、引数をチェック
	if (!checkAndGetArguments(server, clientFd, issuerNick, cmdInfo.message, channelName, targetNick, comment))
		return ;

	// パラメータの妥当性を確認する
	if (!checkKickEligibility(server, clientFd, issuerNick, channelName, targetNick))
		return ;

	// チャンネルメンバー全員に追放を通知
	std::map<std::string, Channel> &channels = server->getChannelList();
	Channel &channel = channels.find(channelName)->second;
	broadcastKick(server, channel, client, issuerNick, targetNick, comment);
	
	// 対象ユーザーをチャンネルから削除する
	int targetFd = server->getClientFdByNick(targetNick);
	channel.removeClient(targetFd);
}
