import asyncio
from datetime import datetime
from typing import Optional, Dict, Callable
from Entity.GameRoom import GameStage


class GameTimer:
    def __init__(self, room: GameRoom):
        self.room = room
        self.preparation_time = 20  # 备战阶段时长
        self.monster_time = 30  # 野怪阶段时长
        self.current_timer: Optional[asyncio.Task] = None
        self.stage_start_time: Optional[datetime] = None
        self.stage_callbacks: Dict[GameStage, Callable] = {}

    async def start_preparation_stage(self):
        """开始备战阶段"""
        self.room.current_stage = GameStage.PREPARATION
        self.stage_start_time = datetime.now()

        # 启动计时器
        if self.current_timer:
            self.current_timer.cancel()
        self.current_timer = asyncio.create_task(self._preparation_timer())

    async def start_monster_stage(self):
        """开始野怪阶段"""
        self.room.current_stage = GameStage.MONSTER
        self.stage_start_time = datetime.now()

        if self.current_timer:
            self.current_timer.cancel()
        self.current_timer = asyncio.create_task(self._monster_timer())

    async def start_battle_stage(self):
        """开始对战阶段"""
        self.room.current_stage = GameStage.BATTLE
        self.stage_start_time = datetime.now()

    def register_stage_callback(self, stage: GameStage, callback: Callable):
        """注册阶段结束回调函数"""
        self.stage_callbacks[stage] = callback

    async def _preparation_timer(self):
        """备战阶段计时器"""
        try:
            await asyncio.sleep(self.preparation_time)
            # 时间到，强制结束备战阶段
            if self.room.current_stage == GameStage.PREPARATION:
                await self._handle_stage_end(GameStage.PREPARATION)
        except asyncio.CancelledError:
            pass

    async def _monster_timer(self):
        """野怪阶段计时器"""
        try:
            await asyncio.sleep(self.monster_time)
            # 时间到，强制结束野怪阶段
            if self.room.current_stage == GameStage.MONSTER:
                await self._handle_stage_end(GameStage.MONSTER)
        except asyncio.CancelledError:
            pass

    async def _handle_stage_end(self, stage: GameStage):
        """处理阶段结束"""
        if stage in self.stage_callbacks:
            await self.stage_callbacks[stage]()

    def get_remaining_time(self) -> int:
        """获取当前阶段剩余时间"""
        if not self.stage_start_time:
            return 0

        elapsed = (datetime.now() - self.stage_start_time).total_seconds()
        if self.room.current_stage == GameStage.PREPARATION:
            return max(0, self.preparation_time - elapsed)
        elif self.room.current_stage == GameStage.MONSTER:
            return max(0, self.monster_time - elapsed)
        return 0