#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <cmath>
#include <unordered_map>

#include "items.hpp"

using namespace std;

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
    int defense;
    bool blocked;
};

struct Player : public Entity {


   vector<int> inventory;
   Item sword;
   Item shield;
   Item armor;
   double exp;

    void addItem(int id, Player& player){
        player.inventory.push_back(id);
    }

void levelUp(Player& player) {
    // Example threshold calculation for experience
    const double threshold = std::pow(1.3, player.level);
    if (player.exp <= threshold) {
        return;
    }

    player.level += 1;

    // Damage calculation
    if (player.level > 0) {
        player.damage = (std::rand() % 5) * (player.health / player.level);
    }

    int prevHp = player.health;

    // Ensure health is incremented reasonably
    player.health = prevHp + player.level * (std::rand() % 10 + 1);

    return;
}


    void useItem(int id, Player& player){
        vector<int> inv = player.inventory;
        find(inv.begin(), inv.end(), id);
    }
};

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

    Item playerSword = items[999];
    Item playerShield = items[999];

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