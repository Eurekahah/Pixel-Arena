#include "Player.h"


std::string Player::getStatusString() const {
    static const std::unordered_map<Status, std::string> statusStrings = {
        {Status::IDLE, "idle"},
        {Status::MATCHING, "matching"},
        {Status::PLAYING, "playing"} };
    return statusStrings.at(_status);
}

// 打印玩家信息
std::string Player::toString() const {
    return "Player[ID=" + _id +
        ", Name=" + _name +
        ", Status=" + getStatusString() + "]";
}

nlohmann::json Player::toJson() const {
    return { {"id", _id}, 
        {"name", _name}, 
        {"status", getStatusString()} };
}

