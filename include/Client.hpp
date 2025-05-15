/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:17:54 by urabex            #+#    #+#             */
/*   Updated: 2025/05/16 01:39:53 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"

class Client {
    private:
        int             _clientFd;
        std::string		_userName;
        std::string		_realName;
        std::string		_nickname;
        std::string		_oldNickname;
        std::string		_readBuf;            // 受信バッファ
        std::string		_sendBuf;            // 送信バッファ
        bool		    _connexion_password; // パスワード認証が成功したか
        int             _nmInfo;             // 受け取った情報の数
        bool		    _toDeconnect;        // 切断フラグ
        bool		    _registrationDone;   // 登録完了フラグ

    public:
        Client(int clientFd);
        ~Client();

        // Setters
        void setUserName(const std::string &userName);
        void setRealName(const std::string &realName);
        void setNickname(const std::string &nickname);
        void setOldNickname(const std::string &oldNickname);
        void setReadBuf(const std::string &readBuf);
        void setSendBuf(const std::string &sendBuf);
        void setConnexionPassword();
        void incrementNmInfo();
        void setToDeconnect();
        void setRegistrationDone();
        void resetReadBuf();

        // Getters
        int getClientFd();
        std::string &getUserName();
        std::string &getNickname();
        std::string &getOldNickname();
        std::string &getReadBuf();
        std::string &getSendBuf();
        bool getConnexionPassword();
        int getNmInfo();
        bool getToDeconnect();
        bool isRegistrationDone();
};

#endif
