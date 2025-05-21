/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limit.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:00:01 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 21:01:40 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static bool isAlreadySetLimit(Channel &channel, bool sign, const std::string &limitStr) {
	// 文字列を整数に変換
	std::istringstream iss(limitStr);
	int limit;
	iss >> limit;
	// 既に+lモードが有効で、かつ現在の制限値が同じ場合は通知してtrueを返す
	if (channel.getMode("l") == sign && channel.getCapacity() == static_cast<size_t>(limit)) {
		std::string msg = "User limit mode is already ";
		msg += (sign ? ("set to " + limitStr + "\r\n") : "OFF\r\n");
		std::cout << msg << std::endl;
		return true;
	}
	return false;
}

static bool isValidLimit(const std::string &limitStr) {
	if (limitStr.empty())
		return false;
	for (size_t i = 0; i < limitStr.size(); i++) {
		if (!isdigit(limitStr[i]) || limitStr[0] == '+')
			return false;
	}
	// 文字列を整数に変換
	std::istringstream iss(limitStr);
	int limit;
	iss >> limit;
	if (iss.fail())
		return false;
	// 要件：1～MAX_CLIENTSの整数であること
	if (limit < 1 || limit > MAX_CLIENTS)
		return false;
	return true;
}

static void broadcastLimitModeChange(Server *server, Channel &channel, Client &client, bool sign, const std::string &limitStr) {
	std::string modeString = (sign ? ("+l " + limitStr) : "-l");
	std::string notify = RPL_MODE(IRC_PREFIX(client.getNickname(), client.getUserName()), channel.getName(), modeString);
	std::map<const int, Client> &clientList = channel.getClientList();
	for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it)
		addToClientSendBuf(server, it->second.getClientFd(), notify);
}

void userLimitMode(Server *server, Channel &channel, Client &client, bool sign, std::string modeArgs) {
	if (!isValidLimit(modeArgs)) {
		std::cout << MODE_REQ_L_LIMIT << std::endl;
		return;
	}
	// 既に同じ制限状態なら、通知して処理終了
	if (isAlreadySetLimit(channel, sign, modeArgs))
		return;
	// +l（有効化）の場合、limitの検証を行う
	if (sign) {
		int limit = std::atoi(modeArgs.c_str());
		// 制限人数を更新（例：channel.setCapacity()でユーザー数制限を設定）
		channel.setMode(sign, 'l');
		channel.setCapacity(limit);
	} else {
		channel.setMode(sign, 'l');
		// 制限解除時は、0に戻す
		channel.setCapacity(0);
	}

	// 変更内容をチャンネル全体に通知
	broadcastLimitModeChange(server, channel, client, sign, modeArgs);
}
