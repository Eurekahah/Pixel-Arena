import asyncio
import websockets
import json
import unittest

class GameServerTest:
    def __init__(self):
        self.uri = "ws://localhost:8888"
        self.websocket = None
        self.player_id = None
        self.room_id = None

    async def connect(self):
        self.websocket = await websockets.connect(self.uri)

    async def login(self):
        await self.websocket.send(json.dumps({
            "type": "login",
            "username": "测试用户"
        }))
        response = await self.websocket.recv()
        response_data = json.loads(response)
        self.player_id = response_data.get("player_id")
        return response_data

    async def match_game(self):
        await self.websocket.send(json.dumps({
            "type": "match_game",
            "player_id": self.player_id
        }))
        # response = await self.websocket.recv()
        # response_data = json.loads(response)
        # self.room_id = response_data.get("room_id")
        # return response_data
        return ""
    async def send_game_action(self, action_data):
        await self.websocket.send(json.dumps({
            "type": "game_action",
            "player_id": self.player_id,
            "room_id": self.room_id,
            "action": action_data
        }))
        response = await self.websocket.recv()
        return json.loads(response)

    async def listen_server_messages(self):
        """监听服务器主动发送的消息并打印"""
        try:
            while True:
                message = await self.websocket.recv()
                print("收到服务器消息:", message)  # 打印服务器发送的消息
        except websockets.exceptions.ConnectionClosed:
            print("连接已关闭")

    async def close(self):
        if self.websocket:
            await self.websocket.close()

async def run_tests():
    # 创建两个测试客户端
    client1 = GameServerTest()
    client2 = GameServerTest()

    # 连接服务器
    await client1.connect()
    await client2.connect()

    # 测试登录
    print("客户端1登录:", await client1.login())
    print("客户端2登录:", await client2.login())

    # 启动监听服务器消息的任务
    asyncio.create_task(client1.listen_server_messages())
    asyncio.create_task(client2.listen_server_messages())

    # 测试匹配
    print("客户端1请求匹配:", await client1.match_game())
    print("客户端2请求匹配:", await client2.match_game())

    # 测试游戏操作
    action_data = {"type": "move", "data": {"x": 100, "y": 200}}
    print("客户端1发送操作:", await client1.send_game_action(action_data))

    # 关闭连接
    await client1.close()
    await client2.close()

if __name__ == "__main__":
    asyncio.run(run_tests())