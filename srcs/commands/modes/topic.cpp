/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:00:03 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 21:23:57 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySet(Channel &channel, bool sign) {
	// 既にトピック保護モードが設定されている場合、通知を送ってtrueを返す
	if (channel.getMode("t") == sign) {
		std::string msg = "Topic protect mode is already ";
		msg += (sign ? "ON\r\n" : "OFF\r\n");
		std::cout << msg << std::endl;
		return true;
	}
	return false;
}

static void broadcastModeChange(Server *server, Channel &channel, Client &client, bool sign) {
	std::string modeString = (sign ? "+t" : "-t");
	std::string notify = RPL_MODE(IRC_PREFIX(client.getNickname(), client.getUserName()), channel.getName(), modeString);
	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
		addToClientSendBuf(server, it->second.getClientFd(), notify);
}

void topicProtectMode(Server *server, Channel &channel, Client &client, bool sign) {
	// 現在の状態をチェック
	if (isAlreadySet(channel, sign))
		return;
	// モードを変更
	channel.setMode(sign, 't');
	// モード変更を全員に通知
	broadcastModeChange(server, channel, client, sign);
}
