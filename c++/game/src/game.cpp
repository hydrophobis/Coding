#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <algorithm>
#include <iterator>
#include <limits> // For std::numeric_limits
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::seconds

#include "def.hpp"
#include "func.hpp"

#define STAT_BASE_DAMAGE 3
#define STAT_BASE_HEALTH 10

using namespace std;

Game newGame(string name) {
    Game game;
    game.player.health = 10;
    game.player.healthMax = 10;
    game.player.damage = 3;
    game.player.level = 1;
    game.player.name = name;
    game.player.exp = 0.0;

    return game;
}

void saveGame(Game game) {
    FileManager fileManager;
    std::string filename = "savegame.dat";
    fileManager.saveGameData(game, filename);
}

Game loadGame() {
    Game game;
    FileManager fileManager;
    std::string filename = "savegame.dat";
    fileManager.loadGameData(game, filename);
    return game;
}


char** readFile(const std::string& fileName, int& lineCount);

Entity genEnemy(int lvl) {
    string enemyName;
    unsigned int enemyLvl = rand() % lvl + (lvl * 1.5); 
    int health = enemyLvl * ((rand() % 10) * 0.2);
    int damage = enemyLvl * ((rand() % 10) * 0.1);
    double multi;

    int name = rand() % 4;
    switch (name) {
        case 0:
            enemyName = "Goblin";
            multi = 0.5;
            break;
        case 1:
            enemyName = "Orc";
            multi = 1.5;
            break;
        case 2:
            enemyName = "Troll";
            multi = 1.25;
            break;
        case 3:
            enemyName = "Dragon";
            multi = 2.0;
            break;
        default:
            enemyName = "Unknown";
            multi = 1.0;
            break;
    }

    health = (health + 1) * multi;
    damage = damage * multi;
    
    Entity entity = {enemyName, health, damage, enemyLvl};
    return entity;
}


int generateItem() {
    std::vector<Item> vecI = initializeItems();
    int size = vecI.size(); // Get the size of the item vector
    int index = rand() % size; // Generate a random index

    return index;
}


void outputStats(Player& player) {
    cout << player.name << "'s stats:\n" << "\nLevel: " << YELLOW << to_string(player.level) << RESET << "\nExperience: " << CYAN << to_string(player.exp) << RESET << "\nHealth: " << RED << to_string(player.health) << " / " << to_string(player.healthMax) << RESET << "\nDamage: " << MAGENTA << to_string(player.damage) << RESET << "\nDefense: " << BLUE << to_string(player.defense) << RESET << "\nWeight: " << BRIGHT_YELLOW << player.weight << RESET << "\nSword: " << GREEN << player.sword.name << RESET << "\nShield: " << GREEN << player.shield.name << RESET << "\nChestplate: " << GREEN << player.chestplate.name << RESET << "\nGreaves: " << GREEN << player.greaves.name << RESET << endl;
}

double updateStats(Game& game, Player& player) {
    player.weight = player.sword.weight + player.greaves.weight + player.shield.weight + player.chestplate.weight;

    // Calculate damage reduction per weight unit
    double damageReductionPerUnit = 0.25;
    double totalDamageReduction = damageReductionPerUnit * player.weight;

    // Calculate the reduction factor based on weight
    double reductionFactor = 1.0 - totalDamageReduction;

    // Apply damage reduction to player's damage
    player.damage = static_cast<int>((player.sword.damage + STAT_BASE_DAMAGE) * reductionFactor);

    // Update player's defense
    player.defense = player.greaves.block + player.chestplate.block + player.shield.block;

    // Ensure player's health does not exceed maximum health
    if (player.health > player.healthMax) {
        player.health = player.healthMax;
    }

    // Update game's player stats
    game.player.health = player.health;
    game.player.healthMax = player.healthMax;
    game.player.damage = player.damage;
    game.player.inventory = player.inventory;
    game.player.sword = player.sword;
    game.player.shield = player.shield;
    game.player.greaves = player.greaves;
    game.player.chestplate = player.chestplate;
    game.player.exp = player.exp;
    game.player.level = player.level;

    return totalDamageReduction; // Return the total damage reduction applied
}


int fight(Game& game, Player& player) {
    int hp = player.health;
    int baseDamage = player.damage; // Include sword damage in base damage
    vector<unsigned int> inv = player.inventory;
    int input;
    vector<Entity> enemies;
    bool expCalc = false;
    bool blocking = false;
    double exp;

    int numEnemies = rand() % 1 + 1; // Ensure at least one enemy
    for (int i = 0; i < numEnemies; ++i) {
        Entity enemy = genEnemy(player.level);
        enemies.push_back(enemy);
    }

    system("clear");

    while (!enemies.empty()) {
        updateStats(game, player);
        if (!expCalc) {
            for (int i = 0; i < enemies.size(); ++i) {
                exp = (enemies[i].level / ((rand() % 8) + 1.0));
            }
            expCalc = true;
        }

        for (int i = 0; i < enemies.size(); ++i) {
            cout << "A " << enemies[i].name << " has appeared!\n" << RED << enemies[i].health << "HP" << RESET << " | " << MAGENTA << enemies[i].damage << "DMG" << RESET << " | " << YELLOW << enemies[i].level << "LVL\n" << RESET << "1. Block\n2. Attack\n";

            if (enemies[i].health > 0) {
                int enemyDamage = enemies[i].damage;
                signed int zero = 0;

                enemyDamage = max(zero, (enemyDamage - static_cast<int>(player.defense)));
                if (!((enemyDamage - player.shield.block) < 0)) {
                    player.health -= (enemyDamage - static_cast<int>(player.defense));
                }
                cout << "The " << enemies[i].name << " attacks you for " << RED << max(zero, (enemyDamage - static_cast<int>(player.defense))) << RESET << " damage!\n\n\n";

                outputStats(player);
                ENDL

            }
            cin >> input;

            switch (input) {
                case 1: {
                    enemies[i].blocked = true;
                    break;
                }
                case 2: {
                    // Calculate player's total damage
                    if ((rand() % 9) - player.weight > 0) {
                        enemies[i].health -= player.damage;
                        cout << "You attacked the " << enemies[i].name << " for " << baseDamage << " damage!\n";
                    } else {
                        cout << "\nYou missed!\n";
                    }
                    break;
                }
                default: {
                    cout << "Invalid choice" << endl;
                    break;
                }
            }

            if (enemies[i].health < 1) {
                enemies.erase(enemies.begin() + i);
            }
        }

        if (player.health <= 0) {
            system("clear");
            cout << GAME_OVER << "You lost all your health and were sent back to Tartarus" << endl;
            return 0;
        }

        system("clear");
    }

    int gendItemIndex = generateItem(); // Generate the item index
    if (gendItemIndex != -1) {
        player.inventory.push_back(gendItemIndex); // Push back the index to the inventory
    }

    player.exp += exp;

    system("clear");
    return 0;
}


string explore() {
    int r = rand() % 5;

    switch (r) {
        case 0:
            return "chest";
        case 1:
            return "enemy";
        case 2:
            return "chest";
        case 3:
            return "trap";
        case 4:
            return "enemy";
        default:
            return "err";
    }
}

vector<int> inventoryB(vector<int> inv) {
    vector<int> list;
    for (size_t i = 0; i < inv.size(); ++i) {
        list.push_back(inv[i]);
    }
    return list;
}

void outputInv(const vector<unsigned int>& inventory, const vector<Item>& items) {
    for (size_t counter = 0; counter < inventory.size(); ++counter) {
        int index = inventory[counter];
        if (index >= 0 && index < items.size()) {
            cout << counter + 1 << ": " << items[index].name << endl;
        } else {
            cout << "Invalid item index: " << index << endl;
        }
    }
}

// Function to check if the choice is valid (within the inventory range)
bool isValidChoice(int choice, size_t inventorySize) {
    return choice >= 0 && choice < inventorySize;
}

// Function to use food items
bool useFood(Item& item, Player& player) {
    int in;
    if (item.food == true) {
        player.health = player.health + item.healAmount;
        return true; // Food item used
    }
    return false; // Not a food item
}

// Function to use equipment items (sword, shield, armor)
bool useEquipment(Item& item, Player& player) {
    if ( contains(item.name, "Sword")) {
        player.sword = item;
        return true; // Sword equipped
    } else if ( contains(item.name, "Greaves")) {
        player.greaves = item;
        return true; // Greaves equipped
    } else if ( contains(item.name, "Chestplate") ){
        player.chestplate = item;
        return true; // Chestplate equipped
    } else if ( contains(item.name, "Shield")){
        player.shield = item;
        return true; // Shield equipped       
    } else {
        cout << "Cannot equip unknown item: " << item.name << endl;
        return false; // Not an equipment item
    }
}


// Function to handle selecting and using items from the inventory
void useSelectedItem(int choice, Player& player, std::vector<Item>& items) {
    if (isValidChoice(choice, player.inventory.size())) {
        Item& selectedItem = items[player.inventory[choice]];
        cout << "Using item: " << selectedItem.name << endl;

        if (useFood(selectedItem, player)) {
            // Food item used
            player.inventory.erase(player.inventory.begin() + choice);
        } else if (useEquipment(selectedItem, player)) {
            // Equipment item used (sword, shield, armor)
            player.inventory.erase(player.inventory.begin() + choice);
        } else {
            std::cout << "\nInvalid Item" << std::endl;
        }
    } else {
        std::cout << "Invalid choice" << std::endl;
    }
}






int main() {
    Game game;
    Player player;
    vector<Item> items = initializeItems();
    bool gameOver = false;

    system("clear");
    int option;
    cout << INTRO_1 << INTRO_2 << "\n" << MENU_MAIN;
    cin >> option;
    switch (option) {
        case 1:
            system("clear");
            FileManager fm;
            fm.loadGameData(game, "savegame.dat");
            break;
        case 2: {
            string name;
            cout << endl << "Player Name: ";
            cin >> name;
            system("clear");
            game = newGame(name);
            break;
        }
        case 3:
            return 0;
        default:
            cout << "Invalid option.\n";
            return 1;
    }

    time_t randomSeed = time(0);
    srand(randomSeed);

    // Initialize player stats
    player.health = game.player.health;
    player.healthMax = game.player.healthMax;
    player.damage = game.player.damage;
    player.defense = 0;
    player.level = game.player.level;
    player.name = game.player.name;
    player.exp = game.player.exp;
    player.weight = 0;
    player.inventory = game.player.inventory;
    player.sword = game.player.sword;
    player.shield = game.player.shield;
    player.chestplate = game.player.chestplate;
    player.greaves = game.player.greaves;

    while (true) {
        string explorationResult;

        if (player.health <= 0) {
            system("clear");
            gameOver = true;
            cout << GAME_OVER << "You lost all your health and were sent back to Tartarus" << endl;
            return 0;
        }

        updateStats(game, player);
        player.levelUp(player);

        outputStats(player);
        cout << "\n\n" << MENU_ACTION;

        string input;
        cin >> input;

        try {
            option = stoi(input);
        } catch (invalid_argument&) {
            cout << "Invalid option. Please enter a number.\n";
            continue;
        } catch (out_of_range&) {
            cout << "Invalid option. Please enter a valid number.\n";
            continue;
        }

        system("clear");

        switch (option) {
            case 1: {
                explorationResult = explore();
                break;
            }
            case 2:
                outputInv(player.inventory, items);
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                break;
            case 3: {
                outputInv(player.inventory, items);
                cout << "Choose an item: ";
                int choice;
                cin >> choice;

                if (cin.fail()) {
                    cout << "Invalid choice. Please enter a number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                choice--;

                useSelectedItem(choice, player, items);
                system("clear");
                break;
            }
            case 9: {
                FileManager fm;
                fm.saveGameData(game, "savegame.dat");
            }
            default:
                cout << "Invalid option.\n";
                break;
        }

        if (explorationResult == "chest") {
            int gendItem = generateItem();
            player.inventory.push_back(gendItem);
            cout << "You found a chest! It contains a " << GREEN << items[gendItem].name << RESET << endl;
            cin.ignore();
        } else if (explorationResult == "enemy") {
            fight(game, player);
        } else if (explorationResult == "trap") {
            system("clear");
            int damage = (rand() % 10) * (player.health / 10);
            cout << "Oh no there was a trap!\nYou took " << RED << damage << RESET << " damage\n";
            player.health -= damage;
            cin.ignore();
        } else if (explorationResult == "nothing") {
            cout << "Nothing happened and you continue on your journey.\n";
            cin.ignore();
        }
    }

    return 0;
}
