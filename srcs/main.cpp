/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:19:34 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 21:42:40 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <iostream>

int main(int argc, char **argv) {
    // argチェック(ircserv + ポート番号 + パスワード)
    if (argc != 3) {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
    }

    time_t rawtime;
    struct tm *timeinfo;

    // 現在時刻の取得し、ローカル時間に変換
	time(&rawtime);
	timeinfo = localtime(&rawtime);

    // シグナルハンドラ設定(pipe注意)
    signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);
	signal(SIGTERM, Server::signalHandler);
	signal(SIGPIPE, SIG_IGN);

    // サーバーの初期化
	Server ircServer(argv[1], argv[2], timeinfo);

    try {
        // サーバーの情報を取得
		ircServer.getServerInfo();
		// サーバーを起動
        ircServer.launchServer();
		// サーバーのメインループ
        ircServer.manageServerLoop();
    } catch(char const*message) {
        std::cerr << message << std::endl;
		return (EXIT_FAILURE);
    } catch(const std::string &message) {
        std::cerr << message << std::endl;
		return (EXIT_FAILURE);
    }

    return 0;
}
