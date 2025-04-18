#ifndef ENEMY_CC
#define ENEMY_CC
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <cmath>
#include <map>
#include "Player.cc"
#include "Enum.cc"
#include "Entity.cc"
#include "Item.cc"
#include "Position.cc"

class Enemy:public Entity{
protected:
    Type enemyType;
    int maxHp, hp, atk, def;
    std::shared_ptr<Item> protectedItem;
    Position proectedItemPos;
public:
    Enemy(Type t)
        :  enemyType(t) {
        switch (t) {
            case Type::VAMPIRE: maxHp = hp = 50; atk = 25; def = 25; break;
            case Type::WEREWOLF: maxHp = hp = 120; atk = 30; def = 5; break;
            case Type::TROLL: maxHp = hp = 120; atk = 25; def = 15; break;
            case Type::GOBLIN: maxHp = hp = 70; atk = 5; def = 10; break;
            case Type::MERCHANT: maxHp = hp = 30; atk = 70; def = 5; break;
            case Type::DRAGON: maxHp = hp = 150; atk = 20; def = 20; break;
            case Type::PHOENIX: maxHp = hp = 50; atk = 35; def = 20; break;
            default: throw std::invalid_argument("Unknown enemy type");
        }
    }
    std::shared_ptr<Item> getProtectedItem() const{return protectedItem;}
    Position getProtectedItemPosition() const{return proectedItemPos;}
    void setProtectedItem(std::shared_ptr<Item> it, Position pos){protectedItem=it; proectedItemPos=pos;}
    std::map<std::string, int> getInfo() const {
        std::map<std::string, int> info;
        info["HP"] = hp;
        info["Atk"] = atk;
        info["Def"] = def;
        return info;
    }
    Type getType() const {return enemyType;}
    int getHp()const{return hp;}
    int getAtk()const{return atk;}
    int getDef()const{return def;}
    int getMaxHp()const{return maxHp;}
    void setHp(int delta) {
        hp += delta;
        if(hp > maxHp) hp = maxHp;
        if(hp < 0) hp = 0;
    }
    bool isDead() const { return hp<=0;}
    virtual void useSpecialAbility(PlayerCharacter& player) {}

};

class Vampire : public Enemy {
    int stealAmount = 10;
public:
Vampire(): Enemy(Type::VAMPIRE) {}

    void useSpecialAbility(PlayerCharacter& player) {
        player.setHp(-stealAmount);
        hp += stealAmount;
        std::cout << "Vampire drained " << stealAmount << " HP from the player and restored its own HP!" << std::endl;
    }
};

class Merchant : public Enemy {
public:
Merchant(): Enemy(Type::MERCHANT) {}
};

class Dragon : public Enemy {
public:
    Dragon() : Enemy(Type::DRAGON) {}
};
class Werewolf : public Enemy {
public:
Werewolf() : Enemy(Type::WEREWOLF) {}
};
class Troll : public Enemy {
    int healAmount = 5;
public:
Troll() : Enemy(Type::TROLL) {}
    void useSpecialAbility() {
        hp += healAmount;
        std::cout << "Troll regenerates " << healAmount << " HP using its natural healing ability!" << std::endl;

    }
};
class Phoenix : public Enemy {
    bool isRisen_ = false;
public:
    Phoenix() : Enemy(Type::PHOENIX) {}
    void useSpecialAbility() {
        if (hp <= 0 && !isRisen_) {
            hp = maxHp / 2;
            isRisen_ = true;
            std::cout << "Phoenix rises from the ashes with half of its maximum HP (" << hp << ")!" << std::endl;
        } 
    }
    bool isRisen(){return isRisen_;}
};
class Goblin : public Enemy {
    int stealAmount = 5;
public:
    Goblin() : Enemy(Type::GOBLIN) {}
    void useSpecialAbility(PlayerCharacter& player) {
        player.setGold(-stealAmount);
        std::cout << "Goblin steals " << stealAmount << " gold from the player!" << std::endl;
    }
};

class EnemyGenerator {
public:
    // WEREWOLF: 2/9 VAMPIRE: 3/18 GOBLIN: 5/18 TROLL: 1/9 PHOENIX: 1/9 MERCHANT: 1/9
    static std::shared_ptr<Enemy> generateEnemy() {
        int i = rand()%18;
        // 设置随机种子
        if (i < 4) {          // 2/9 (4/18): 狼人
            return std::make_shared<Werewolf>();
        } else if (i < 7) {   // 3/18: 吸血鬼
            return std::make_shared<Vampire>();
        } else if (i < 12) {  // 5/18: 哥布林
            return std::make_shared<Goblin>();
        } else if (i < 14) {  // 1/9 (2/18): 巨魔
            return std::make_shared<Troll>();
        } else if (i < 16) {  // 1/9 (2/18): 凤凰
            return std::make_shared<Phoenix>();
        } else {              // 1/9 (2/18): 商人
            return std::make_shared<Merchant>();
        }
    }
    static std::shared_ptr<Enemy> generateEnemy(Type t) {
        switch(t){
            case Type::WEREWOLF: return std::make_shared<Werewolf>();
            case Type::VAMPIRE: return std::make_shared<Vampire>();
            case Type::GOBLIN: return std::make_shared<Goblin>();
            case Type::PHOENIX: return std::make_shared<Phoenix>();
            case Type::MERCHANT: return std::make_shared<Merchant>();
            case Type::TROLL: return std::make_shared<Troll>();
            case Type::DRAGON: return std::make_shared<Dragon>();
            default: return nullptr;
        }
    }
    static std::shared_ptr<Enemy> generateDragon() {
        return std::make_shared<Dragon>();
    }
};
#endif
