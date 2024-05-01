#include <string>
#include <algorithm>
#include <vector>

using namespace std;

struct Effect {
    std::string name;
    int id;
    int effectId;
};

struct Entity {
    string name;
    int health;
    int damage;
    int level;
};

struct Player : public Entity {

   vector<int> inventory;

    void AddItem(int id, Player player){
        player.inventory.push_back(id);
   }
};

int totalItems = 8;

struct Item {
    std::string name;
    int id;
    double weight;
};

struct Sword : public Item {
    int damage;
};

struct Shield : public Item {
    int block;
};

struct Food : public Item {
    int healAmount;
};

// Given an item ID will return the corresponding item
Item lookup(int id){
    switch (id){
        case 0:{
            Sword woodSword;
            woodSword.id = 0;
            woodSword.name = "Wood Sword";
            woodSword.weight = 0.10;
            woodSword.damage = 3;
            return woodSword;
        }
        case 1:{
            Shield woodShield;
            woodShield.id = 1;
            woodShield.name = "Wood Shield";
            woodShield.weight = 0.15;
            woodShield.block = 2;
            return woodShield;
        }
        case 2:{
            Sword stoneSword;
            stoneSword.id = 2;
            stoneSword.name = "Stone Sword";
            stoneSword.weight = 0.12;
            stoneSword.damage = 4;
            return stoneSword;
        }


        default:{
            Item nullItem;
            nullItem.id = 999;
            nullItem.name = "NULL";
            nullItem.weight = 0.0;
            return nullItem;
        }
    }
}

void useItem(int id, Player player){
    vector<int> inv = player.inventory;
    find(inv.begin(), inv.end(), id);
}

/*
    int woodSword = 0;
    int woodShield = 1;

    int stoneSword = 2;
    int stoneShield = 3;

    int ironSword = 4;
    int ironShield = 5;

    int goldSword = 6;
    int goldShield = 7;
*/