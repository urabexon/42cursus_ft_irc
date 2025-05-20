/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePollFd.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:11:46 by urabex            #+#    #+#             */
/*   Updated: 2025/05/20 18:12:35 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

// 新規クライアントの接続を処理する
int Server::handleNewConnection(std::vector<pollfd> &pollFds, std::vector<pollfd> &tmpPollFds) {
    
}
