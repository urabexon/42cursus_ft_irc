/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumerics.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:18:00 by urabex            #+#    #+#             */
/*   Updated: 2025/05/19 21:32:19 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCNUMERICS_HPP
#define IRCNUMERICS_HPP

// Length of parameters
#define USERNAMELEN 20
#define REALNAMELEN 20
#define NICKLEN 10
#define CHANNELLEN 20
#define KEYLEN 10
#define TOPICLEN 400
#define KICKLEN 400
#define PARTLEN 400
#define MSGLEN 400
#define QUITLEN 400

// Error message
#define ERR_INVALID_PARM		"Error: Invalid parameters.\r\n"
#define ERR_PASS_AUTH_YET		"Error: Password authentication is not complete. Please \"PASS\".\r\n"

// Command usage and requirements
#define NICK_USAGE			    "Usage: NICK <nickname>\r\n"
#define NICK_REQUIREMENTS  	    "[!] nickname requirements: len(10), char(a-z, A-Z, 0-9)\r\n"
#define USER_USAGE		    	"Usage: USER <username> 0 * <realname>\r\n"
#define USER_REQUIREMENTS  	    "[!] username requirements: len(20), char(a-z, A-Z, 0-9)\r\n    realname requirements: len(20), char(a-z, A-Z, 0-9, ' ')\r\n"
#define PASS_USAGE		    	"Usage: PASS <password>\r\n"
#define JOIN_USAGE		    	"Usage: JOIN <channel> [key]\r\n"
#define JOIN_REQUIREMENTS  	    "[!] channel requirements: len(20), char(a-z, A-Z, 0-9)\r\n    key requirements: len(10), char(a-z, A-Z, 0-9)\r\n"
#define TOPIC_USAGE	    	    "Usage: TOPIC <channel> [<topic>]   If <topic> is not given, display topic\r\n"
#define TOPIC_REQUIREMENTS 	    "[!] topic requirements: len(400), char(a-z, A-Z, 0-9)\r\n"
#define INVITE_USAGE	    	"Usage: INVITE <nickname> <channel>\r\n"
#define KICK_USAGE		    	"Usage: KICK <channel> <nickname> [<comment>]\r\n"
#define KICK_REQUIREMENTS  	    "[!] comment requirements: len(400)\r\n"
#define MODE_USAGE		    	"MODE <channel> [<modestring> [<mode arguments>]]   If <modestring> is not given, display mode\r\n"
#define MODE_USAGE_K_O_L   	    "Usage: MODE <channel> +k <password>  |  +o <nickname>  |  -o <nickname>  |  +l <limit>\r\n"
#define MODE_REQUIREMENTS  	    "[!] modestring requirements: option sign( + , - ) + frag( i, t, k, o, l )\r\n"
#define MODE_REQ_K_PASS    	    "[!] password requirements: len(10), char(a-z, A-Z, 0-9)\r\n"
#define MODE_REQ_L_LIMIT   	    "[!] limit requirements: int(1-4)\r\n"
#define PRIVMSG_USAGE	    	"Usage: PRIVMSG <nickname/channel> <message>\r\n"
#define PRIVMSG_REQUIREMENTS    "[!] message requirements: len(400)\r\n"
#define PART_USAGE			    "Usage: PART <channel> [<reason>]\r\n"
#define PART_REQUIREMENTS	    "[!] reason requirements: len(400)\r\n"

// Success message
#define IRC_PREFIX(nickname, username) (":" + nickname + "!" + username + "@localhost")
#define RPL_PASS                "Password accepted\r\n"
#define RPL_USER(IRC_PREFIX, user, real) (IRC_PREFIX + " :Your username is " + user + ", realname is " + real + "\r\n")
#define RPL_NICK(IRC_PREFIX, newNick) (IRC_PREFIX + " NICK " + newNick + "\r\n")
#define RPL_JOIN(IRC_PREFIX, channel) (IRC_PREFIX + " JOIN :#" + channel + "\r\n")
#define RPL_TOPICSET(IRC_PREFIX, channel, topic) (IRC_PREFIX + " TOPIC #" + channel + " " + topic + "\r\n")
#define RPL_INVITE(IRC_PREFIX, target, channelName) (IRC_PREFIX + " INVITE " + target + " #" + channelName + "\r\n")
#define RPL_KICK(IRC_PREFIX, channel, target, comment) (IRC_PREFIX + " KICK #" + channel + " " + target + " " + comment + "\r\n")
#define RPL_MODE(IRC_PREFIX, channel, modestring) (IRC_PREFIX + " MODE #" + channel + " " + modestring + "\r\n")
#define RPL_PRIVMSG(IRC_PREFIX, target, message) (IRC_PREFIX + " PRIVMSG " + target + " " + message + "\r\n")
#define RPL_PART(IRC_PREFIX, channel, reason) (IRC_PREFIX + " PART #" + channel + " " + reason + "\r\n")
#define RPL_QUIT(IRC_PREFIX, reason) (IRC_PREFIX + " :QUIT:" + reason + "\r\n")

#define DEFAULT_KICK_COMMENT	":Kicked by channel operator"
#define DELIMITER_LINE          "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\r\n"

// クライアント登録直後のウェルカムメッセージ
#define RPL_WELCOME(client, nick) (":ircserv 001 " + client + " :Welcome to the Internet Relay Network, " + nick + "\r\n")
// 接続中のサーバ名とバージョン情報
#define RPL_YOURHOST(client, server, version) (":ircserv 002 " + client + " :Your host is " + server + ", running version " + version + "\r\n")
// サーバの作成日時情報
#define RPL_CREATED(client, datetime) (":ircserv 003 " + client + " :This server was created " + datetime + "\r\n")
// サーバ名とバージョン情報のみ
#define RPL_MYINFO(client, server, version) (":ircserv 004 " + client + " " + server + " " + version + "\r\n")
// チャネルの現在のモード情報
#define RPL_CHANNELMODEIS(client, channel, modestring, modeArgs) (":localhost 324 " + client + " " + channel + " " + modestring + " " + modeArgs + "\r\n")
// チャネルのトピック未設定状態
#define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set\r\n")
// チャネルの現在のトピック情報
#define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic + "\r\n")
// 招待リストに登録されているチャネル情報
#define RPL_INVITING(IRC_PREFIX, client, nick, channel) (IRC_PREFIX + " 341 " + client + " " + nick + " #" + channel + "\r\n")
// NAMES応答（チャネル参加者一覧）
#define RPL_NAMREPLY(client, symbol, channel, nicks) (":localhost 353 " + client + " " + symbol + " " + channel + " :" + nicks + "\r\n")
// NAMES応答の終了通知
#define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " " + channel + " :End of /NAMES list\r\n")

#endif
