/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 01:33:14 by urabex            #+#    #+#             */
/*   Updated: 2025/05/20 09:55:53 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Server.hpp"
#include "Command.hpp"

static void splitCommandLine(std::string &message, std::vector<std::string> &cmds) {
    // 改行コードを"\r\n"から"\n"に変換
	std::string::size_type pos = 0;
    while ((pos = message.find("\r\n", pos)) != std::string::npos) {
		message.replace(pos, 2, "\n");
		pos += 1;
	}
    // メッセージを１行ずつ分割してコマンドリストに格納
	std::istringstream	iss(message);
	std::string			line;
    while (std::getline(iss, line, '\n'))
		cmds.push_back(line);
}

// 処理例
//   ":nick!user@host PRIVMSG #channel :Hello there\n"
// 　→ "PRIVMSG #channel :Hello there\n"
// 　→ "PRIVMSG" をコマンド名として取得
// 　→ ":nick!user@host " をプレフィックスとして取得
// 　→ "#channel :Hello there\n" をメッセージとして取得
// 　→ "PRIVMSG" を大文字に変換

static int parseCommand(std::string &cmdLine, s_ircCommand &cmdInfo) {
    // コマンドが空の場合エラー
	cmdLine = trim(cmdLine);
    if (cmdLine.empty()) {
		std::cerr << ERROR_SERVER_EMPTY_CMD << std::endl;
		return (EXIT_FAILURE);
	}

    // コマンドの先頭が":"の場合、コマンドの先頭までを取り除く
    std::string	copy = cmdLine;
    if (copy[0] == ':') {
		if (cmdLine.find(' ') != std::string::npos)
			copy.erase(0, copy.find(' ') + 1);
	}

    // コマンド名を取得(スペースがない場合、全体がコマンド名、ある場合は先頭からスペースまで)
    std::string::size_type firstSpace = copy.find(' ');
	if (firstSpace == std::string::npos)
		cmdInfo.name.assign(copy);
	else
		cmdInfo.name.assign(copy, 0, firstSpace);

    // プレフィックスを取得
    std::string::size_type commandPos = cmdLine.find(cmdInfo.name);
    if (commandPos != std::string::npos)
		cmdInfo.prefix.assign(cmdLine, 0, commandPos);
	else
		cmdInfo.prefix = "";

    // メッセージ部分取得
    std::string::size_type msgPos = commandPos + cmdInfo.name.size();
	if (msgPos < cmdLine.size()) {
		cmdInfo.message = cmdLine.substr(msgPos);
		cmdInfo.message = trim(cmdInfo.message);
	}
	else
		cmdInfo.message = "";
    
    // コマンド名を大文字に変換
    for (std::string::size_type i = 0; i < cmdInfo.name.size(); ++i)
		cmdInfo.name[i] = std::toupper(cmdInfo.name[i]);
    return (EXIT_SUCCESS);
}

static void sendClientRegistrationMsg(Server *server, int clientFd, Client *client) {
    addToClientSendBuf(server, clientFd, DELIMITER_LINE);
	addToClientSendBuf(server, clientFd, RPL_WELCOME(client->getNickname(), client->getNickname()));
	addToClientSendBuf(server, clientFd, RPL_YOURHOST(client->getNickname(), "ircserv", "1.0"));
	addToClientSendBuf(server, clientFd, RPL_CREATED(client->getNickname(), server->getDateTime()));
	addToClientSendBuf(server, clientFd, RPL_MYINFO(client->getNickname(), "ircserv", "1.0"));
	addToClientSendBuf(server, clientFd, DELIMITER_LINE);
}

void Server::fillClientInfo(Client *client, int clientFd, s_ircCommand cmdInfo) {
    // irssiからの"CAP LS"リクエストを無視
	if (cmdInfo.name == "CAP" && cmdInfo.message.find("LS") != std::string::npos) {
		// CAP LS に対して空の応答を返す
		addToClientSendBuf(this, clientFd, "CAP * LS :\r\n");
		return;
	}

    // パスワード認証が完了していない場合
	if (!client->getConnexionPassword()) {
		if (cmdInfo.name != "PASS")
			addToClientSendBuf(this, clientFd, ERR_PASS_AUTH_YET);
		else
			pass(this, clientFd, cmdInfo);
		return ;
	} else {
        // クライアント情報が全て揃っていない場合、クライアント情報を取得する
		if (cmdInfo.name != "NICK" && cmdInfo.name != "USER") {
            addToClientSendBuf(this, clientFd, ERR_NOTREGISTERED);
        } else {
            if (cmdInfo.name == "NICK")
				nick(this, clientFd, cmdInfo);
			else if (cmdInfo.name == "USER")
				user(this, clientFd, cmdInfo);
        }
        // クライアント情報が全て揃った場合、登録処理を行う
        if (client->getNmInfo() == 3 && client->isRegistrationDone() == false) {
			sendClientRegistrationMsg(this, clientFd, client);
			client->setRegistrationDone();
		}
		return ;
    }
}

void Server::execCommand(int clientFd, s_ircCommand	&cmdInfo) {
    
}

void Server::parseExecCommand(int clientFd, std::string &message) {
    
}
