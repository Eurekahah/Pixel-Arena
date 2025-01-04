#include "cocos2d.h"
#include <vector>

// use factory method to create different Scenes, but all Scenes should act as the child component of BattleScene, since it is the basic scene

void create_button(ui::Button* button, float X, float Y, float X_size , float Y_size ) {

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto button_origin = Director::getInstance()->getVisibleOrigin();
    float button_width = visibleSize.width * X_size;
    float button_height = visibleSize.height * Y_size;
    auto buttonSize = button->getContentSize();
    float button_scaleX = button_width / buttonSize.width;
    float button_scaleY = button_height / buttonSize.height;
    button->setScale(button_scaleX, button_scaleY);
    button->setPosition(Vec2((button_origin.x + visibleSize.width) * X - button->getContentSize().width * button_scaleX / 2,
        (button_origin.y + visibleSize.height) * Y - music_button->getContentSize().height * button_scaleY / 2));

}

class SceneComponent : public cocos2d::Node {
public:
    SceneComponent() : cocos2d::Node() {}
    virtual ~SceneComponent() {}
    virtual void show() = 0;
    virtual void showDetails() = 0;
    void addChild(SceneComponent* component) {
        this->addChild(component); // 利用Cocos2d的addChild将组件添加到当前节点中
        components.push_back(component); // 记录子组件
    }
    void removeChild(SceneComponent* component){
        if(this->components.contains(component)){
            this->components.erase(component);
            CCLOG("component has been removed");
        }
        else
            CCLOG("component does not exist");
    }

protected:
    std::vector<SceneComponent*> components; // 存储子组件的容器
};

class HeroUI : public SceneComponent{
public:
    HeroUI(const std::string& fileName) : name(fileName){}
    void show(){
        auto sprite = cocos2d::Sprite::create(name);
    }
    void showDetails(){
        CCLOG("This is hero: %s", name.c_str());
    }
private:
    std::string name;
}

class EquipmentUI : public SceneComponent{
public:
    HeroUI(const std::string& fileName) : name(fileName){}
    void show(){
        auto sprite = cocos2d::Sprite::create(name);
    }
    void showDetails(){
        CCLOG("This is equipment : %s", name.c_str());
    }
private:
    std::string name;
}

class ShopScene : public SceneComponent {
public:
    ShopScene(const std::string& name) : name(name) {}

    void show() override {
        ui::Button buy_button = new ui::Button("buy_button.png");
        create_button(buy_button, 150,150, 1, 1);
        auto shop_image = Sprite::create(name);
        CCLOG("Showing shop: %s", name.c_str());
        
        auto label = cocos2d::Label::createWithSystemFont(name, "Arial", 24);
        label->setPosition(cocos2d::Vec2(200, 300)); 
        this->addChild(label); 

        auto sprite = cocos2d::Sprite::create("shop_icon.png");
        sprite->setPosition(cocos2d::Vec2(200, 150)); 
        this->addChild(sprite); 
    }

    void showDetails() override {
        CCLOG("This is shop: %s", name.c_str());
    }

private:
    std::string name;
};

class HeroSelectionScene : public SceneComponent {
public:
    HeroSelectionScene(const std::string& name) : name(name) {}

    void show() override {
        ui::Button buy_hero_button = new ui::Button("hero_selection.png");
        create_button(buy_hero_button, 300,300, 1, 1);
        auto hero_shop = Sprite::create(name);
        CCLOG("Showing shop: %s", name.c_str());
        CCLOG("Showing hero selection: %s", name.c_str());
        auto label = cocos2d::Label::createWithSystemFont(name, "Arial", 24);
        label->setPosition(cocos2d::Vec2(300, 400)); 
        this->addChild(label); 
        
        
        auto heroButton = cocos2d::MenuItemLabel::create(
            cocos2d::Label::createWithSystemFont("Select Hero 1", "Arial", 20),
            CC_CALLBACK_1(HeroSelectionScene::onSelectHero, this)
        );
        heroButton->setPosition(cocos2d::Vec2(400, 400)); 

        auto menu = cocos2d::Menu::create(heroButton, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO); 
        this->addChild(menu); 
    }

    void showDetails() override {
        // 显示英雄选择的详细内容
        CCLOG("This is hero selection: %s", name.c_str());
    }

    void onSelectHero(cocos2d::Ref* sender) {
        CCLOG("Hero selected from %s", name.c_str());
    }

private:
    std::string name;
};

class EquipmentSelectionScene : public SceneComponent {
public:
    EquipmentSelectionScene(const std::string& name) : name(name) {}

    void show() override {
        ui::Button buy_equipment = new ui::Button("equipment_selection.png");
        create_button(buy_hero_button, 600,600, 1, 1);
        auto hero_shop = Sprite::create(name);
        CCLOG("Showing equipment selection: %s", name.c_str());
        auto label = cocos2d::Label::createWithSystemFont(name, "Arial", 24);
        label->setPosition(cocos2d::Vec2(600, 700)); 
        this->addChild(label); 
        auto equipButton = cocos2d::MenuItemLabel::create(
            cocos2d::Label::createWithSystemFont("Select Sword", "Arial", 20),
            CC_CALLBACK_1(EquipmentSelectionScene::onSelectEquipment, this)
        );
        equipButton->setPosition(cocos2d::Vec2(700, 700)); 

        auto menu = cocos2d::Menu::create(equipButton, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO); 
        this->addChild(menu); 
    }

    void showDetails() override {
        CCLOG("This is equipment selection: %s", name.c_str());
    }

    void onSelectEquipment(cocos2d::Ref* sender) {
        CCLOG("Equipment selected from %s", name.c_str());
    }

private:
    std::string name;
};

class BattleScene : public SceneComponent {
public:
    BattleScene(const std::string& name) : name(name) {}

    void show() override {
        CCLOG("Showing battle scene: %s", name.c_str());
    }

    void addChild(SceneComponent* component) override {
        components.push_back(component);
        SceneComponent::addChild(component); 
    }

    void showDetails() override {
        CCLOG("This is battle scene: %s", name.c_str());
        for (auto& child : components) {
            child->showDetails(); 
        }
    }

private:
    std::string name;
    std::vector<SceneComponent*> components; // 存储子场景
};

class SceneManager{
public:
    createScene(){
        return create();
    }
    void GetHeroUI(std::vector<string> hero_names){
        for(std::string hero : hero_names){
            HeroUI* newHero = new HeroUI(hero);
            this->heroes.insert(newHero);
        }
    }
    void GetEquipmentUI(std::vector<std::string> equipments){
        for(std::string equipment : equipments){
            EquipmentUI* newEquipment = new EquipmentUI(equipment);
            this->equipments.insert(newEquipment)
        }
    }
    void AppendHeroesAndEquipmentToScene(){
        for(auto hero : this->heroes){
            this->heroSelection->addChild(hero);
        }
        for(auto equipment : this->equipments){
            this->equipmentSelection->addChild(equipment);
        }
    }
    void RunScene(){
        if(init())
            CCLOG("init succeed");
        else{
            CCLOG("init fail");
            return;
        }
        this->AppendHeroesAndEquipmentToScene();
        this->battleScene->addChild(shop);
        this->battleScene->addChild(heroSelection);
        this->battleScene->addChild(equipmentSelection);
        this->battleScene->show();
        this->battleScene->showDetails();
    }
    bool init(){
        this->battleScene = new BattleScene("Main Battle Scene");
        this->shop = new ShopScene("Weapon Shop");
        this->heroSelection = new HeroSelectionScene("Hero Selection");
        this->equipmentSelection = new EquipmentSelectionScene("Equipment Selection");
        return true;
    }
private:
    BattleScene* battleScene;
    ShopScene* shop;
    HeroSelectionScene* heroSelection;
    EquipmentSelectionScene* equipmentSelection;
    std::vector<HeroUI*> heroes;
    std::vector<EquipmentUI*> equipments;
}