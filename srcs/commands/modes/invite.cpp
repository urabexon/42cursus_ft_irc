/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:59:57 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 15:36:25 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySet(Channel &channel, bool sign) {
	// 既に指定されたモードが設定されている場合はエラー
	if (channel.getMode("i") == sign) {
		std::string msg = "Invite-only mode is already ";
		msg += (sign ? "ON\r\n" : "OFF\r\n");
		std::cout << msg << std::endl;
		return (true);
	}
	return (false);
}

static void broadcastModeChange(Server *server, Channel &channel, Client &client, bool sign) {
	std::string modeString = (sign ? "+i" : "-i");
	std::string notify = RPL_MODE(IRC_PREFIX(client.getNickname(), client.getUserName()), channel.getName(), modeString);
	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
		addToClientSendBuf(server, it->second.getClientFd(), notify);
}

void inviteOnlyMode(Server *server, Channel &channel, Client &client, bool sign) {
	// 現在の状態をチェック
	if (isAlreadySet(channel, sign))
		return;
	// モード変更
	channel.setMode(sign, 'i');
	// モード変更を全員に通知
	broadcastModeChange(server, channel, client, sign);
}
