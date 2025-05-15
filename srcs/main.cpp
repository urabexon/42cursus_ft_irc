/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:19:34 by urabex            #+#    #+#             */
/*   Updated: 2025/05/16 01:21:49 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

int main(int argc, char **argv) {
    // argチェック(ircserv + ポート番号 + パスワード)
    if (argc != 3) {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
    }

    
    return 0;
}
