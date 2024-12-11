#pragma once
#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#include "AppDelegate.h"
#include "network/WebSocket.h"
#include "cocos2d.h"

class NetworkManager : public cocos2d::Node, public cocos2d::network::WebSocket::Delegate {
private:
    static NetworkManager* _instance;

    cocos2d::network::WebSocket* _webSocket;

    NetworkManager();
    virtual ~NetworkManager();

public:
    static NetworkManager* getInstance();

    bool init();

    void sendMessage(const std::string& message);

    virtual void onOpen(cocos2d::network::WebSocket* ws) override;

    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override;
       
    virtual void onClose(cocos2d::network::WebSocket* ws) override;

    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override;

    // 禁用拷贝构造和赋值
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    CREATE_FUNC(NetworkManager);
};
#endif //__NETWORK_MANAGER_H__