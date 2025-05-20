/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:10:56 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 01:28:50 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static std::string getReason(std::string &cmdLine) {
    std::string reason = trim(cmdLine);
	if (reason.size() > QUITLEN) {
		reason = reason.substr(0, QUITLEN);
		reason += "[CUT]";
	}
	return ("Quit: " + reason);
}

// コマンド形式: QUIT [<reason>]
void quit(Server *server, int const clientFd, s_ircCommand cmdInfo) {
    Client &client = retrieveClient(server, clientFd);
    
    // 理由（reason）の取得
	std::string reason = getReason(cmdInfo.message);
	std::string notice = RPL_QUIT(IRC_PREFIX(client.getNickname(), client.getUserName()), reason);

	// QUIT通知を送信
	std::map<const int, Client>	&clientList = server->getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
		addToClientSendBuf(server, it->first, notice);
		
	// 発行者を全チャンネルから削除
	std::map<std::string, Channel> &channelList = server->getChannelList();
	for (std::map<std::string, Channel>::iterator it = channelList.begin(); it != channelList.end(); ++it) {
		if (it->second.isClientInChannel(clientFd))
			it->second.removeClient(clientFd);
	}

	client.setToDeconnect();
}
