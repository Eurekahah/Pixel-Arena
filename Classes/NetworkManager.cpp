
#include"NetworkManager.h"

// 定义默认地址
const std::string DEFAULT_SERVER_ADDRESS = "ws://localhost:8888";

NetworkManager* NetworkManager::_instance = nullptr;

NetworkManager::NetworkManager() : _webSocket(nullptr) {
}

NetworkManager::~NetworkManager() {
    if (_webSocket) {
        _webSocket->close();
        CC_SAFE_DELETE(_webSocket);
    }
}


NetworkManager* NetworkManager::getInstance() {
    if (_instance == nullptr) {
        _instance = NetworkManager::create();
        if (_instance && _instance->init()) {
            _instance->retain(); // 保持单例不被自动释放
        }
    }
    return _instance;
}


bool NetworkManager::init() {
    _webSocket = new cocos2d::network::WebSocket();
    // 替换为您的服务器地址
    if (!_webSocket->init(*this, DEFAULT_SERVER_ADDRESS)) {
        delete _webSocket;
        _webSocket = nullptr;
        return false;
    }
    return true;
}

void NetworkManager::sendMessage(const std::string& message) {
    if (_webSocket && _webSocket->getReadyState() == cocos2d::network::WebSocket::State::OPEN) {
        _webSocket->send(message);
    }
}

// WebSocket::Delegate 接口实现
void NetworkManager::onOpen(cocos2d::network::WebSocket* ws) {
    cocos2d::log("WebSocket connection opened");
    // 可以在这里发送初始连接消息
    sendMessage("Client connected");
}

 void NetworkManager::onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) {
    std::string receivedMsg = std::string(data.bytes, data.len);
    cocos2d::log("Received message: %s", receivedMsg.c_str());

    // 在这里处理收到的消息
    // 例如：解析JSON、更新游戏状态等
}

void NetworkManager::onClose(cocos2d::network::WebSocket* ws) {
    cocos2d::log("WebSocket connection closed");
}

void NetworkManager::onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) {
    cocos2d::log("WebSocket error occurred");
}

void NetworkManager::destroyInstance() {
    if (_instance) {
        _instance->release(); // 减少引用计数
        _instance = nullptr;
    }
}


