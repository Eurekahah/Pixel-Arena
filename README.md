# Pixel-Arena

仿照金铲铲之战制作的游戏——像素竞技场（Pixel Arena）

## Config



## Directory Info

- Classes
	客户端游戏类的目录
- Resources
	资源文件目录
	- 图片
	- 字体
	- 音频
	- 数值
- Server
	服务端运行代码目录
- Design
  游戏设计部分


## 针对游戏场景的优化
> 使用组合模式进行优化  

### 优化原因
战斗场景作为游戏的基界面，商店界面、英雄选择界面、装备选择界面都会被附加于其上，而每一个英雄和装备又会被附加到英雄选择界面和装备界面，且他们都属于场景元素，都可以继承同一基类；需要支持灵活添加和删除操作。
### 具体设计
1. **场景组件基类（SceneComponent）**
`SceneComponent` 是组合模式的核心类，它继承自 `cocos2d::Node`，并且是所有具体场景组件（如 `HeroUI`、`ShopScene` 等）的父类。组合模式的关键在于：父节点和子节点都可以以相同的方式处理。换句话说，`SceneComponent` 类定义了操作父类和子类的一致方法，如 `addChild()`、`show()`、`showDetails()`，这些方法可以对父类和子类都进行相同的操作。
2. **具体场景组件**
具体的场景组件（如 `HeroUI、ShopScene` 等）继承自 `SceneComponent`，它们实现了 `show()` 和 `showDetails()` 方法。每个具体组件都有自己独特的行为，但都能通过 `SceneComponent` 提供的一致接口进行管理。
3. **组合父节点和子节点**
在 `BattleScene`（或任何其他场景类）中，可以将多个组件添加到场景中。父组件（如 `BattleScene`）可以包含子组件（如 `HeroSelectionScene`、`EquipmentSelectionScene`、`ShopScene` 等）。这是组合模式的典型应用：父节点和子节点都可以通过相同的接口来进行操作。
4. **递归展示子组件**
在 `showDetails()` 方法中，父类 `SceneComponent` 和所有子类都可以递归调用 `showDetails()` 来显示所有子组件的详细信息。父组件（如 `BattleScene`）通过调用所有子组件的 `showDetails()` 方法，递归地展示其下所有组件的信息。这就是组合模式的一个特点：父组件和子组件都能以相同的方式来执行操作。

### 优化带来的好处
1. **模块化和扩展性：**
每个场景组件（如 `HeroUI`、`ShopScene`、`EquipmentSelectionScene` 等）都能独立工作，并且遵循相同的接口（`show()` 和 `showDetails()`），从而实现了模块化。这意味着如果你需要添加新的场景组件或UI元素，只需创建一个新的 `SceneComponent` 子类，而不需要修改现有的代码。
新的场景组件可以很容易地被组合到现有的场景中，通过 `addChild()` 方法将其添加到父组件。
2. **统一的操作方式：**
无论是父组件还是子组件，都可以通过统一的接口 `addChild()`、`show()` 和 `showDetails()` 进行操作。这为管理和显示场景中的元素提供了统一的方式，使得代码更清晰且易于维护。
3. **递归管理：**
通过递归管理子组件，BattleScene 等父组件可以管理任意层次的子组件。每个子组件在自己的 `showDetails()` 中只需实现自己的展示逻辑，而父组件会自动处理它们的层次结构和细节展示。
4. **提高可维护性：**
`SceneComponent` 的子类负责自己特定的行为，例如 `HeroSelectionScene` 负责显示英雄信息，`ShopScene` 负责显示商店信息。这样，每个场景组件的逻辑独立，代码可读性强，且当需要修改某个特定的组件时，修改的范围不会影响到其他部分。

## Game Scene Optimization
> Optimizing using the Composite Pattern

### Optimization Reasons
As the base interface of the game, the battle scene will have other scenes such as the shop scene, hero selection scene, and equipment selection scene attached to it. Additionally, each hero and equipment will be attached to the hero selection and equipment selection scenes, respectively. All of these elements belong to the scene and can inherit from the same base class. The design needs to support flexible addition and removal operations.

### Specific Design
1. **Scene Component Base Class (SceneComponent)**  
   `SceneComponent` is the core class of the Composite Pattern. It inherits from `cocos2d::Node` and serves as the parent class for all specific scene components (e.g., `HeroUI`, `ShopScene`). The key feature of the Composite Pattern is that both parent and child nodes can be treated in the same way. In other words, the `SceneComponent` class defines common methods such as `addChild()`, `show()`, and `showDetails()`, which can be applied to both parent and child nodes.

2. **Specific Scene Components**  
   Specific scene components (e.g., `HeroUI`, `ShopScene`) inherit from `SceneComponent` and implement the `show()` and `showDetails()` methods. Each component has its unique behavior, but they can all be managed through the common interface provided by `SceneComponent`.

3. **Composing Parent and Child Nodes**  
   In `BattleScene` (or any other scene class), multiple components can be added to the scene. A parent component (e.g., `BattleScene`) can contain child components (e.g., `HeroSelectionScene`, `EquipmentSelectionScene`, `ShopScene`). This is a typical use of the Composite Pattern, where both parent and child nodes can be operated on using the same interface.

4. **Recursive Display of Child Components**  
   In the `showDetails()` method, both the parent class `SceneComponent` and its subclasses can recursively call `showDetails()` to display details of all child components. The parent component (e.g., `BattleScene`) will recursively call `showDetails()` for all its child components, thus displaying the details of the entire component hierarchy. This is one of the key features of the Composite Pattern: both parent and child components can perform operations in the same way.

### Benefits of the Optimization
1. **Modularity and Extensibility**  
   Each scene component (e.g., `HeroUI`, `ShopScene`, `EquipmentSelectionScene`) can function independently, following the same interface (`show()` and `showDetails()`), thus achieving modularity. This means that if a new scene component or UI element needs to be added, it can be done by simply creating a new `SceneComponent` subclass without modifying the existing code. New scene components can easily be composed into the existing scene using the `addChild()` method.

2. **Unified Operation**  
   Both parent and child components can be operated on using the same interface (`addChild()`, `show()`, `showDetails()`). This provides a unified way to manage and display the elements within a scene, making the code cleaner and easier to maintain.

3. **Recursive Management**  
   Through recursive management of child components, the parent component (e.g., `BattleScene`) can manage child components at any depth. Each child component only needs to implement its display logic in its own `showDetails()` method, and the parent component automatically handles the component hierarchy and detail display.

4. **Improved Maintainability**  
   Subclasses of `SceneComponent` handle their specific behavior. For instance, `HeroSelectionScene` handles displaying hero information, and `ShopScene` handles displaying shop information. As a result, each scene component has independent logic, making the code more readable. Additionally, when modifications are required for a particular component, the scope of changes is limited to that component, minimizing the impact on other parts of the system.
