/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:17:51 by urabex            #+#    #+#             */
/*   Updated: 2025/05/16 01:44:34 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Irc.hpp"
#include "Client.hpp"
#include <iostream>
#include <string>
#include <map>

struct s_mode {
	bool	invite;
	bool	topic;
	bool	key;
	bool	limit;
};

class Channel {
    private:
	    std::string						_name;
	    std::string						_topic;
	    s_mode							_mode;
	    std::string						_password;
	    size_t							_capacity; // チャンネルに参加できる最大クライアント数
	    std::map<const int, Client>		_clientList;
	    std::vector<int>				_invitedList;
	    std::vector<int>				_operatorList;

  public:
	    Channel(const std::string &name);
	    ~Channel();

        // Setters
	    void setTopic(const std::string &topic);
	    void setMode(bool sign, const char mode); // 1つのモードを設定する場合
	    void setPassword(const std::string &password);
	    void setCapacity(int capacity);

	    // Getters
	    const std::string &getName() const;
	    const std::string &getTopic() const;
	    bool getMode(const std::string mode) const;
	    const std::string &getPassword() const;
	    size_t getCapacity() const;
	    std::map<const int, Client> &getClientList();
	
	    // check
	    bool isClientInChannel(const int clientFd);
	    bool isInvited(const int clientFd);
	    bool isOperator(const int clientFd);
	    // add
	    void addClientToChannel(Client &client);
	    void addInvitedList(int clientFd);
	    void addOperatorList(int clientFd);
	    // remove
	    void removeClient(int clientFd);
	    void removeInvited(int clientFd);
	    void removeOperator(int clientFd);
	
	    // others
	    std::string getClientListString();
};

#endif
