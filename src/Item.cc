#ifndef ITEM_CC
#define ITEM_CC
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <mutex>
#include "Enum.cc"
#include "EffectManager.cc"
#include "Player.cc"

class Item :public Entity{
protected:
    Type ItemType;
    bool protecting;
public:
    Item(Type t, bool isProtected=false) : ItemType(t),protecting(isProtected){};
    virtual ~Item() =default;
    void unprotect() { protecting = false; }
    bool isProtected() const { return protecting;}
    Type getType() const {return ItemType;}
};

class Potion:public Item{
public:
    Potion(Type t ) : Item(t){}
    virtual void use(std::shared_ptr<PlayerCharacter> pc) = 0;
};
class RestoreHealthPotion :public Potion{
public:
    RestoreHealthPotion() : Potion(Type::RH) {}
    void use(std::shared_ptr<PlayerCharacter> pc) {
        // 永久效果，直接应用
        auto effect = std::make_unique<RestoreHealthEffect>();
        effect->apply(pc);
    }
};
class PoisonHealthPotion :public Potion {
public:
    PoisonHealthPotion() : Potion(Type::PH) {}
    void use(std::shared_ptr<PlayerCharacter> pc) {
        // 永久效果，直接应用
        auto effect = std::make_unique<PoisonHealthEffect>();
        effect->apply(pc);
    }
};
class BoostAtkPotion :public Potion {
public:
    BoostAtkPotion() : Potion(Type::BA) {}
    void use(std::shared_ptr<PlayerCharacter> pc)  {
        auto effect = std::make_unique<BoostAtkEffect>();
        effect->apply(pc);
        EffectManager::getInstance()->addEffect(std::move(effect));
    }
};
class WoundAtkPotion :public Potion {
public:
    WoundAtkPotion() : Potion(Type::WA) {}
    void use(std::shared_ptr<PlayerCharacter> pc)  {
        auto effect = std::make_unique<WoundAtkEffect>();
        effect->apply(pc);
        EffectManager::getInstance()->addEffect(std::move(effect));
    }
};
class BoostDefPotion :public Potion {
public:
    BoostDefPotion() : Potion(Type::BD) {}
    void use(std::shared_ptr<PlayerCharacter> pc) {
        auto effect = std::make_unique<BoostDefEffect>();
        effect->apply(pc);
        EffectManager::getInstance()->addEffect(std::move(effect));
    }  
};
class WoundDefPotion:public Potion  {
public:
    WoundDefPotion() : Potion(Type::WD) {}
    void use(std::shared_ptr<PlayerCharacter> pc) {
        auto effect = std::make_unique<WoundDefEffect>();
        effect->apply(pc);
        EffectManager::getInstance()->addEffect(std::move(effect));
    }      
};

// 宝物基类
class Gold : public Item {
protected:
    int gold;
public:
    Gold(int g, Type t, bool isProtected=false) : Item(t,isProtected), gold(g) {}
    void getGold(std::shared_ptr<PlayerCharacter>& pc) {pc->setGold(gold);}
    int getValue()const{return gold;}
};
    
class NormalGoldPile : public Gold {
public:
    NormalGoldPile() : Gold(1, Type::NORMAL_GOLD_PILE) {}
};

class SmallHoard : public Gold {
public:
    SmallHoard() : Gold(2, Type::SMALL_HOARD) {}
};

class DragonHoard : public Gold {
public:
    DragonHoard() 
        : Gold(6, Type::DRAGON_HOARD,true) {
    }
};
class MerchantHoard : public Gold {
public:
    MerchantHoard():Gold(4,Type::MERCHANT_HOARD,true){}
};
// 重新设计MajorItem为普通物品
class Compass : public Item {
public:
    Compass() : Item(Type::COMPASS,true) {}
};

// BarrierSuit（单例 + 受保护）
class BarrierSuit : public Item {
public:
    BarrierSuit() : Item(Type::BARRIER_SUIT,true) {}
};

class ItemGenerator {
public:
    // 1/6 chance to spawn a particular potion
    static std::shared_ptr<Potion> generatePotion() {
        int i=rand()%6;
        switch (i) {
            case 0: return std::make_shared<RestoreHealthPotion>();
            case 1: return std::make_shared<PoisonHealthPotion>();
            case 2: return std::make_shared<BoostAtkPotion>();
            case 3: return std::make_shared<WoundAtkPotion>();
            case 4: return std::make_shared<BoostDefPotion>();
            case 5: return std::make_shared<WoundDefPotion>();
            default: return nullptr;
        }
    }
    static std::shared_ptr<Potion> generatePotion(Type t) {
        switch (t) {
            case Type::RH: return std::make_shared<RestoreHealthPotion>();
            case Type::PH: return std::make_shared<PoisonHealthPotion>();
            case Type::BA: return std::make_shared<BoostAtkPotion>();
            case Type::WA: return std::make_shared<WoundAtkPotion>();
            case Type::BD: return std::make_shared<BoostDefPotion>();
            case Type::WD: return std::make_shared<WoundDefPotion>();
            default: return nullptr;
        }
    }
    // 5/8 chance of normal, 1/8 dragon hoard, 1/4 small hoard
    static std::shared_ptr<Gold> generateGold() {
        int i=rand()%8;
        switch (i)
        {
            case 0: return std::make_shared<NormalGoldPile>();
            case 1: return std::make_shared<NormalGoldPile>();
            case 2: return std::make_shared<NormalGoldPile>();
            case 3: return std::make_shared<NormalGoldPile>();
            case 4: return std::make_shared<NormalGoldPile>();
            case 5: return std::make_shared<DragonHoard>();
            case 6: return std::make_shared<SmallHoard>();
            case 7: return std::make_shared<SmallHoard>();
            default: return nullptr;
        }
    }
    static std::shared_ptr<Gold> generateGold(Type t) {
        switch (t)
        {
            case Type::NORMAL_GOLD_PILE: return std::make_shared<NormalGoldPile>();
            case Type::MERCHANT_HOARD: return std::make_shared<MerchantHoard>();
            case Type::DRAGON_HOARD: return std::make_shared<DragonHoard>();
            case Type::SMALL_HOARD: return std::make_shared<SmallHoard>();
            default: return nullptr;
        }
    }
    static std::shared_ptr<Gold> generateMerchantHoard() {
        return std::make_shared<MerchantHoard>();
    }

    static std::shared_ptr<Item> generateCompass() {
        return std::make_shared<Compass>();
    }
    static std::shared_ptr<Item> generateBarrierSuit() {
        return std::make_shared<BarrierSuit>();
    }

};
std::shared_ptr<EffectManager> EffectManager::instance = nullptr;
std::once_flag EffectManager::initFlag;
#endif
