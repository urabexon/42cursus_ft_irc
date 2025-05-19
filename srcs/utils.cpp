/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:22:13 by urabex            #+#    #+#             */
/*   Updated: 2025/05/19 22:30:26 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Server.hpp"

// クライアントの送信バッファにメッセージを追加する関数
void addToClientSendBuf(Server *server, int clientFd, std::string message) {
    
}

// 指定されたFDに対応するクライアントを取得する関数
Client &retrieveClient(Server *server, int clientFd) {
    
}

// メッセージ文字列からチャンネル名を抽出する関数
std::string getChannelName(std::string &msgToParse) {

}

// 文字列の先頭と末尾の空白（スペース、タブ）を除去する
std::string trim(const std::string &s) {
    
}

// どの種類のIRCコマンドか判定、コマンドタイプの番号を返す
int getCommandType(std::string &cmd) {
    int	type = 0;

    std::string	cmdList[NUM_OF_CMD] = {
		"CAP", "INVITE", "JOIN", "KICK", "MODE", "NICK",
		"PART", "PASS", "PING", "PRIVMSG", "QUIT", "TOPIC", "USER"
	};
    
    // コマンドリストからコマンドを検索
    while (type < NUM_OF_CMD) {
		if (cmd == cmdList[type])
			break;
		type++;
	}
	return (type + 1);
}
