import asyncio
import websockets
import json
from typing import Dict, Set
from GameServer import GameServer


async def main():
    game_server = GameServer()
    server = await websockets.serve(
        game_server.handle_client,
        "127.0.0.1",
        8888
    )
    print("WebSocket服务器已在 ws://127.0.0.1:8888 启动")
    await server.wait_closed()


if __name__ == "__main__":
    asyncio.run(main())