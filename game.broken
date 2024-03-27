#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

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
    vector<string> inventory;

    Player(int health, int attack) : Character(health, attack), maxHealth(health), level(1) {}

    void display() const override {
        cout << "Player (Level " << level << "): " << health << " health, " << attack << " attack" << endl;
        cout << "Inventory: ";
        for (const auto& item : inventory) {
            cout << item << " ";
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

    void addToInventory(const string& item) {
        inventory.push_back(item);
        if (item == "Better Sword") {
            // Increase player's attack when a better sword is obtained
            attack += 10;
            cout << "You equipped a Better Sword! Your attack increased.\n \n";
        }
    }

    void performSpecialAttack(Character& target) override {
        // Player's special attack: Double Attack
        cout << "Player performs a Double Attack!\n";
        target.takeDamage(2 * attack);
    }
};

class Enemy : public Character {
public:
    int level;
    string name;
    string specialAttackName;

    Enemy(int health, int attack, int level, const string& name, const string& specialAttackName)
        : Character(health, attack), level(level), name(name), specialAttackName(specialAttackName) {}

    void display() const override {
        cout << "Enemy: " << name << " (Level " << level << "): " << health << " health, " << attack << " attack" << endl;
    }

    void takeDamage(int damage) override {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }

    class Shop {
public:
    void visitShop(Player& player) {
        cout << "Welcome to the Shop!\n";
        cout << "1. Buy Healing Potion (10 Gold)\n2. Buy Upgrade (50 Gold)\n3. Leave Shop\n";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                if (player.inventoryContains("Gold", 10)) {
                    player.removeFromInventory("Gold", 10);
                    player.addToInventory("Healing Potion");
                    cout << "You bought a Healing Potion!\n";
                } else {
                    cout << "Not enough Gold!\n";
                }
                break;

            case 2:
                if (player.inventoryContains("Gold", 50)) {
                    player.removeFromInventory("Gold", 50);
                    player.upgradeWeapon();
                    cout << "You upgraded your weapon!\n";
                } else {
                    cout << "Not enough Gold!\n";
                }
                break;

            case 3:
                cout << "Thanks for visiting the Shop!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
};

void shopPhase(Player& player, Shop& shop) {
    shop.visitShop(player);
}

void playerTurn(Player& player, vector<Enemy>& enemies, vector<Trap>& traps, vector<Chest>& chests, vector<vector<char>>& dungeon, Shop& shop) {
    int choice;

    cout << "1. Explore\n2. Heal\n3. View Inventory\n4. Visit Shop\n5. Quit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            explorePhase(player, enemies, traps, chests, dungeon);
            break;

        case 2:
            player.heal();
            cout << "You have been healed.\n \n";
            break;

        case 3:
            player.display();
            break;

        case 4:
            shopPhase(player, shop);
            break;

        case 5:
            cout << "Thank you for playing! Goodbye.\n \n";
            exit(0);

        default:
            cout << "Invalid choice. Please try again.\n \n";
    }
}

    void performSpecialAttack(Character& target) override {
        cout << name << " performs " << specialAttackName << "!\n";
        // Unique special attacks for each enemy
        if (specialAttackName == "Fireball") {
            target.takeDamage(1.5 * attack);
        } else if (specialAttackName == "Ice Shard") {
            target.takeDamage(attack + 5);
        } else if (specialAttackName == "Venom Strike") {
            target.takeDamage(attack);
            cout << "You are poisoned!\n";
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

// New class for traps in the dungeon
class Trap {
public:
    string type;
    int damage;

    Trap(const string& type, int damage) : type(type), damage(damage) {}

    void trigger(Player& player) {
        cout << "You triggered a " << type << " trap! ";
        player.takeDamage(damage);
        cout << "You took " << damage << " damage.\n";
    }
};

// Function to generate a random event in the dungeon (e.g., trap or chest)
void generateRandomEvent(Player& player, vector<Trap>& traps, vector<Chest>& chests) {
    int randomEvent = rand() % 10; // Increase the likelihood of random events
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
                cout << "You found some Gold!\n \n";
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

    // Update dungeon map (mark as visited)
    dungeon[rand() % dungeon.size()][rand() % dungeon[0].size()] = 'V';

    // Player has successfully cleared the enemies
    cout << "You cleared the enemies! Continue exploring...\n \n";
    player.levelUp();
}

// Function to handle the player's turn during exploration
void playerTurn(Player& player, vector<Enemy>& enemies, vector<Trap>& traps, vector<Chest>& chests, vector<vector<char>>& dungeon) {
    int choice;

    cout << "1. Explore\n2. Heal\n3. View Inventory\n4. Quit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            explorePhase(player, enemies, traps, chests, dungeon);
            break;

        case 2:
            player.heal();
            cout << "You have been healed.\n \n";
            break;

        case 3:
            player.display();
            break;

        case 4:
            cout << "Thank you for playing! Goodbye.\n \n";
            exit(0);

        default:
            cout << "Invalid choice. Please try again.\n \n";
    }
}

void encounterBoss(Player& player, Boss& boss) {
    cout << "You've encountered a formidable Boss!\n";
    player.display();
    boss.display();

    int choice;
    cout << "1. Attack\n2. Use Special Attack\n3. Retreat\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        boss.takeDamage(player.attack);
        if (boss.health <= 0) {
            cout << "Congratulations! You defeated the Boss and cleared the dungeon!\n";
            exit(0);
        } else {
            boss.performSpecialAttack(player);
            if (player.health <= 0) {
                cout << "You were defeated by the Boss!\n";
                exit(0);
            }
        }
    } else if (choice == 2) {
        player.performSpecialAttack(boss);
        if (boss.health <= 0) {
            cout << "Congratulations! You defeated the Boss and cleared the dungeon!\n";
            exit(0);
        } else {
            boss.performSpecialAttack(player);
            if (player.health <= 0) {
                cout << "You were defeated by the Boss!\n";
                exit(0);
            }
        }
    } else {
        cout << "You successfully retreated from the Boss battle.\n";
    }
}

// Main function with enhanced features
int main() {
    srand(time(0));

    const int dungeonSize = 10;
    vector<vector<char>> dungeon(dungeonSize, vector<char>(dungeonSize, '.'));

    Player player(100, 5);
    vector<Enemy> enemies;
    vector<Trap> traps;
    vector<Chest> chests;
    Shop shop;
    Boss boss(300, 25);

    cout << "Welcome to the Enhanced Roguelite Dungeon Crawler!" << endl;

    do {
        for (int i = 0; i < dungeonSize; ++i) {
            for (int j = 0; j < dungeonSize; ++j) {
                cout << dungeon[i][j] << " ";
            }
            cout << endl;
        }

        playerTurn(player, enemies, traps, chests, dungeon, shop);

        // Generate random events during exploration
        generateRandomEvent(player, traps, chests);

        // Introduce Boss encounter after reaching a certain level
        if (player.level >= 5 && rand() % 2 == 0) {
            encounterBoss(player, boss);
        }

    } while (true);

    return 0;
}
