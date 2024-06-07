#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <cmath>

using namespace std;

class Item {
public:

    virtual ~Item() {}

    string name;
    int id = -1;
    int damage = 0;
    int block = 0;
    double weight = 0.0;
};

struct Effect {
    string name;
    int id;
    int effectId;
};

struct Entity {
    string name;
    int health;
    int damage;
    int level;
    bool blocked;
};

struct Player : public Entity {


   vector<int> inventory;
   Item sword;
   Item shield;
   double exp;

    void addItem(int id, Player& player){
        player.inventory.push_back(id);
    }

    int levelUp(Player& player){

        if (!player.exp >= pow(1.3, player.level)){
            return 1;
        }

        int prevHp;
        player.level += 1;

        player.damage = (rand() % 5) * (player.health / player.level);

        prevHp = player.health;
        player.health = 0;

        while(player.health < prevHp){
            player.health = player.level * (rand() % 10);
        }

        return 0;
    }

    void useItem(int id, Player& player){
        vector<int> inv = player.inventory;
        find(inv.begin(), inv.end(), id);
    }
};

// Given an item ID will return the corresponding item s
Item lookup(int id) {
    switch (id) {
        case 0: {
            Item woodSword;
            woodSword.id = 0;
            woodSword.name = "Wood Sword";
            woodSword.weight = 0.09;
            woodSword.damage = 3;
            return woodSword;
        }
        case 1: {
            Item woodShield;
            woodShield.id = 1;
            woodShield.name = "Wood Shield";
            woodShield.weight = 0.13;
            woodShield.block = 2;
            return woodShield;
        }
        case 2: {
            Item stoneSword;
            stoneSword.id = 2;
            stoneSword.name = "Stone Sword";
            stoneSword.weight = 0.12;
            stoneSword.damage = 4;
            return stoneSword;
        }
        case 3: {
            Item stoneShield;
            stoneShield.id = 3;
            stoneShield.name = "Stone Shield";
            stoneShield.weight = 0.20;
            stoneShield.block = 3;
            return stoneShield;
        }
        case 4: {
            Item ironSword;
            ironSword.id = 4;
            ironSword.name = "Iron Sword";
            ironSword.weight = 0.18;
            ironSword.damage = 6;
            return ironSword;
        }
        case 5: {
            Item ironShield;
            ironShield.id = 5;
            ironShield.name = "Iron Shield";
            ironShield.weight = 0.25;
            ironShield.block = 5;
            return ironShield;
        }
        case 6: {
            Item tinSword;
            tinSword.id = 6;
            tinSword.name = "Tin Sword";
            tinSword.weight = 0.14;
            tinSword.damage = 4;
            return tinSword;
        }
        case 7: {
            Item tinShield;
            tinShield.id = 7;
            tinShield.name = "Tin Shield";
            tinShield.weight = 0.18;
            tinShield.block = 3;
            return tinShield;
        }
        case 8: {
            Item tungstenSword;
            tungstenSword.id = 8;
            tungstenSword.name = "Tungsten Sword";
            tungstenSword.weight = 0.22;
            tungstenSword.damage = 8;
            return tungstenSword;
        }
        case 9: {
            Item tungstenShield;
            tungstenShield.id = 9;
            tungstenShield.name = "Tungsten Shield";
            tungstenShield.weight = 0.30;
            tungstenShield.block = 7;
            return tungstenShield;
        }
        case 10: {
            Item copperSword;
            copperSword.id = 10;
            copperSword.name = "Copper Sword";
            copperSword.weight = 0.16;
            copperSword.damage = 5;
            return copperSword;
        }
        case 11: {
            Item copperShield;
            copperShield.id = 11;
            copperShield.name = "Copper Shield";
            copperShield.weight = 0.22;
            copperShield.block = 4;
            return copperShield;
        }
        default: {
            Item nullItem;
            nullItem.id = -1;
            nullItem.name = "NULL";
            nullItem.weight = 0.0;
            return nullItem;
        }
    }
}

struct Food : public Item {
    int healAmount;
};

struct Game {

    Player player;
    vector<int> playerInv;
    int playerHealth;
    int playerDamage;
    int playerLevel;
    string playerName;

    Item playerSword = lookup();
    Item playerShield = lookup();

};

 const string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";   

// ChatGPT made this
class FileManager {
public:

    void saveGameData(const Game& game, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);

        // Serialize player
        size_t nameSize = game.player.name.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(game.player.name.c_str(), nameSize);
        file.write(reinterpret_cast<const char*>(&game.player.health), sizeof(game.player.health));
        file.write(reinterpret_cast<const char*>(&game.player.damage), sizeof(game.player.damage));
        file.write(reinterpret_cast<const char*>(&game.player.level), sizeof(game.player.level));
        file.write(reinterpret_cast<const char*>(&game.player.blocked), sizeof(game.player.blocked));
    
        // Serialize player inventory
        size_t invSize = game.player.inventory.size();
        file.write(reinterpret_cast<const char*>(&invSize), sizeof(invSize));
        file.write(reinterpret_cast<const char*>(game.player.inventory.data()), invSize * sizeof(int));
    
        // Serialize items
        nameSize = game.player.sword.name.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(game.player.sword.name.c_str(), nameSize);
        file.write(reinterpret_cast<const char*>(&game.player.sword.id), sizeof(game.player.sword.id));
        file.write(reinterpret_cast<const char*>(&game.player.sword.damage), sizeof(game.player.sword.damage));
        file.write(reinterpret_cast<const char*>(&game.player.sword.block), sizeof(game.player.sword.block));
        file.write(reinterpret_cast<const char*>(&game.player.sword.weight), sizeof(game.player.sword.weight));
    
        nameSize = game.player.shield.name.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(game.player.shield.name.c_str(), nameSize);
        file.write(reinterpret_cast<const char*>(&game.player.shield.id), sizeof(game.player.shield.id));
        file.write(reinterpret_cast<const char*>(&game.player.shield.damage), sizeof(game.player.shield.damage));
        file.write(reinterpret_cast<const char*>(&game.player.shield.block), sizeof(game.player.shield.block));
        file.write(reinterpret_cast<const char*>(&game.player.shield.weight), sizeof(game.player.shield.weight));
    
        // Serialize player experience
        file.write(reinterpret_cast<const char*>(&game.player.exp), sizeof(game.player.exp));
    
        // Serialize game attributes
        file.write(reinterpret_cast<const char*>(&game.playerHealth), sizeof(game.playerHealth));
        file.write(reinterpret_cast<const char*>(&game.playerDamage), sizeof(game.playerDamage));
        file.write(reinterpret_cast<const char*>(&game.playerLevel), sizeof(game.playerLevel));
    
        // Serialize player name
        nameSize = game.playerName.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(game.playerName.c_str(), nameSize);
    }
    
};

#define TOTAL_ITEMS 11



/*
    int woodItem = 0;
    int woodItem = 1;

    int stoneItem = 2;
    int stoneItem = 3;

    int ironItem = 4;
    int ironItem = 5;

    int goldItem = 6;
    int goldItem = 7;
*/