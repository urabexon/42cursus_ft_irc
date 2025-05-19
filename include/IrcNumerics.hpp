/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumerics.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:18:00 by urabex            #+#    #+#             */
/*   Updated: 2025/05/19 21:39:27 by urabex           ###   ########.fr       */
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
// 存在しないニックまたはチャネル
#define ERR_NOSUCHNICK(client, nickname) (":localhost 401 " + client + " " + nickname + " :No such nick/channel\r\n")
// 存在しないチャネル名
#define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " #" + channel + " :No such channel\r\n")
// 宛先が指定されていない（コマンド名付き）
#define ERR_NORECIPIENT(client, command) (":localhost 411 " + client + " :No recipient given (" + command + ")\r\n")
// 送信するテキストが指定されていない
#define ERR_NOTEXTTOSEND(client) (":localhost 412 " + client + " :No text to send\r\n")
// 未知のコマンド
#define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client + " " + command + " :Unknown command\r\n")

// MOTDファイルが存在しない
// #define ERR_NOMOTD(client) (":localhost 422 " + client + " :MOTD File is missing\r\n")

// ニックネームが指定されていない
#define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :No nickname given\r\n")
// 不正なニックネーム
#define ERR_ERRONEUSNICKNAME(client, nick) (":localhost 432 " + client + " " + nick + " :Erroneus nickname\r\n")
// 既に使用中のニックネーム
#define ERR_NICKNAMEINUSE(client, nick) (":localhost 433 " + client + " " + nick + " :Nickname is already in use\r\n")

// ニックネーム衝突（他サーバとの競合）
// # define ERR_NICKCOLLISION(client, nick, user, host) (":localhost 436 " + client + " " + nick + " :Nickname collision KILL from " + user + "@" + host + "\r\n")

// 不正なターゲット
#define ERR_ERRONEUSTARGET(client, target, info) (":localhost 437 " + client + " " + target + " :Erroneous target" + info + "\r\n")
// 指定されたチャネルに所属していない
#define ERR_USERNOTINCHANNEL(client, nick, channel) (":localhost 441 " + client + " " + nick + " " + channel + " :They aren't on that channel\r\n")
// チャネルに参加していない
#define ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " #" + channel + " :You're not on that channel\r\n")
// 既にチャネルに参加している
#define ERR_USERONCHANNEL(client, nick, channel) (":localhost 443 " + client + " " + nick + " #" + channel + " :is already on channel\r\n")
// クライアントが未登録である
#define ERR_NOTREGISTERED ":localhost 451  :You have not registered\r\n"
// パラメータが不足している
#define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters\r\n")
// 再登録は許可されていない
#define ERR_ALREADYREGISTERED(client) (":localhost 462 " + client + " :You may not reregister\r\n")
// パスワードが一致しない
#define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect\r\n")

// サーバからBANされているため接続できない
// # define ERR_YOUREBANNEDCREEP(client) (":localhost 465 " + client + " :You are banned from this server.\r\n")

// チャネルの参加人数上限に達している
#define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " #" + channel + " :Cannot join channel (+l)\r\n")
// 未知のモード文字が指定された
#define ERR_UNKNOWNMODE(client, modechar) (":localhost 472 " + client + " " + modechar + " :is unknown mode char to me\r\n")
// 招待専用チャネルであるため参加できない
#define ERR_INVITEONLYCHAN(client, channel) (":localhost 473 " + client + " #" + channel + " :Cannot join channel (+i)\r\n")

// BANによりチャネル参加が拒否されている
// #define ERR_BANNEDFROMCHAN(client, channel) (":localhost 474 " + client + " " + channel + " :Cannot join channel (+b)\r\n")

// チャネルキーが不正または不足している
#define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")

// 不正なチャネル名が指定された
// #define ERR_BADCHANMASK(channel) (":localhost 476 " + channel + " :Bad Channel Mask\r\n")

// IRCオペレーターとしての権限が不足している
// #define ERR_NOPRIVILEGES(client) (":localhost 481 " + client + " :Permission Denied- You're not an IRC operator\r\n")

// チャネル管理権限が不足している
#define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " #" + channel + " :You're not channel operator\r\n")
// チャネルキーが不正な形式である
#define ERR_INVALIDKEY(client, targetChan) (":localhost 525 " + client + " " + targetChan + " :Key is not well-formed\r\n")

#endif
