#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <unordered_map>
#include "cocos2d.h"
#include "json.hpp"

class Player {
public:
    // ���״̬ö��
    enum class Status {
        IDLE,        // ����
        MATCHING,    // ƥ����
        PLAYING       // ��Ϸ��
    };

    CC_SYNTHESIZE_READONLY(std::string, _id, Id);       // ��� ID
    CC_SYNTHESIZE(Status, _status, Status);             // ��ǰ״̬
    CC_SYNTHESIZE(std::string, _name, Name);            // ��ѡ��������ƣ���չ���ܣ�


    
public:
    // ���캯��
    Player(std::string id = "0", Status status = Status::IDLE, const std::string& name = "*Player*")
        : _id(id), _status(status), _name(name) {}

    // ��������
    virtual ~Player() = default;

    // ����Ƿ����ض�״̬
    bool isIdle() const { return _status == Status::IDLE; }
    bool isMatching() const { return _status == Status::MATCHING; }
    bool isPlaying() const { return _status == Status::PLAYING; }
    
    // ��ȡ״̬���ַ�����ʾ
    std::string getStatusString() const;

    // ��ӡ�����Ϣ
    std::string toString() const;
    
    // �������Ϣ���ΪJson��ʽ
    nlohmann::json toJson() const;
    
};

#endif // __PLAYER_H__
