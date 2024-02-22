#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <stdexcept>

using namespace std;

// ANSI escape codes for text color
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

int except( int a, int b){
    a = 1;
    b = 1;
    if(a = b){
        throw invalid_argument("Failed");
    }
    return 0;
}

class Character {
public:
    int health;
    int attack;

    Character(int health, int attack) : health(health), attack(attack) {}

    virtual void display() const = 0;
    virtual void takeDamage(int damage) = 0;
    virtual void performSpecialAttack(Character& target) = 0;
};

class Player : public Character {
public:
    int maxHealth;
    int level;
    string inventory[255];

    Player(int health, int attack) : Character(health, attack), maxHealth(health), level(1) {}

    void display() const override {
        cout << CYAN << "Player (Level " << level << "): " << health << " health, " << attack << " attack" << RESET << endl;
        cout << "Inventory: ";
        for (const auto& item : inventory) {
            cout << RED << item << RESET << "," << " ";
        }
        cout << "\n";
    }

    void takeDamage(int damage) override {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }

    void heal() {
        health += maxHealth / 2;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    void levelUp() {
        level++;
        maxHealth += 10;
        attack += 5;
        heal();
    }

    void addToInventory(const string& item);

    void performSpecialAttack(Character& target) override {
        // Player's special attack: Double Attack
        cout << CYAN << "Player performs a Double Attack!" << RESET << endl;
        target.takeDamage(2 * attack);
    }

    void upgradeWeapon() {
        // Implement the logic for upgrading the player's weapon
        cout << CYAN << "You upgraded your weapon!" << RESET << endl;
        attack += 10; // Example: Increase attack when upgrading weapon
    }
};

class Boss : public Character {
public:
    string name;
    string specialAttackName;

    Boss(int health, int attack, const string& name, const string& specialAttackName)
        : Character(health, attack), name(name), specialAttackName(specialAttackName) {}

    void display() const override {
        cout << RED << "Boss: " << name << ": " << health << " health, " << attack << " attack" << RESET << endl;
    }

    void attackPlayer(Player& player);
    void performSpecialAttack(Character& target) override;

    // Implement the takeDamage function to resolve the abstract class issue
    void takeDamage(int damage) override {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }
};

class Shop {
public:
    void visitShop(Player& player);
};

class Enemy : public Character {
public:
    int level;
    string name;
    string specialAttackName;

    Enemy(int health, int attack, int level, const string& name, const string& specialAttackName)
        : Character(health, attack), level(level), name(name), specialAttackName(specialAttackName) {}

    void display() const override {
        cout << YELLOW << "Enemy: " << name << " (Level " << level << "): " << health << " health, " << attack << " attack" << RESET << endl;
    }

    void takeDamage(int damage) override {
        health -= damage;
        if ((health -= damage) < 0) {
            health = 0;
        } else {
            health -= damage;
        }
    }

    void attackPlayer(Player& player);

    void performSpecialAttack(Character& target) override {
        cout << YELLOW << name << " performs " << specialAttackName << "!" << RESET << endl;
        // Unique special attacks for each enemy
        if (specialAttackName == "Fireball") {
            target.takeDamage(1.5 * attack);
        } else if (specialAttackName == "Ice Shard") {
            target.takeDamage(attack + 5);
        } else if (specialAttackName == "Venom Strike") {
            target.takeDamage(attack);
            cout << "You are poisoned!" << endl;
        } else if (specialAttackName == "Thunderstorm") {
            target.takeDamage(attack - 3);
        } else if (specialAttackName == "Earthquake") {
            target.takeDamage(2 * attack);
        } else if (specialAttackName == "Dark Blast") {
            target.takeDamage(1.2 * attack);
        }
    }
};

class Chest {
public:
    string content;

    Chest() {
        int random = rand() % 3;
        if (random == 0) {
            content = "Better Sword";
        } else if (random == 1) {
            content = "Healing Potion";
        } else {
            content = "Gold";
        }
    }

    void display() const {
        cout << "You found a chest containing: " << content << endl;
    }
};

class Trap {
public:
    string type;
    int damage;

    Trap(const string& type, int damage) : type(type), damage(damage) {}

    void trigger(Player& player) {
        cout << RED << "You triggered a " << type << " trap! " << RESET;
        player.takeDamage(damage);
        cout << "You took " << damage << " damage." << endl;
    }
};

// Function to generate a random event in the dungeon (e.g., trap or chest)
void generateRandomEvent(Player& player, vector<Trap>& traps, vector<Chest>& chests) {
    int randomEvent = rand() % 15; // Increase the likelihood of random events
    if (randomEvent < 4) {
        // Player encounters a chest
        chests.emplace_back();
        player.display();
        chests.back().display();

        cout << "1. Open the chest\n2. Leave the chest\n";
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (chests.back().content == "Better Sword") {
                player.addToInventory("Better Sword");
            } else if (chests.back().content == "Healing Potion") {
                player.addToInventory("Healing Potion");
                player.heal();
            } else {
                cout << "You found some Gold!" << endl << endl;
            }
        }

        chests.pop_back();
    } else if (randomEvent < 7) {
        // Player encounters a trap
        int trapIndex = rand() % traps.size();
        traps[trapIndex].trigger(player);
    }
}

// Function to handle the exploration phase
void explorePhase(Player& player, vector<Enemy>& enemies, vector<Trap>& traps, vector<Chest>& chests, vector<vector<char>>& dungeon) {
    int enemyCount = rand() % 3 + 1;
    for (int i = 0; i < enemyCount; ++i) {
        enemies.emplace_back(50 + (player.level - 1) * 10, 15 + (player.level - 1) * 5, player.level,
                             "Enemy" + to_string(i + 1), "SpecialAttack" + to_string(i + 1));
    }

    for (auto& enemy : enemies) {
        player.display();
        enemy.display();

        cout << "1. Attack\n2. Use Special Attack\n3. Retreat\n";
        int choice;
        cin >> choice;

        if (choice == 1) {
            enemy.takeDamage(player.attack);
            if (enemy.health <= 0) {
                cout << "You defeated the enemy!" << endl;
                player.addToInventory("Potion");
            } else {
                enemy.attackPlayer(player);
                if (player.health <= 0) {
                    cout << "You were defeated by the enemy!" << endl;
                    exit(0);
                }
            }
        } else if (choice == 2) {
            player.performSpecialAttack(enemy);
            if (enemy.health <= 0) {
                cout << "You defeated the enemy!" << endl;
                player.addToInventory("Potion");
            } else {
                enemy.attackPlayer(player);
                if (player.health <= 0) {
                    cout << "You were defeated by the enemy!" << endl;
                    exit(0);
                }
            }
        } else {
            cout << "You successfully retreated from the battle." << endl;
        }
    }
    dungeon[rand() % dungeon.size()][rand() % dungeon[0].size()] = 'V';
    cout << "You cleared the enemies! Continue exploring..." << endl << endl;
    player.levelUp();
    system("clear");
}

// Function to handle the player's turn during exploration
void playerTurn(Player& player, vector<Enemy>& enemies, vector<Trap>& traps, vector<Chest>& chests, vector<vector<char>>& dungeon) {
    int choice;

    system("clear");

    cout << "1. Explore\n2. Heal\n3. View Inventory\n4. Quit\n5. Shop";
    cout << "\n" << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            explorePhase(player, enemies, traps, chests, dungeon);
            break;

        case 2:
            player.heal();
            cout << "You have been healed." << endl << endl;
            break;

        case 3:
            player.display();
            break;

        case 4:
            cout << "Thank you for playing! Goodbye." << endl << endl;
            exit(0);

        default:
            cout << "Invalid choice. Please try again." << endl << endl;
    }
}

void bossEncounter(Player& player, Boss& boss) {
    cout << "You encountered the Boss!" << endl;
    player.display();
    boss.display();

    int choice;
    cout << "1. Attack\n2. Use Special Attack\n3. Retreat\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        // Player attacks boss
        boss.takeDamage(player.attack);
        if (boss.health <= 0) {
            cout << "You defeated the Boss!" << endl;
            player.addToInventory("Ultimate Potion");  // Example: Add an ultimate potion to inventory after defeating the boss
        } else {
            // Boss counterattacks
            boss.attackPlayer(player);
            if (player.health <= 0) {
                cout << "You were defeated by the Boss!" << endl;
                exit(0);
            }
        }
    } else if (choice == 2) {
        // Player uses special attack against the boss
        player.performSpecialAttack(boss);

        // After player's special attack, check if the boss is defeated
        if (boss.health <= 0) {
            cout << "You defeated the Boss!" << endl;
            player.addToInventory("Ultimate Potion");  // Example: Add an ultimate potion to inventory after defeating the boss
        } else {
            // Boss counterattacks
            boss.attackPlayer(player);
            if (player.health <= 0) {
                cout << "You were defeated by the Boss!" << endl;
                exit(0);
            }
        }
    } else {
        cout << "You successfully retreated from the boss battle." << endl;
    }
}

void Boss::attackPlayer(Player& player) {
    cout << RED << name << " attacks the player!" << RESET << endl;
    player.takeDamage(attack);
    system("clear");
}

void Boss::performSpecialAttack(Character& target) {
    // Implement the special attack logic for the Boss
    // For example:
    cout << RED << name << " performs special attack: " << specialAttackName << " on the player!" << RESET << endl;
    target.takeDamage(2 * attack);
}

void Player::addToInventory(const string& item) {
    inventory->append(item);
    if (item == "Better Sword") {
        // Ensure that the attack increase is applied within reasonable bounds
        if (attack + 10 > 0) {
            attack += 10;
            cout << CYAN << "You equipped a Better Sword! Your attack increased." << RESET << endl << endl;
        } else {
            cout << CYAN << "You found a Better Sword, but your attack cannot go below 0." << RESET << endl << endl;
        }
    }
    system("clear");
}

void Enemy::attackPlayer(Player& player) {
    cout << YELLOW << name << " attacks the player!" << RESET << endl;
    player.takeDamage(attack);
    system("clear");
}

void Shop::visitShop(Player& player) {
    cout << "Welcome to the Shop!" << endl;
    cout << "1. Buy Healing Potion (10 Gold)\n2. Buy Upgrade (50 Gold)\n3. Leave Shop\n";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            if (find(player.inventory->begin(), player.inventory->end(), "Gold") != player.inventory->end() &&
                count(player.inventory->begin(), player.inventory->end(), "Gold") >= 10) {
                player.inventory->erase(find(player.inventory->begin(), player.inventory->end(), "Gold"),
                                       player.inventory->begin() + 10);
                player.addToInventory("Healing Potion");
                cout << "You bought a Healing Potion!" << endl;
            } else {
                cout << "Not enough Gold!" << endl;
            }
            break;

        case 2:
            if (find(player.inventory->begin(), player.inventory->end(), "Gold") != player.inventory->end() &&
                count(player.inventory->begin(), player.inventory->end(), "Gold") >= 50) {
                player.inventory->erase(find(player.inventory->begin(), player.inventory->end(), "Gold"),
                                       player.inventory->begin() + 50);
                player.upgradeWeapon();
                cout << "You upgraded your weapon!" << endl;
            } else {
                cout << "Not enough Gold!" << endl;
            }
            break;

        case 3:
            cout << "Thanks for visiting the Shop!" << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void clearScreen() {
    system("clear");
}

// Function declarations
void explorePhase(Player& player, vector<Enemy>& enemies, vector<Trap>& traps, vector<Chest>& chests, vector<vector<char>>& dungeon);
void playerTurn(Player& player, vector<Enemy>& enemies, vector<Trap>& traps, vector<Chest>& chests, vector<vector<char>>& dungeon);
void generateRandomEvent(Player& player, vector<Trap>& traps, vector<Chest>& chests);
void bossEncounter(Player& player, Boss& boss);

// Main function with enhanced features
int main() {
    srand(time(0));

    const int dungeonSize = 10;
    vector<vector<char>> dungeon(dungeonSize, vector<char>(dungeonSize, '.'));

    Player player(100, 20);
    vector<Enemy> enemies;
    vector<Trap> traps;
    vector<Chest> chests;
    Shop shop;
    Boss boss(200, 30, "Evil Overlord", "Dark Explosion");
    int choice;

    cout << "Welcome to the Enhanced Roguelite Dungeon Crawler!" << endl;

    do {
        system("clear");
        // Display the dungeon map
        for (int i = 0; i < dungeonSize; ++i) {
            for (int j = 0; j < dungeonSize; ++j) {
                cout << dungeon[i][j] << " ";
            }
            cout << endl;
        }

        player.display();

        if (!enemies.empty()) {
            cout << "Current Enemy: ";
            enemies.front().display();
        }

        cout << "1. Explore\n2. Heal\n3. View Inventory\n4. Shop\n5. Quit";
        cout << "\n" <<"Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                system("clear");
                int randomEvent = rand() % 5;
                if (randomEvent == 0) {
                    chests.emplace_back();
                    clearScreen();
                    player.display();
                    chests.back().display();

                    cout << "1. Open the chest\n2. Leave the chest\n";
                    cout << "Enter your choice: ";
                    cin >> choice;

                    if (choice == 1) {
                        if (chests.back().content == "Better Sword") {
                            player.addToInventory("Better Sword");
                        } else if (chests.back().content == "Healing Potion") {
                            player.addToInventory("Healing Potion");
                            player.heal();
                        } else {
                            cout << "You found some Gold!\n \n";
                        }
                    }

                    chests.pop_back();
                } else {
                    int enemyCount = rand() % 3 + 1;
                    for (int i = 0; i < enemyCount; ++i) {
                        enemies.emplace_back(50 + (player.level - 1) * 10, 15 + (player.level - 1) * 5, player.level,
                                             "Enemy" + to_string(i + 1), "SpecialAttack" + to_string(i + 1));
                    }

                    for (auto& enemy : enemies) {
                        system("clear");
                        player.display();
                        enemy.display();

                        cout << "1. Attack\n2. Use Special Attack\n3. Retreat\n";
                        cout << "Enter your choice: ";
                        cin >> choice;

                        system("clear");

                        if (choice == 1) {
                            enemy.takeDamage(player.attack);
                            if (enemy.health <= 0) {
                                cout << "You defeated the enemy!" << endl;
                                player.addToInventory("Gold");
                            } else {
                                enemy.attackPlayer(player);
                                if (player.health <= 0) {
                                    cout << "You were defeated by the enemy!" << endl;
                                    return 0;
                                }
                            }
                        } else if (choice == 2) {
                            player.performSpecialAttack(enemy);
                            if (enemy.health <= 0) {
                                cout << "You defeated the enemy!" << endl;
                                player.addToInventory("Potion");
                            } else {
                                enemy.attackPlayer(player);
                                if (player.health <= 0) {
                                    cout << "You were defeated by the enemy!" << endl;
                                    return 0;
                                }
                            }
                        } else {
                            cout << "You successfully retreated from the battle." << endl;
                        }
                    }
                    dungeon[rand() % dungeonSize][rand() % dungeonSize] = 'V';
                    cout << "You cleared the enemies! Continue exploring...\n \n";
                    player.levelUp();
                    system("clear");
                }
                break;
            }
            case 2:
                player.heal();
                cout << "You have been healed.\n \n";
                break;
            case 3:
                clearScreen();
                player.display();
                break;
            case 5:
                cout << "Thank you for playing! Goodbye.\n \n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n \n";
            case 4:
                cout << "error";

        generateRandomEvent(player, traps, chests);

        system("clear");

        if (player.level % 5 == 0) {
            bossEncounter(player, boss);
        }
    } while (true);

    return 0;
}