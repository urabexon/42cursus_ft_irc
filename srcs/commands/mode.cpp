/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:10:49 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 12:40:25 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool checkAndGetChannel(Server *server, int const clientFd, std::vector<std::string> &words, std::string &channelName) {
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";
	int wordsSize = words.size();
	if (wordsSize == 0 || words[0].empty()) {
		errMessage = ERR_NEEDMOREPARAMS(nickname, "MODE");
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << MODE_USAGE << std::endl;
		return (false);
	}

	channelName = getChannelNameFromWord(words[0]);

	if (words[0].at(0) == '#') {
		// チャンネルが存在するか確認
		if (!server->isChannelExist(channelName)) {
			errMessage = ERR_NOSUCHCHANNEL(nickname, channelName);
			addToClientSendBuf(server, clientFd, errMessage);
			return (false);
		}
	} else if (server->isClientExist(channelName))
		return (false);

	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;

	if (!channel.isClientInChannel(clientFd)) {
		errMessage = ERR_NOTONCHANNEL(nickname, channelName);
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	return (true);
}

static bool checkModeArguments(Server *server, int const clientFd, Channel &channel, std::string &modeString, std::string &modeArgs) {
	std::string nickname = server->getNickname(clientFd);
	std::string errMessage = "";
	
	// クライアントがそのチャンネルのオペレーターであるか確認
	if (!channel.isOperator(clientFd)) {
		errMessage = ERR_CHANOPRIVSNEEDED(nickname, channel.getName());
		addToClientSendBuf(server, clientFd, errMessage);
		return (false);
	}

	// モード文字列が正しいか確認
	if (modeString.empty() || modeString.size() != 2 || ((modeString[0] != '+' && modeString[0] != '-')) || ((modeString[1] != 'i' && modeString[1] != 't' && modeString[1] != 'k' && modeString[1] != 'o' && modeString[1] != 'l'))) {
		errMessage = ERR_UNKNOWNMODE(nickname, modeString);
		addToClientSendBuf(server, clientFd, errMessage);
		std::cout << MODE_REQUIREMENTS << std::endl;
		return (false);
	}

	// <mode arguments> が必要なモード文字列の場合、引数があるか確認
	if (modeString == "+k" || modeString == "+o" || modeString == "-o" || modeString == "+l") {
		if (modeArgs.empty()) {
			errMessage = ERR_NEEDMOREPARAMS(nickname, "MODE");
			addToClientSendBuf(server, clientFd, errMessage);
			std::cout << MODE_USAGE_K_O_L << std::endl;
			return (false);
		}
	}
	return (true);
}

static std::string getChannelModeString(Channel &channel) {
	std::string modeString = "+";
	if (channel.getMode("i"))
		modeString += "i";
	if (channel.getMode("t"))
		modeString += "t";
	if (channel.getMode("k"))
		modeString += "k";
	if (channel.getMode("o"))
		modeString += "o";
	if (channel.getMode("l"))
		modeString += "l";
	if (modeString == "+")
		modeString = "all off";
	return (modeString);
}

static void infoChannelMode(Server *server, int const clientFd, Channel &channel, std::string channelName) {
	std::string modeInfo = "";
	std::string modeString = getChannelModeString(channel);

	modeInfo = RPL_CHANNELMODEIS(server->getNickname(clientFd), channelName, modeString, channel.getPassword());
	addToClientSendBuf(server, clientFd, modeInfo);

	// debug
	std::cout << "===== " + channelName + "'s mode =====" << std::endl
			  << " invite only   : "
			  << (channel.getMode("i") ? "ON" : "OFF") << std::endl
			  << " topic protect : "
			  << (channel.getMode("t") ? "ON" : "OFF") << std::endl
			  << " channel key   : "
			  << (channel.getMode("k") ? "ON" : "OFF") << std::endl
			  << " user limit    : "
			  << (channel.getMode("l") ? "ON" : "OFF") << std::endl
			  << " Client " << clientFd << " is "
			  << (channel.isOperator(clientFd) ? "an operator" : "not an operator") << std::endl;
}

static void processModeChanges(Server *server, Client &client, Channel &channel, std::string modeString, std::string modeArgs) {
	// モード変更の内容を取得
	bool sign = modeString[0] == '+';
	char mode = modeString[1];

	// モード変更の処理
	switch (mode) {
		case 'i':
			inviteOnlyMode(server, channel, client, sign);
			break;
		case 't':
			topicProtectMode(server, channel, client, sign);
			break;
		case 'k':
			channelKeyMode(server, channel, client, sign, modeArgs);
			break;
		case 'l':
			userLimitMode(server, channel, client, sign, modeArgs);
			break;
		case 'o':
			operatorMode(server, channel, client, sign, modeArgs);
			break;
		default:
			break;
	}
}

void mode(Server *server, int const clientFd, s_ircCommand cmdInfo) {
	std::string channelName = "";

	// 入力パラメータを空白で分割
	std::vector<std::string> words = splitMessage(cmdInfo.message);

	// 1つ目の引数をチャンネル名として取得
	if (!checkAndGetChannel(server, clientFd, words, channelName))
		return;

	std::map<std::string, Channel> &channelList = server->getChannelList();
	Channel &channel = channelList.find(channelName)->second;
	
	//「MODE #channelname」の場合、チャンネルのモード情報をクライアントに通知
	if (words.size() == 1) {
		// チャンネルのモード情報をクライアントに通知
		infoChannelMode(server, clientFd, channel, channelName);
		return;
	}

	// 追加パラメータ（mode arguments）を取得
	std::string modeString = words[1];
	std::string modeArgs = "";
	if (words.size() > 2)
		modeArgs = words[2];

	// モード文字列と引数の妥当性をチェック
	if (!checkModeArguments(server, clientFd, channel, modeString, modeArgs))
		return;

	// MODEコマンドの処理
	Client &client = retrieveClient(server, clientFd);
	processModeChanges(server, client, channel, modeString, modeArgs);
}
