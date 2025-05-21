/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:10:46 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 13:44:57 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool	isValid(std::string &channelName, std::string &key) {
	if (channelName.empty() || channelName.size() > 20 || key.size() > 10)
		return (false);
	for (std::size_t i = 0; i < channelName.size(); i++) {
		if (!isalnum(channelName[i]))
			return (false);
	}
	for (std::size_t i = 0; i < key.size(); i++) {
		if (!isalnum(key[i]))
			return (false);
	}
	return (true);
}

static bool checkAndGetArguments(Server *server, int clientFd, std::vector<std::string> &words, std::string &nickname, std::string &channelName, std::string &key) {
	std::string errMessage = "";

	if (words.empty() || words[0].empty()) {
		errMessage = ERR_NEEDMOREPARAMS(nickname, "JOIN");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << JOIN_USAGE << std::endl;
		return (false);
	}

	if (words.size() > 2)
		std::cout << JOIN_USAGE << std::endl;
	
	// チャンネル名とキーを取得
	channelName = getChannelNameFromWord(words[0]);
	if (words.size() > 1)
		key = words[1];

	// チャンネル名とキーの妥当性をチェック
	if (!isValid(channelName, key)) {
		errMessage = ERR_INVALIDKEY(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << JOIN_REQUIREMENTS << std::endl;
		return (false);
	}
	return (true);
}

static bool checkJoinEligibility(Server *server, int clientFd, std::string &nickname, Channel &channel, std::string key) {
	std::string channelName = channel.getName();
	std::string errMessage = "";

	if (channel.isClientInChannel(clientFd)) {
		errMessage = ERR_USERONCHANNEL(nickname, nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// パスワード保護(mode:k)されている場合、キーの一致を確認する
	if (channel.getMode("key")) {
		if (key != channel.getPassword()) {
			errMessage = ERR_BADCHANNELKEY(nickname, channelName);
			addToClientSendBuf(server, clientFd, errMessage);
			return (false);
		}
	}
	
	// 招待制(mode:i)されている場合、はJOINできない
	if (channel.getMode("invite") && !channel.isInvited(clientFd)) {
		errMessage = ERR_INVITEONLYCHAN(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// 参加人数制限チェック
	if (channel.getMode("limit") && channel.getClientList().size() >= channel.getCapacity()) {
		errMessage = ERR_CHANNELISFULL(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	return (true);
}

static void broadcastJoin(Server *server, Channel &channel, std::string &nickname, std::string &userName, std::string &channelName) {
	std::string notice = RPL_JOIN(IRC_PREFIX(nickname, userName), channelName);

	// チャンネル全体にJOINメッセージを送信
	std::map<const int, Client>	&clientList = channel.getClientList();
	for (std::map<const int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
		addToClientSendBuf(server, it->first, notice);
	// 新メンバーにトピック情報を送信
	if (!channel.getTopic().empty()) {
		notice = RPL_TOPIC(nickname, channelName, channel.getTopic());
		addToClientSendBuf(server, server->getClientFdByNick(nickname) , notice);
	}

	// 新メンバーに参加者一覧を送信
	notice = RPL_NAMREPLY(nickname, "=", channelName, channel.getClientListString());
	notice += RPL_ENDOFNAMES(nickname, channelName);
	addToClientSendBuf(server, server->getClientFdByNick(nickname) , notice);
}

void join(Server *server, const int clientFd, s_ircCommand cmdInfo) {
	Client &client = retrieveClient(server, clientFd);
	std::string nickname = client.getNickname();
	std::string channelName = "";
	std::string key = "";

	// ユーザー入力をスペース区切りで取得し、引数の数をチェック
	std::vector<std::string> words = splitMessage(cmdInfo.message);
	if (!checkAndGetArguments(server, clientFd, words, nickname, channelName, key))
		return;

	// サーバーのチャンネルリストから対象チャンネルを探す
	std::map<std::string, Channel> &channelList = server->getChannelList();
	std::map<std::string, Channel>::iterator it = channelList.find(channelName);

	// 存在しない場合は新規作成
	if (it == channelList.end()) {
		// チャンネルを作成
		server->addChannel(channelName);
		it = channelList.find(channelName);

		// チャンネルの初期設定（オペレーター、パスワード等）
		it->second.addOperatorList(clientFd);
		if (key != "") {
			it->second.setMode(true, 'k');
			it->second.setPassword(key);
		}
	} else {
		if (!checkJoinEligibility(server, clientFd, nickname, it->second, key))
			return ;
	}

	// クライアントをチャンネルに追加
	server->addClientToChannel(channelName, client);
	if (it->second.isInvited(clientFd))
		it->second.removeInvited(clientFd);

	// チャンネル参加後、JOINメッセージ、トピック、参加者一覧などの情報を送信
	broadcastJoin(server, it->second, nickname, client.getUserName(), channelName);
}
