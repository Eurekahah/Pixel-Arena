from Entity.Player import Player, PlayerStatus
from typing import List, Optional
import websockets
from Entity.GameRoom import GamePhase, GameRoom
import json
import uuid


class GameServer:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(GameServer, cls).__new__(cls)
            cls._instance._initialize()  # 在创建实例时调用初始化方法
        return cls._instance

    def _initialize(self):
        self.online_players: Dict[str, Player] = {}  # key为player_id
        self.game_rooms: Dict[str, GameRoom] = {}  # key为room_id
        self.connected_clients: Set[websockets.WebSocketServerProtocol] = set()

    async def handle_client(self, websocket: websockets.WebSocketServerProtocol):
        """处理客户端连接的主要方法"""
        try:
            # 将新的WebSocket连接添加到已连接客户端集合中
            self.connected_clients.add(websocket)

            async for message in websocket:
                try:
                    # 解析接收到的JSON消息
                    data = json.loads(message)
                    message_type = data.get("type")

                    # 根据消息类型调用相应的处理方法
                    if message_type == "login":
                        await self.handle_login(websocket, data)

                    elif message_type == "match_game":
                        await self.handle_match_game(websocket, data)

                    elif message_type == "game_action":
                        await self.handle_game_action(websocket, data)

                    elif message_type == "leave_room":
                        await self.handle_leave_room(websocket, data)

                    elif message_type == "disconnect":
                        await self.handle_client_disconnect(websocket)

                    else:
                        await websocket.send(json.dumps({
                            "type": "error",
                            "message": f"未知的消息类型: {message_type}"
                        }))

                except json.JSONDecodeError:
                    await websocket.send(json.dumps({
                        "type": "error",
                        "message": "无效的JSON格式"
                    }))

        except websockets.exceptions.ConnectionClosed:
            # 处理连接关闭
            await self.handle_client_disconnect(websocket)

        finally:
            # 清理断开连接的客户端
            self.connected_clients.remove(websocket)
            await self.handle_client_disconnect(websocket)

    async def handle_login(self, websocket, data):
        """处理登录请求"""
        # 生成唯一的player_id
        player_id = str(uuid.uuid4())

        # 创建新玩家并保存
        new_player = Player(websocket, player_id)
        self.online_players[player_id] = new_player

        # 返回登录成功响应，包含player_id
        await websocket.send(json.dumps({
            "type": "login_response",
            "success": True,
            "message": "登录成功",
            "player_id": player_id
        }))

    def find_available_room(self) -> Optional[GameRoom]:
        """查找可用的等待中的房间"""
        for room in self.game_rooms.values():
            if room.phase == GamePhase.WAITING and not room.is_full:
                return room
        return None

    async def handle_match_game(self, websocket, data):
        """处理游戏匹配请求"""
        player_id = data.get("player_id")
        player = self.online_players.get(player_id)

        if not player:
            await websocket.send(json.dumps({
                "type": "match_game_response",
                "success": False,
                "message": "玩家未登录"
            }))
            return

        if player.is_playing :
            await websocket.send(json.dumps({
                "type": "match_game_response",
                "success": False,
                "message": "玩家已在其他游戏中"
            }))
            return

        # 查找可用房间
        room = self.find_available_room()

        # 如果没有可用房间，创建新房间
        if not room:
            room = GameRoom(player)
            self.game_rooms[room.room_id] = room
            print("新建房间",room.to_dict())
            await websocket.send(json.dumps({
                "type": "match_game_response",
                "success": True,
                "message": "已创建新房间，等待其他玩家加入",
                "room_data": room.to_dict()
            }))
            return

        # 如果有可用房间，将玩家加入该房间
        if room.add_player(player):
            print("找到房间", room.to_dict())
            # 通知房间所有玩家
            for p in room.players:
                await p.websocket.send(json.dumps({
                    "type": "room_update",
                    "room_data": room.to_dict()
                }))

            # 如果房间满员，自动开始游戏
            if room.is_full:
                room.start_game()
                for p in room.players:
                    await p.websocket.send(json.dumps({
                        "type": "game_start",
                        "room_data": room.to_dict()
                    }))
        else:
            await websocket.send(json.dumps({
                "type": "match_game_response",
                "success": False,
                "message": "加入房间失败"
            }))

    async def handle_game_action(self, websocket, data):
        """处理游戏中的操作"""
        player_id = data.get("player_id")
        room_id = data.get("room_id")
        action = data.get("action")

        room = self.game_rooms.get(room_id)
        if not room:
            await websocket.send(json.dumps({
                "type": "error",
                "message": "房间不存在"
            }))
            return

        # 处理游戏操作
        result = room.process_action(player_id, action)

        await websocket.send(json.dumps(result))

        # 向房间内所有玩家广播游戏状态更新
        for player in room.players:
            await player.websocket.send(json.dumps({
                "type": "game_update",
                "room_data": room.to_dict()
            }))

    async def handle_leave_room(self, websocket, data):
        """处理玩家离开房间"""
        player_id = data.get("player_id")
        room_id = data.get("room_id")

        # 实现玩家离开房间的逻辑
        player = self.online_players.get(player_id)

        room = self.game_rooms.get(room_id)

        if room:
            room.remove_player(player)

    async def handle_client_disconnect(self, websocket):
        """处理客户端断开连接"""
        # 查找断开连接的玩家
        disconnected_player = None
        for player in self.online_players.values():
            if player.websocket == websocket:
                disconnected_player = player
                break

        if disconnected_player:
            # 从在线玩家列表中移除
            del self.online_players[disconnected_player.player_id]

            # 处理玩家所在房间的清理工作
            for room in self.game_rooms.values():
                if disconnected_player in room.players:
                    # 处理玩家离开房间的逻辑
                    room.remove_player(disconnected_player)
