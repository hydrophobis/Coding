#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <variant>

#define TOTAL_ITEMS 24

using namespace std;

struct Entity {
    string name;
    int health;
    int damage;
    int level;
    int defense;
    bool blocked;
};

class Item {
public:

    virtual ~Item() {}

    string name;
    int id = -1;
    int damage = 0;
    int block = 0;
    double weight = 0.0;
    bool food = false;
    int healAmount = 0;

    Item() : id(-1), name(""), weight(0.0), damage(0), block(0) {}

    Item(int i, const string& n, double w, int d = 0, int b = 0, bool f = false, int heal = 0)
        : id(i), name(n), weight(w), damage(d), block(b), food(f), healAmount(heal) {}

    bool operator==(const Item& other) const {
        return id == other.id;
    }
};

struct Effect {
    Entity appliedEntity;
    string name;
    int id;

    struct EffectDPS {
        int damage;

        void apply(Entity entity){
            entity.health -= damage;
        }
    };

    struct EffectBuff {
        std::string stat;
        double multiplier; // 0.9 for -10%

        void apply(Entity& entity, const std::unordered_map<std::string, std::variant<int Entity::*, double Entity::*>>& attributeMap, std::unordered_map<std::string, double>& originalValues) const {
            auto it = attributeMap.find(stat);
            if (it != attributeMap.end()) {
                std::visit([&entity, this, &originalValues](auto&& memberPtr) {
                    using T = std::decay_t<decltype(memberPtr)>;
                    if constexpr (std::is_same_v<T, int Entity::*>) {
                        originalValues[stat] = static_cast<double>(entity.*memberPtr);
                        entity.*memberPtr = static_cast<int>(entity.*memberPtr * multiplier);
                    } else if constexpr (std::is_same_v<T, double Entity::*>) {
                        originalValues[stat] = entity.*memberPtr;
                        entity.*memberPtr = entity.*memberPtr * multiplier;
                    }
                }, it->second);
            }
        }

        void unapply(Entity& entity, const std::unordered_map<std::string, std::variant<int Entity::*, double Entity::*>>& attributeMap, const std::unordered_map<std::string, double>& originalValues) const {
            auto it = attributeMap.find(stat);
            if (it != attributeMap.end()) {
                auto origIt = originalValues.find(stat);
                if (origIt != originalValues.end()) {
                    std::visit([&entity, &origIt](auto&& memberPtr) {
                        using T = std::decay_t<decltype(memberPtr)>;
                        if constexpr (std::is_same_v<T, int Entity::*>) {
                            entity.*memberPtr = static_cast<int>(origIt->second);
                        } else if constexpr (std::is_same_v<T, double Entity::*>) {
                            entity.*memberPtr = origIt->second;
                        }
                    }, it->second);
                }
            }
        }

    };
};

struct Orb : public Item {
    static Effect effect;
    string story;

    void tellStory(){
        system("clear");
        cout << story;
        cout << "\n1. Done";
    }
};

class Player : public Entity {
public:
    int healthMax;
    double weight;
    vector<int> inventory;
    Item sword;
    Item shield;
    Item greaves;
    Item chestplate;
    double exp;
    Effect effects[3];

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

        int prevHp = player.healthMax;

        // Ensure health is incremented reasonably
        player.healthMax = prevHp + player.level * (rand() % 10 + 1);

        return;
    }


    void useItem(int id, Player& player){
        vector<int> inv = player.inventory;
        find(inv.begin(), inv.end(), id);
    }

    using Attribute = std::variant<int Player::*, double Player::*, Item Player::*>;

    std::unordered_map<std::string, Attribute> attributeMap = {
        {"health", &Player::health},
        {"healthMax", &Player::healthMax},
        {"damage", &Player::damage},
        {"defense", &Player::defense},
        {"weight", &Player::weight},
        {"exp", &Player::exp},
        {"sword", &Player::sword},
        {"shield", &Player::shield},
        {"chestplate", &Player::chestplate},
        {"greaves", &Player::greaves}
    };
};

struct Game {
    Player player;
};

// ChatGPT made this
class FileManager {
public:
    void saveGameData(const Game& game, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        const Player& player = game.player;

        cout << "0";

        // Serialize player
        size_t nameSize = player.name.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(player.name.c_str(), nameSize);
        file.write(reinterpret_cast<const char*>(&player.health), sizeof(player.health));
        file.write(reinterpret_cast<const char*>(&player.damage), sizeof(player.damage));
        file.write(reinterpret_cast<const char*>(&player.level), sizeof(player.level));

        // Serialize player inventory
        size_t invSize = player.inventory.size();
        file.write(reinterpret_cast<const char*>(&invSize), sizeof(invSize));
        file.write(reinterpret_cast<const char*>(player.inventory.data()), invSize * sizeof(int));

        // Serialize items
        serializeItem(player.sword, file);
        serializeItem(player.shield, file);
        serializeItem(player.greaves, file);
        serializeItem(player.chestplate, file);

        // Serialize player experience
        file.write(reinterpret_cast<const char*>(&player.exp), sizeof(player.exp));

        // Serialize player additional fields
        file.write(reinterpret_cast<const char*>(&player.healthMax), sizeof(player.healthMax));
        file.write(reinterpret_cast<const char*>(&player.weight), sizeof(player.weight));

        // Debug output
        std::cout << "Saved game data:\n";
        std::cout << "Name: " << player.name << "\nHealth: " << player.health << "\nMax Health: " << player.healthMax << "\nWeight: " << player.weight << "\n";
    }

void loadGameData(Game& game, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return;
    }
    
    Player& player = game.player;

    // Deserialize player
    size_t nameSize;
    file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    player.name.resize(nameSize);
    file.read(&player.name[0], nameSize);
    file.read(reinterpret_cast<char*>(&player.health), sizeof(player.health));
    file.read(reinterpret_cast<char*>(&player.damage), sizeof(player.damage));
    file.read(reinterpret_cast<char*>(&player.level), sizeof(player.level));

    // Deserialize player inventory
    size_t invSize;
    file.read(reinterpret_cast<char*>(&invSize), sizeof(invSize));
    player.inventory.resize(invSize);
    file.read(reinterpret_cast<char*>(player.inventory.data()), invSize * sizeof(int));

    // Deserialize items
    deserializeItem(player.sword, file);
    deserializeItem(player.shield, file);
    deserializeItem(player.greaves, file);
    deserializeItem(player.chestplate, file);

    // Deserialize player experience
    file.read(reinterpret_cast<char*>(&player.exp), sizeof(player.exp));

    // Deserialize player additional fields
    file.read(reinterpret_cast<char*>(&player.healthMax), sizeof(player.healthMax));
    file.read(reinterpret_cast<char*>(&player.weight), sizeof(player.weight));

    // Debug output
    std::cout << "Loaded game data:\n";
    std::cout << "Name: " << player.name << "\nHealth: " << player.health << "\nMax Health: " << player.healthMax << "\nWeight: " << player.weight << "\n";
}

private:
    void serializeItem(const Item& item, std::ofstream& file) {
        size_t nameSize = item.name.size();
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(item.name.c_str(), nameSize);
        file.write(reinterpret_cast<const char*>(&item.id), sizeof(item.id));
        file.write(reinterpret_cast<const char*>(&item.damage), sizeof(item.damage));
        file.write(reinterpret_cast<const char*>(&item.block), sizeof(item.block));
        file.write(reinterpret_cast<const char*>(&item.weight), sizeof(item.weight));
        file.write(reinterpret_cast<const char*>(&item.food), sizeof(item.food));
        file.write(reinterpret_cast<const char*>(&item.healAmount), sizeof(item.healAmount));
    }

    void deserializeItem(Item& item, std::ifstream& file) {
        size_t nameSize;
        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        if (file.fail()) { std::cerr << "Error reading item name size" << std::endl; return; }
        item.name.resize(nameSize);
        file.read(&item.name[0], nameSize);
        if (file.fail()) { std::cerr << "Error reading item name" << std::endl; return; }
        file.read(reinterpret_cast<char*>(&item.id), sizeof(item.id));
        if (file.fail()) { std::cerr << "Error reading item id" << std::endl; return; }
        file.read(reinterpret_cast<char*>(&item.damage), sizeof(item.damage));
        if (file.fail()) { std::cerr << "Error reading item damage" << std::endl; return; }
        file.read(reinterpret_cast<char*>(&item.block), sizeof(item.block));
        if (file.fail()) { std::cerr << "Error reading item block" << std::endl; return; }
        file.read(reinterpret_cast<char*>(&item.weight), sizeof(item.weight));
        if (file.fail()) { std::cerr << "Error reading item weight" << std::endl; return; }
        file.read(reinterpret_cast<char*>(&item.food), sizeof(item.food));
        if (file.fail()) { std::cerr << "Error reading item food" << std::endl; return; }
        file.read(reinterpret_cast<char*>(&item.healAmount), sizeof(item.healAmount));
        if (file.fail()) { std::cerr << "Error reading item healAmount" << std::endl; return; }
    }
};