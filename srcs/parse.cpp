/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 01:33:14 by urabex            #+#    #+#             */
/*   Updated: 2025/05/20 01:34:38 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Server.hpp"
#include "Command.hpp"

static void splitCommandLine(std::string &message, std::vector<std::string> &cmds) {
    
}

// 処理例
//   ":nick!user@host PRIVMSG #channel :Hello there\n"
// 　→ "PRIVMSG #channel :Hello there\n"
// 　→ "PRIVMSG" をコマンド名として取得
// 　→ ":nick!user@host " をプレフィックスとして取得
// 　→ "#channel :Hello there\n" をメッセージとして取得
// 　→ "PRIVMSG" を大文字に変換

static int parseCommand(std::string &cmdLine, s_ircCommand &cmdInfo) {
    
}
