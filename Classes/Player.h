#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <unordered_map>
#include "cocos2d.h"
#include "json.hpp"

class Player {
public:
    // 玩家状态枚举
    enum class Status {
        IDLE,        // 空闲
        MATCHING,    // 匹配中
        PLAYING       // 游戏中
    };

    CC_SYNTHESIZE_READONLY(std::string, _id, Id);       // 玩家 ID
    CC_SYNTHESIZE(Status, _status, Status);             // 当前状态
    CC_SYNTHESIZE(std::string, _name, Name);            // 可选的玩家名称（扩展功能）


    
public:
    // 构造函数
    Player(std::string id = "0", Status status = Status::IDLE, const std::string& name = "*Player*")
        : _id(id), _status(status), _name(name) {}

    // 析构函数
    virtual ~Player() = default;

    // 检查是否处于特定状态
    bool isIdle() const { return _status == Status::IDLE; }
    bool isMatching() const { return _status == Status::MATCHING; }
    bool isPlaying() const { return _status == Status::PLAYING; }
    
    // 获取状态的字符串表示
    std::string getStatusString() const;

    // 打印玩家信息
    std::string toString() const;
    
    // 将玩家信息打包为Json格式
    nlohmann::json toJson() const;
    
};

#endif // __PLAYER_H__
