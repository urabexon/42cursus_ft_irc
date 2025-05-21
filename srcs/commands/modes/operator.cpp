/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:00:04 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 21:17:01 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySetOperator(Server *server, Channel &channel, Client &client, bool sign, const std::string &targetNick) {
	// 対象ユーザーのFDを取得
	int targetFd = server->getClientFdByNick(const_cast<std::string&>(targetNick));
	if (targetFd == -1) {
		// 対象ユーザーが存在しない場合
		addToClientSendBuf(server, client.getClientFd(), ERR_NOSUCHNICK(client.getNickname(), targetNick));
		return (false);
	}
	// 既に対象ユーザーがオペレーター状態であれば、同じ状態の変更は不要
	if (channel.isOperator(targetFd) == sign) {
		std::string msg = (targetNick + " is already ");
		msg += (sign ? "an operator.\r\n" : "not an operator.\r\n");
		std::cout << msg << std::endl;
		return (true);
	}
	return (false);
}

static void broadcastOperatorModeChange(Server *server, Channel &channel, Client &client, bool sign, const std::string &targetNick) {
	std::string modeString = (sign ? "+o": "-o");
	std::string notify = RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modeString, targetNick);
	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
		addToClientSendBuf(server, it->second.getClientFd(), notify);
}

void operatorMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs) {
	// 既に同じ状態なら、通知して処理終了
	if (isAlreadySetOperator(server, channel, client, sign, modeArgs))
		return;
	// 対象ユーザーのFDを取得
	int targetFd = server->getClientFdByNick(modeArgs);
	if (targetFd == -1) {
		// 対象ユーザーが存在しない場合
		addToClientSendBuf(server, client.getClientFd(), ERR_NOSUCHNICK(client.getNickname(), modeArgs));
		return;
	}
	// +oの場合：対象ユーザーをオペレーターリストに追加
	if (sign)
		channel.addOperatorList(targetFd);
	else
		channel.removeOperator(targetFd);

	// 変更内容をチャンネル全体に通知
	broadcastOperatorModeChange(server, channel, client, sign, modeArgs);
}
