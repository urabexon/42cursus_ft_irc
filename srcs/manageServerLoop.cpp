/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manageServerLoop.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 00:46:13 by urabex            #+#    #+#             */
/*   Updated: 2025/05/20 00:47:00 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

void Server::manageServerLoop() {
    std::vector<pollfd>		pollFds;
	std::vector<pollfd>		tmpPollFds;

    // サーバーのソケットを監視対象に追加

    // サーバーがシャットダウンされるまでループ
}