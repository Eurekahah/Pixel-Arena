from enum import Enum
from typing import List, Optional, Dict,Any
from Entity.Player import Player, PlayerStatus
import uuid
from dataclasses import dataclass

class GamePhase(Enum):
    """游戏状态枚举"""
    WAITING = "waiting"       # 等待玩家加入
    PREPARATION = "preparation"  # 备战阶段
    MONSTER = "monster"  # 野怪阶段
    COMBAT = "combat"        # 战斗阶段
    FINISHED = "finished"    # 游戏结束

class ActionType(Enum):
    BUY_HERO = "buy_hero"            # 购买英雄
    SELL_HERO = "sell_hero"          # 出售英雄
    SELL_EQUIPMENT = "sell_equipment"# 出售装备
    MOVE_PLAYER = "move_player"      # 移动玩家
    PLACE_HERO = "place_hero"        # 放置英雄
    WITHDRAW_HERO = "withdraw_hero"  # 撤下英雄
    EQUIP_ITEM = "equip_item"        # 装备
    UNEQUIP_ITEM = "unequip_item"    # 卸装


@dataclass
class GameRoom:
    max_players: int = 2  # 最大玩家数

    def __init__(self, creator: Player):
        self.room_id = str(uuid.uuid4())

        self.players: List[Player] = []
        self.add_player(creator)
        self.current_round: int = 0
        self.phase: GamePhase = GamePhase.WAITING

    @property
    def is_full(self) -> bool:
        return len(self.players) >= self.max_players

    def add_player(self, player: Player) -> bool:
        """添加玩家到房间"""
        if self.is_full:
            return False
        self.players.append(player)
        player.join_room(self.room_id)
        return True

    def remove_player(self, player: Player):
        """将玩家从房间移除"""
        if player in self.players:
            self.players.remove(player)
            player.leave_room()

    def start_game(self):
        """开始游戏"""
        if len(self.players) == self.max_players:
            self.phase = GamePhase.PREPARATION
            self.current_round = 1
            return True
        return False

    def next_round(self):
        """进入下一回合"""
        self.current_round += 1

    def process_action(self, player_id: str, action: Dict[str, Any]) -> dict:
        """
        处理玩家操作

        参数:
            player_id: 执行操作的玩家ID
            action: {
                "type": ActionType的值,
                "data": 具体操作的数据
            }
        """
        try:
            # 验证玩家
            player = next((p for p in self.players if p.player_id == player_id), None)
            if not player:
                return {"success": False, "message": "玩家不存在"}

            action_type = ActionType(action.get("type"))
            action_data = action.get("data", {})

            # 任何时候都可以执行的操作
            if action_type in [ActionType.BUY_HERO, ActionType.SELL_HERO,
                               ActionType.SELL_EQUIPMENT, ActionType.MOVE_PLAYER]:
                return self._handle_anytime_action(player, action_type, action_data)

            # 只能在备战阶段或野怪结束后执行的操作
            if action_type in [ActionType.PLACE_HERO, ActionType.WITHDRAW_HERO,
                               ActionType.EQUIP_ITEM, ActionType.UNEQUIP_ITEM]:
                if not self._can_modify_battlefield(player):
                    return {
                        "success": False,
                        "message": "当前阶段不能进行此操作"
                    }
                return self._handle_preparation_action(player, action_type, action_data)

            return {"success": False, "message": "未知的操作类型"}

        except Exception as e:
            return {"success": False, "message": f"操作处理失败: {str(e)}"}

    def _can_modify_battlefield(self, player: Player) -> bool:
        """判断玩家是否可以修改战场"""
        # 在备战阶段可以修改
        if self.current_stage == GameStage.PREPARATION:
            return True

        # 在等待阶段可以修改
        if self.current_stage == GameStage.WAITING:
            return True

        return False

    def _handle_anytime_action(self, player: Player, action_type: ActionType,
                               action_data: dict) -> dict:
        """处理任何时候都可以执行的操作"""
        if action_type == ActionType.BUY_HERO:
            return self._buy_hero(player, action_data.get("hero_id"))

        elif action_type == ActionType.SELL_HERO:
            return self._sell_hero(player, action_data.get("hero_id"))

        elif action_type == ActionType.SELL_EQUIPMENT:
            return self._sell_equipment(player, action_data.get("equipment_id"))

        elif action_type == ActionType.MOVE_PLAYER:
            result = self._move_player(player,
                                       action_data.get("position_x"),
                                       action_data.get("position_y"))
            if result["success"]:
                # 广播玩家位置更新
                self.broadcast_player_position(player)
            return result

    def _handle_preparation_action(self, player: Player, action_type: ActionType,
                                   action_data: dict) -> dict:
        """处理备战相关的操作"""
        if action_type == ActionType.PLACE_HERO:
            return self._place_hero(player,
                                    action_data.get("hero_id"),
                                    action_data.get("position_x"),
                                    action_data.get("position_y"))

        elif action_type == ActionType.WITHDRAW_HERO:
            return self._withdraw_hero(player, action_data.get("hero_id"))

        elif action_type == ActionType.EQUIP_ITEM:
            return self._equip_item(player,
                                    action_data.get("hero_id"),
                                    action_data.get("equipment_id"))

        elif action_type == ActionType.UNEQUIP_ITEM:
            return self._unequip_item(player,
                                      action_data.get("hero_id"),
                                      action_data.get("equipment_id"))

    def _buy_hero(self, player: Player, hero_id: str) -> dict:
        """购买英雄的具体实现

        Args:
            player: 玩家对象
            hero_id: 要购买的英雄ID

        Returns:
            dict: 操作结果
        """
        try:
            # 检查商店是否有该英雄
            if hero_id not in self.shop_heroes:
                return {"success": False, "message": "商店中没有该英雄"}

            hero = self.shop_heroes[hero_id]
            # 检查玩家金币是否足够
            if player.gold < hero.price:
                return {"success": False, "message": "金币不足"}

            # 检查英雄槽位是否已满
            if len(player.heroes) >= player.max_hero_slots:
                return {"success": False, "message": "英雄槽位已满"}

            # 扣除金币
            player.gold -= hero.price
            # 添加英雄到玩家英雄列表
            player.heroes[hero_id] = hero
            # 从商店移除该英雄
            del self.shop_heroes[hero_id]

            return {
                "success": True,
                "message": "购买英雄成功",
                "hero_id": hero_id,
                "cost": hero.price
            }
        except Exception as e:
            return {"success": False, "message": f"购买英雄失败: {str(e)}"}

    def _sell_hero(self, player: Player, hero_id: str) -> dict:
        """出售英雄的具体实现

        Args:
            player: 玩家对象
            hero_id: 要出售的英雄ID

        Returns:
            dict: 操作结果
        """
        try:
            # 检查玩家是否拥有该英雄
            if hero_id not in player.heroes:
                return {"success": False, "message": "您没有该英雄"}

            hero = player.heroes[hero_id]
            # 计算出售价格(通常为购买价的一半)
            sell_price = hero.price // 2

            # 移除英雄
            del player.heroes[hero_id]
            # 返还金币
            player.gold += sell_price

            return {
                "success": True,
                "message": "出售英雄成功",
                "hero_id": hero_id,
                "gold_received": sell_price
            }
        except Exception as e:
            return {"success": False, "message": f"出售英雄失败: {str(e)}"}

    def _sell_equipment(self, player: Player, equipment_id: str) -> dict:
        """出售装备的具体实现

        Args:
            player: 玩家对象
            equipment_id: 要出售的装备ID

        Returns:
            dict: 操作结果
        """
        try:
            # 检查玩家是否拥有该装备
            if equipment_id not in player.items:
                return {"success": False, "message": "您没有该装备"}

            equipment = player.items[equipment_id]
            # 计算出售价格
            sell_price = equipment.price // 2

            # 移除装备
            del player.items[equipment_id]
            # 返还金币
            player.gold += sell_price

            return {
                "success": True,
                "message": "出售装备成功",
                "equipment_id": equipment_id,
                "gold_received": sell_price
            }
        except Exception as e:
            return {"success": False, "message": f"出售装备失败: {str(e)}"}

    def _move_player(self, player: Player, des_x: float, des_y: float) -> dict:
        """移动英雄的具体实现

        Args:
            player: 玩家对象
            des_x: 目标X坐标
            des_y: 目标Y坐标

        Returns:
            dict: 操作结果
        """
        try:
            # 检查目标位置是否有效
            if not self._is_valid_position(des_x, des_y):
                return {"success": False, "message": "目标位置无效"}

            # 更新玩家位置
            player.position_x = des_x
            player.position_y = des_y

            return {
                "success": True,
                "message": "移动成功",
                "new_position": {
                    "x": des_x,
                    "y": des_y
                }
            }
        except Exception as e:
            return {"success": False, "message": f"移动失败: {str(e)}"}

    def _place_hero(self, player: Player, des_x: float, des_y: float) -> dict:
        """放置英雄的具体实现

        Args:
            player: 玩家对象
            des_x: 目标X坐标
            des_y: 目标Y坐标

        Returns:
            dict: 操作结果
        """
        try:
            # 检查目标位置是否有效
            if not self._is_valid_position(des_x, des_y):
                return {"success": False, "message": "目标位置无效"}

            # 检查目标位置是否已被占用
            if self._is_position_occupied(des_x, des_y):
                return {"success": False, "message": "该位置已被占用"}

            # 检查是否有选中的英雄
            if not player.selected_hero:
                return {"success": False, "message": "未选择要放置的英雄"}

            # 放置英雄
            hero = player.selected_hero
            hero.position_x = des_x
            hero.position_y = des_y
            player.heroes_on_field[hero.id] = hero

            return {
                "success": True,
                "message": "放置英雄成功",
                "hero_id": hero.id,
                "position": {
                    "x": des_x,
                    "y": des_y
                }
            }
        except Exception as e:
            return {"success": False, "message": f"放置英雄失败: {str(e)}"}

    def _is_valid_position(self, x: float, y: float) -> bool:
        """检查位置是否在有效范围内"""
        return (0 <= x <= self.map_width and 0 <= y <= self.map_height)

    def _is_position_occupied(self, x: float, y: float) -> bool:
        """检查位置是否已被占用"""
        # 这里需要实现具体的位置检查逻辑
        pass

    def _withdraw_hero(self, player: Player, hero_id: str) -> dict:
        """撤下英雄的具体实现

        Args:
            player: 玩家对象
            hero_id: 要撤下的英雄ID

        Returns:
            dict: 操作结果
        """
        try:
            # 检查英雄是否存在且在场上
            if hero_id not in player.heroes_on_field:
                return {"success": False, "message": "该英雄不在场上"}

            # 从场上移除英雄
            hero = player.heroes_on_field.pop(hero_id)
            # 将英雄加入备用区
            player.heroes_in_bench[hero_id] = hero

            return {
                "success": True,
                "message": "英雄撤下成功",
                "hero_id": hero_id
            }
        except Exception as e:
            return {"success": False, "message": f"撤下英雄失败: {str(e)}"}

    def _equip_item(self, player: Player, hero_id: str, item_id: str) -> dict:
        """为英雄装备物品的具体实现

        Args:
            player: 玩家对象
            hero_id: 英雄ID
            item_id: 装备ID

        Returns:
            dict: 操作结果
        """
        try:
            # 检查英雄是否存在
            if hero_id not in player.heroes:
                return {"success": False, "message": "英雄不存在"}

            # 检查装备是否存在
            if item_id not in player.items:
                return {"success": False, "message": "装备不存在"}

            hero = player.heroes[hero_id]
            item = player.items[item_id]

            # 检查装备槽位是否已满
            if len(hero.equipment) >= hero.max_equipment_slots:
                return {"success": False, "message": "装备槽位已满"}

            # 装备物品
            hero.equipment[item_id] = item
            # 从玩家物品栏移除
            del player.items[item_id]

            return {
                "success": True,
                "message": "装备成功",
                "hero_id": hero_id,
                "item_id": item_id
            }
        except Exception as e:
            return {"success": False, "message": f"装备失败: {str(e)}"}

    def _unequip_item(self, player: Player, hero_id: str, item_id: str) -> dict:
        """为英雄卸下装备的具体实现

        Args:
            player: 玩家对象
            hero_id: 英雄ID
            item_id: 装备ID

        Returns:
            dict: 操作结果
        """
        try:
            # 检查英雄是否存在
            if hero_id not in player.heroes:
                return {"success": False, "message": "英雄不存在"}

            hero = player.heroes[hero_id]

            # 检查装备是否存在于英雄身上
            if item_id not in hero.equipment:
                return {"success": False, "message": "该英雄未装备此物品"}

            # 卸下装备
            item = hero.equipment.pop(item_id)
            # 将装备返回到玩家物品栏
            player.items[item_id] = item

            return {
                "success": True,
                "message": "卸装成功",
                "hero_id": hero_id,
                "item_id": item_id
            }
        except Exception as e:
            return {"success": False, "message": f"卸装失败: {str(e)}"}
    def to_dict(self):
        """转换为字典格式用于JSON传输"""
        return {
            "room_id": self.room_id,
            "players": [p.player_id for p in self.players],
            "current_round": self.current_round,
            "phase": self.phase.value,
        }
