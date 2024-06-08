#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <cmath>
#include <unordered_map>

using namespace std;

class Item {
public:

    virtual ~Item() {}

    string name;
    int id = -1;
    int damage = 0;
    int block = 0;
    double weight = 0.0;

    Item() : id(-1), name(""), weight(0.0), damage(0), block(0) {}

    Item(int i, const string& n, double w, int d = 0, int b = 0)
        : id(i), name(n), weight(w), damage(d), block(b) {}
};

#define TOTAL_ITEMS 24

bool contains(const string& str, string term) {
    return str.find(term) != string::npos;
}

unordered_map<int, Item> items;

void initializeItems() {
    // Tier One
    items[0] = {0, "Wood Sword", 0.09, 3, 0};
    items[1] = {1, "Wood Shield", 0.13, 0, 2};
    items[2] = {2, "Stone Sword", 0.12, 4, 0};
    items[3] = {3, "Stone Shield", 0.20, 0, 3};
    items[4] = {4, "Iron Sword", 0.18, 6, 0};
    items[5] = {5, "Iron Shield", 0.25, 0, 5};
    items[6] = {6, "Tin Sword", 0.10, 4, 0};
    items[7] = {7, "Tin Shield", 0.14, 0, 3};
    items[8] = {8, "Tungsten Sword", 0.22, 8, 0};
    items[9] = {9, "Tungsten Shield", 0.30, 0, 7};
    items[10] = {10, "Copper Sword", 0.16, 5, 0};
    items[11] = {11, "Copper Shield", 0.22, 0, 4};
    // Tier Two
    items[12] = {12, "Double Edged Wood Sword", 0.11, 5, 0};
    items[13] = {13, "Reinforced Wood Shield", 0.15, 0, 4};
    items[14] = {14, "Double Edged Stone Sword", 0.14, 6, 0};
    items[15] = {15, "Reinforced Stone Shield", 0.22, 0, 5};
    items[16] = {16, "Double Edged Iron Sword", 0.20, 8, 0};
    items[17] = {17, "Reinforced Iron Shield", 0.27, 0, 7};
    items[18] = {18, "Double Edged Tin Sword", 0.12, 5, 0};
    items[19] = {19, "Reinforced Tin Shield", 0.16, 0, 4};
    items[20] = {20, "Double Edged Tungsten Sword", 0.26, 10, 0};
    items[21] = {21, "Reinforced Tungsten Shield", 0.34, 0, 9};
    items[22] = {22, "Double Edged Copper Sword", 0.19, 7, 0};
    items[23] = {23, "Reinforced Copper Shield", 0.26, 0, 6};

    // Armor
    items[100] = {100, "Wooden Greaves", 0.14};
    items[101] = {101, "Wooden Chestplate", 0.20};
    items[102] = {102, "Stone Greaves", 0.}

    items[999] = {999, "Nothing", 0.0, 0, 0};
}

vector<Items> itemVector(){
    vector<Item> itemsVector;
    for (const auto& pair : items) {
        itemsVector.push_back(pair.second);
    }
    return itemsVector;
}


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
   Item greaves;
   Item chestplate;
   double exp;

    void addItem(int id, Player& player){
        player.inventory.push_back(id);
    }

void levelUp(Player& player) {
    // Example threshold calculation for experience
    const double threshold = pow(1.3, player.level);
    if (player.exp <= threshold) {
        return;
    }

    player.level += 1;

    // Damage calculation
    if (player.level > 0) {
        player.damage = (rand() % 5) * (player.health / player.level);
    }

    int prevHp = player.health;

    // Ensure health is incremented reasonably
    player.health = prevHp + player.level * (rand() % 10 + 1);

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

    void saveGameData(const Game& game, const string& filename) {
        ofstream file(filename, ios::binary);

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
    
    Game loadGameData(const string& filename) {
        ifstream file(filename, ios::binary);
        Game game;

        // Deserialize player
        size_t nameSize;
        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        game.player.name.resize(nameSize);
        file.read(&game.player.name[0], nameSize);
        file.read(reinterpret_cast<char*>(&game.player.health), sizeof(game.player.health));
        file.read(reinterpret_cast<char*>(&game.player.damage), sizeof(game.player.damage));
        file.read(reinterpret_cast<char*>(&game.player.level), sizeof(game.player.level));
        file.read(reinterpret_cast<char*>(&game.player.blocked), sizeof(game.player.blocked));

        // Deserialize player inventory
        size_t invSize;
        file.read(reinterpret_cast<char*>(&invSize), sizeof(invSize));
        game.player.inventory.resize(invSize);
        file.read(reinterpret_cast<char*>(game.player.inventory.data()), invSize * sizeof(int));

        // Deserialize items
        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        game.player.sword.name.resize(nameSize);
        file.read(&game.player.sword.name[0], nameSize);
        file.read(reinterpret_cast<char*>(&game.player.sword.id), sizeof(game.player.sword.id));
        file.read(reinterpret_cast<char*>(&game.player.sword.damage), sizeof(game.player.sword.damage));
        file.read(reinterpret_cast<char*>(&game.player.sword.block), sizeof(game.player.sword.block));
        file.read(reinterpret_cast<char*>(&game.player.sword.weight), sizeof(game.player.sword.weight));

        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        game.player.shield.name.resize(nameSize);
        file.read(&game.player.shield.name[0], nameSize);
        file.read(reinterpret_cast<char*>(&game.player.shield.id), sizeof(game.player.shield.id));
        file.read(reinterpret_cast<char*>(&game.player.shield.damage), sizeof(game.player.shield.damage));
        file.read(reinterpret_cast<char*>(&game.player.shield.block), sizeof(game.player.shield.block));
        file.read(reinterpret_cast<char*>(&game.player.shield.weight), sizeof(game.player.shield.weight));

        // Deserialize player experience
        file.read(reinterpret_cast<char*>(&game.player.exp), sizeof(game.player.exp));

        // Deserialize game attributes
        file.read(reinterpret_cast<char*>(&game.playerHealth), sizeof(game.playerHealth));
        file.read(reinterpret_cast<char*>(&game.playerDamage), sizeof(game.playerDamage));
        file.read(reinterpret_cast<char*>(&game.playerLevel), sizeof(game.playerLevel));

        // Deserialize player name
        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        game.playerName.resize(nameSize);
        file.read(&game.playerName[0], nameSize);

        return game;
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