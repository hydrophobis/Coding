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
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/variant.hpp>

using namespace std;

struct Entity {
    string name;
    int health;
    int damage;
    unsigned int level;
    unsigned int defense;
    bool blocked;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(name, health, damage, level, defense, blocked);
    }
};

class Item {
public:
    string name;
    int id;
    int damage;
    int block;
    double weight;
    bool food;
    int healAmount;

    Item() : id(-1), name(""), weight(0.0), damage(0), block(0), food(false), healAmount(0) {}
    Item(int i, const string& n, double w, int d = 0, int b = 0, bool f = false, int heal = 0)
        : id(i), name(n), weight(w), damage(d), block(b), food(f), healAmount(heal) {}

    bool operator==(const Item& other) const {
        return id == other.id;
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(name, id, damage, block, weight, food, healAmount);
    }
};

struct Effect {
    Entity appliedEntity;
    string name;
    int id;

    struct EffectDPS {
        int damage;

        void apply(Entity& entity) {
            entity.health -= damage;
        }

        template <class Archive>
        void serialize(Archive& ar) {
            ar(damage);
        }
    };

    struct EffectBuff {
        string stat;
        double multiplier; // 0.9 for -10%

        void apply(Entity& entity, const unordered_map<string, variant<int Entity::*, double Entity::*>>& attributeMap, unordered_map<string, double>& originalValues) const {
            auto it = attributeMap.find(stat);
            if (it != attributeMap.end()) {
                visit([&entity, this, &originalValues](auto&& memberPtr) {
                    using T = decay_t<decltype(memberPtr)>;
                    if constexpr (is_same_v<T, int Entity::*>) {
                        originalValues[stat] = static_cast<double>(entity.*memberPtr);
                        entity.*memberPtr = static_cast<int>(entity.*memberPtr * multiplier);
                    } else if constexpr (is_same_v<T, double Entity::*>) {
                        originalValues[stat] = entity.*memberPtr;
                        entity.*memberPtr = entity.*memberPtr * multiplier;
                    }
                }, it->second);
            }
        }

        void unapply(Entity& entity, const unordered_map<string, variant<int Entity::*, double Entity::*>>& attributeMap, const unordered_map<string, double>& originalValues) const {
            auto it = attributeMap.find(stat);
            if (it != attributeMap.end()) {
                auto origIt = originalValues.find(stat);
                if (origIt != originalValues.end()) {
                    visit([&entity, &origIt](auto&& memberPtr) {
                        using T = decay_t<decltype(memberPtr)>;
                        if constexpr (is_same_v<T, int Entity::*>) {
                            entity.*memberPtr = static_cast<int>(origIt->second);
                        } else if constexpr (is_same_v<T, double Entity::*>) {
                            entity.*memberPtr = origIt->second;
                        }
                    }, it->second);
                }
            }
        }

        template <class Archive>
        void serialize(Archive& ar) {
            ar(stat, multiplier);
        }
    };

    template <class Archive>
    void serialize(Archive& ar) {
        ar(appliedEntity, name, id);
    }
};

struct Orb : public Item {
    static Effect effect;
    string story;

    void tellStory() {
        system("clear");
        cout << story;
        cout << "\n1. Done";
    }

    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<Item>(this), story);
    }
};

class Player : public Entity {
public:
    unsigned int healthMax;
    double weight;
    vector<unsigned int> inventory;
    Item sword;
    Item shield;
    Item greaves;
    Item chestplate;
    double exp;
    Effect effects[3];

    void addItem(int id) {
        inventory.push_back(id);
    }

    void levelUp(Player& player) {
        const double threshold = pow(1.3, player.level);
        if (player.exp <= threshold) {
            return;
        }

        player.level += 1;

        if (level > 0) {
            player.damage = (rand() % 5) * (player.health / player.level);
        }

        int prevHp = player.healthMax;
        player.healthMax = prevHp + player.level * (rand() % 10 + 1);
        if ((player.health * 2) < player.healthMax){
            player.health = player.healthMax / 2;
        }
    }

    void useItem(int id) {
        find(inventory.begin(), inventory.end(), id);
    }

    using Attribute = variant<int Player::*, double Player::*, Item Player::*, unsigned int Player::*>;

    unordered_map<string, variant<int Player::*, double Player::*, Item Player::*, unsigned int Player::*>> attributeMap = {
        {"health", static_cast<int Player::*>(&Player::health)},
        {"healthMax", static_cast<unsigned int Player::*>(&Player::healthMax)},
        {"damage", static_cast<int Entity::*>(&Entity::damage)},
        {"defense", static_cast<unsigned int Entity::*>(&Entity::defense)},
        {"weight", static_cast<double Player::*>(&Player::weight)},
        {"exp", static_cast<double Player::*>(&Player::exp)},
        {"sword", static_cast<Item Player::*>(&Player::sword)},
        {"shield", static_cast<Item Player::*>(&Player::shield)},
        {"chestplate", static_cast<Item Player::*>(&Player::chestplate)},
        {"greaves", static_cast<Item Player::*>(&Player::greaves)},
    };


    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<Entity>(this), healthMax, weight, inventory, sword, shield, greaves, chestplate, exp, effects);
    }
};

struct Game {
    Player player;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(player);
    }
};

// FileManager class with save and load functions using Cereal
class FileManager {
public:
    void saveGameData(const Game& game, const string& filename) {
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error opening file for writing: " << filename << endl;
            return;
        }

        cereal::BinaryOutputArchive archive(file);
        archive(game);

        // Debug output
        const Player& player = game.player;
        cout << "Saved game data:\n";
        cout << "Name: " << player.name << "\nHealth: " << player.health << "\nMax Health: " << player.healthMax << "\nWeight: " << player.weight << "\n";
    }

    void loadGameData(Game& game, string filename) {
        cout << "1";
        ifstream file; cout << "1,2";
        file.open("savegame.dat", std::ios::binary); cout << "1,3";
        /*if (!file) {
            cerr << "Error opening file for reading: " << filename << endl;
            return;
        }*/
        cout << "2";
        cereal::BinaryInputArchive archive(file);
        archive(game);
        cout << "3";

        // Debug output
        const Player& player = game.player;
        cout << "Loaded game data:\n";
        cout << "Name: " << player.name << "\nHealth: " << player.health << "\nMax Health: " << player.healthMax << "\nWeight: " << player.weight << "\n";
    }
};
