from dataclasses import dataclass
from enum import Enum
import websockets
from typing import Optional


class PlayerStatus(Enum):
    MATCHING = "matching"  # 匹配中
    PLAYING = "playing"  # 游戏中
    IDLE = "idle"  # 空闲中


@dataclass
class Player:

    def __init__(self, websocket: websockets.WebSocketServerProtocol, player_id: str):
        self.websocket: websockets.WebSocketServerProtocol = websocket
        self.player_id: str = player_id
        self.username: str = f"玩家{player_id[:6]}"  # 使用player_id前6位作为默认用户名
        self.player_status: PlayerStatus = PlayerStatus.IDLE
        self.current_room_id: str = None
        self.score: float= 0

    @property
    def is_playing(self) -> bool:
        """判断玩家是否在游戏中"""
        return self.player_status in [PlayerStatus.PLAYING, PlayerStatus.MATCHING]

    def reset_status(self):
        """重置玩家状态"""
        self.player_status = PlayerStatus.IDLE
        self.current_room_id = None
        self.score = 0

    def join_room(self, room_id: str):
        """加入房间"""
        self.current_room_id = room_id
        self.player_status = PlayerStatus.MATCHING

    def leave_room(self):
        """离开房间"""
        self.current_room_id = None
        self.player_status = PlayerStatus.IDLE

    def start_game(self):
        """开始游戏"""
        self.player_status = PlayerStatus.PLAYING

    def add_score(self, points: int):
        """增加得分"""
        self.score += points

    def to_dict(self):
        """转换为字典格式用于JSON传输"""
        return {
            "player_id": self.player_id,
            "username": self.username,
            "status": self.player_status.value,
            "score": self.score,
            "room_id": self.current_room_id
        }