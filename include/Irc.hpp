/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:17:58 by urabex            #+#    #+#             */
/*   Updated: 2025/05/16 01:25:38 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

// Libraries
#include "IrcNumerics.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <signal.h>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>
#include <map>

// ----- Error masege -----
// fatal errors
#define ERROR_OPEN_FILE			"Error: could not open file"
#define ERROR_FILE_EMPTY			"Error: file is empty"
#define ERROR_FILE_CONTENTS		"Error: file is wrong"
#define ERROR_SERVER_SOCKET		"Error: socket creation failed"
#define ERROR_SERVER_SETSOCKETOPT	"Error: setsockopt failed"
#define ERROR_SERVER_BIND			"Error: bind failed on port "
#define ERROR_SERVER_LISTEN		"Error: listen failed"
// normal errors
#define ERROR_SERVER_ACCEPT		"Error: accept failed"
#define ERROR_SERVER_FULL			"Error: server is already full"
#define ERROR_CLIENT_NOT_FOUND		"Error: client not found #"
#define ERROR_SERVER_RECV			"Error: recv failed"
#define ERROR_CLIENT_INVALID_INFO	"Error: invalid client information"
#define ERROR_SERVER_POLL			"Error: poll failed on fd: "
#define ERROR_CHANNEL_EXIST		"Error: Channel already exists #"
#define ERROR_CHANNEL_NOT_FOUND	"Error: Channel not found #"
#define ERROR_SERVER_EMPTY_CMD		"Error: empty command"

// Server message
#define SERVER_ADD_CLIENT			"[Server] Added client :"
#define SERVER_DISCONNECT_CLIENT	"[Server] Client disconnected :"
#define SERVER_NUMBER_OF_CLIENTS	"[Server] The current number of clients is "

// Settings
#define MAX_CLIENTS 4
#define BUF_SIZE_MSG 1024
#define NUM_OF_CMD 13

// コマンド関連
struct s_ircCommand {
	std::string		prefix; 
	std::string		name;		// コマンド名
	std::string		message;	// メッセージ
};

#endif