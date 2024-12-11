import asyncio
import websockets
import json

# 客户端连接池
connected_clients = set()


async def handle_client(websocket):
    try:
        # 将客户端添加到连接池
        connected_clients.add(websocket)
        print(f"New client connected. Total clients: {len(connected_clients)}")

        # 接收和处理消息
        async for message in websocket:
            print(f"Received message: {message}")

            # 简单回复机制
            response = f"Server received: {message}"
            await websocket.send(response)

    except websockets.exceptions.ConnectionClosed:
        print("Client disconnected")
    finally:
        # 从连接池移除
        connected_clients.remove(websocket)


async def main():
    # 创建WebSocket服务器
    server = await websockets.serve(
        handle_client,
        "127.0.0.1",  # 本地地址
        8888  # 端口号
    )
    print("WebSocket服务器已在 ws://127.0.0.1:8888 启动")

    # 保持服务器运行
    await server.wait_closed()


# 启动服务器
if __name__ == "__main__":
    asyncio.run(main())