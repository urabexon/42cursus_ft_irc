/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:59:59 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 20:57:39 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySet(Channel &channel, bool sign, const std::string &key) {
	if (sign && channel.getMode("k") == sign && channel.getPassword() == key) {
		std::string msg = "Channel key mode is already ";
		msg += (sign ? ("set to " + key + "\r\n") : "OFF\r\n");
		std::cout << msg << std::endl;
		return true;
	}
	return false;
}

static bool isValidKey(const std::string &key) {
	if (key.empty() || key.size() > 20)
		return false;
	for (size_t i = 0; i < key.size(); i++) {
		if (!isalnum(key[i]))
			return false;
	}
	return true;
}

static void broadcastModeChange(Server *server, Channel &channel, Client &client, bool sign, const std::string &key) {
	std::string modeString = (sign ? ("+k " + key) : "-k");
	std::string notify = RPL_MODE(IRC_PREFIX(client.getNickname(), client.getUserName()), channel.getName(), modeString);
	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
		addToClientSendBuf(server, it->second.getClientFd(), notify);
}

void channelKeyMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs) {
	// 既に同じキー状態なら、通知して処理終了
	if (isAlreadySet(channel, sign, modeArgs))
		return;
	// +k（有効化）の場合、キーの検証を行う
	if (sign) {
		if (!isValidKey(modeArgs)) {
			std::string errMessege = ERR_INVALIDKEY(client.getNickname(), channel.getName());
			addToClientSendBuf(server, client.getClientFd(), errMessege);
			std::cout << MODE_REQ_K_PASS << std::endl;
			return;
		}
		// キーが有効なら、チャンネルのキー情報を更新
		channel.setMode(sign, 'k');
		channel.setPassword(modeArgs);
	} else {
		channel.setMode(sign, 'k');
		channel.setPassword("");
	}

	// 変更内容をチャンネル全体に通知
	broadcastModeChange(server, channel, client, sign, modeArgs);
}
