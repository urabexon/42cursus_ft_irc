/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:10:45 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 13:56:00 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// INVITEコマンドの引数数チェック
static bool checkArguments(Server *server, int clientFd, std::vector<std::string> &words) {
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";

	if (words.size() < 2 || words[0].empty() || words[1].empty()) {
		errMessage = ERR_NEEDMOREPARAMS(nickname, "INVITE");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << INVITE_USAGE << std::endl;
		return (false);
	}
	if (words.size() > 2)
		std::cout << INVITE_USAGE << std::endl;
	return (true);
}

static bool checkInviteEligibility(Server *server, int const clientFd, std::string &inviterNick, std::string &targetNick, std::string &channelName) {
	std::string errMessage = "";

	if (!server->isChannelExist(channelName)) {
		errMessage = ERR_NOSUCHCHANNEL(inviterNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;

	// 招待したユーザーが対象チャンネルに参加していることを確認
	if (!channel.isClientInChannel(clientFd)) {
		errMessage = ERR_NOTONCHANNEL(inviterNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 招待制チャンネルの場合、招待したユーザーがオペレーターであることを確認
	if (channel.getMode("invite") && !channel.isOperator(clientFd)) {
		errMessage = ERR_CHANOPRIVSNEEDED(inviterNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 対象クライアントが存在することを確認
	if (!server->isClientExist(targetNick)) {
		errMessage = ERR_NOSUCHNICK(inviterNick, targetNick);
		addToClientSendBuf(server, clientFd, ERR_NOSUCHNICK(inviterNick, targetNick));
		return (false);
	}

	// InviterとTargetが同一でないことを確認
	if (inviterNick == targetNick) {
		errMessage = ERR_ERRONEUSTARGET(inviterNick, targetNick, " (Inviter and target are the same)");
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 招待対象のユーザーが既にチャンネルに参加していることを確認
	int targetFd = server->getClientFdByNick(targetNick);
	if (channel.isClientInChannel(targetFd)) {
		errMessage = ERR_USERONCHANNEL(inviterNick, targetNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// チャンネルが招待制限されている場合、招待対象のユーザーがチャンネルに参加できることを確認
	if (channel.getMode("limit") && channel.getClientList().size() >= channel.getCapacity()) {
		errMessage = ERR_CHANNELISFULL(inviterNick, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	return (true);
}

void invite(Server *server, int const clientFd, s_ircCommand cmdInfo) {
	Client &client = retrieveClient(server, clientFd);
	std::string inviterNick = client.getNickname();

	// 入力パラメータの分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	if (!checkArguments(server, clientFd, words))
		return ;

	// パラメータから招待対象のニックネームとチャンネル名を取得する
	std::string targetNick = words[0];
	std::string channelName = getChannelNameFromWord(words[1]);

	// パラメータの妥当性を確認する
	if (!checkInviteEligibility(server, clientFd, inviterNick, targetNick, channelName))
		return ;
		
	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;

	// 招待対象のユーザーを招待リストに追加する（すでに登録している場合は追加しない）
	int targetFd = server->getClientFdByNick(targetNick);
	if (!channel.isInvited(clientFd))
		channel.addInvitedList(targetFd);

	// 招待したユーザーに成功通知の送信、招待対象のユーザーに招待通知を送信する
	std::string notice = RPL_INVITE(IRC_PREFIX(inviterNick, client.getUserName()), targetNick, channelName);
	std::string invitation = RPL_INVITING(IRC_PREFIX(inviterNick, client.getUserName()), inviterNick, targetNick, channelName);
	addToClientSendBuf(server, clientFd, notice);
	addToClientSendBuf(server, targetFd, invitation);
}
