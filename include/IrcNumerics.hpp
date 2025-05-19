/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumerics.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:18:00 by urabex            #+#    #+#             */
/*   Updated: 2025/05/19 21:27:29 by urabex           ###   ########.fr       */
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





#endif
