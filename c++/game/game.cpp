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
#include "inc.hpp"
#include "items.hpp"

using namespace std;

Game newGame(string name) {
    Game game;

    game.playerHealth = 10;
    game.playerDamage = 3;
    game.playerLevel = 1;
    game.playerName = name;

    return game;
}

char** readFile(const std::string& fileName, int& lineCount);

Entity genEnemy(int lvl) {
    string enemyName;
    int enemyLvl = rand() % lvl + (lvl * 1.5); 
    int health = enemyLvl * (rand() % 10);
    int damage = enemyLvl * ((rand() % 10) * 0.2);
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

int generateItem(Player player) {
    int id = rand() % TOTAL_ITEMS;
    return id;
}


void outputStats(Player& player) {
    int damage = player.damage + player.sword.damage;
    int defense = player.shield.block;
    cout << player.name << "'s stats:\n" << "\nLevel: " << YELLOW << to_string(player.level) << RESET << "\nExperience: " << CYAN << to_string(player.exp) << RESET << "\nHealth: " << RED << to_string(player.health) << RESET << "\nDamage: " << MAGENTA << to_string(damage) << RESET << "\nDefense: " << BLUE << to_string(player.shield.block) << RESET << "\nSword: " << GREEN << player.sword.name << RESET << "\nShield: " << GREEN << player.shield.name << RESET << endl;
}

int fight(Player& player) {
    int hp = player.health;
    int damage = player.damage + player.sword.damage;
    vector<int> inv = player.inventory;
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

        if(expCalc == false){
            for (int i = 0; i < enemies.size(); ++i) {
                exp = (enemies[i].level / ((rand() % 4) + 1.0));
            }
        }

        for (int i = 0; i < enemies.size(); ++i) {
            cout << "A " << enemies[i].name << " has appeared!\n" << RED << enemies[i].health << "HP" << RESET << " | " << MAGENTA << enemies[i].damage << "DMG" << RESET << " | " << YELLOW << enemies[i].level << "LVL\n" << RESET << "1. Block\n2. Attack\n";
                        // Check if enemy is alive before attacking
            if (enemies[i].health > 0) {
                // Enemy attacks the player
                int enemyDamage = enemies[i].damage;
                if (!((enemyDamage - player.shield.block) < 0)){
                    player.health -= (enemyDamage - player.shield.block);
                }
                cout << "The " << enemies[i].name << " attacks you for " << enemyDamage << " damage!\n\n\n";

                outputStats(player);
                ENDL

            }
            cin >> input;

            switch(input){
                case 1: {
                    enemies[i].blocked = true;
                    break;
                }
                case 2: {
                    // Miss calculation
                    if((rand() % 9) - player.sword.weight - player.shield.weight > 0){
                        enemies[i].health -= damage + player.sword.damage;
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
            //this_thread::sleep_for(chrono::seconds(3));
            system("clear");

            if (enemies[i].health < 1 ){
                enemies.erase(enemies.begin() + i);
            }
        }

        if (player.health <= 0){
            system("clear");
            cout << GAME_OVER << "You lost all your health and were sent back to Tartarus" << endl;
            return 0;
        }
    }

    player.inventory.push_back(generateItem(player));
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

void outputInv(const vector<int>& inventory) {
    for (size_t counter = 0; counter < inventory.size(); ++counter) {
        cout << counter + 1 << ": " << items[inventory[counter]].name << endl;
    }
}

// Function to check if the choice is valid (within the inventory range)
bool isValidChoice(int choice, size_t inventorySize) {
    return choice >= 0 && choice < inventorySize;
}

// Function to use food items
bool useFood(Item& item, Player& player) {
    if (Food* food = dynamic_cast<Food*>(&item)) {
        player.health += food->healAmount;
        return true; // Food item used
    }
    return false; // Not a food item
}

// Function to use equipment items (sword, shield, armor)
bool useEquipment(Item& item, Player& player) {
    if (item.damage > 0) {
        player.sword = item;
        return true; // Sword equipped
    } else if (item.block > 0) {
        if (item.name.find("Armor") != std::string::npos) {
            player.armor = item;
        } else {
            player.shield = item;
        }
        return true; // Armor or shield equipped
    }
    return false; // Not an equipment item
}

// Function to handle selecting and using items from the inventory
void useSelectedItem(int choice, Player& player, std::vector<Item>& items) {
    if (isValidChoice(choice, player.inventory.size())) {
        Item& selectedItem = items[player.inventory[choice]];

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
    bool gameOver = false;

    system("clear");
    int option;
    cout << INTRO_1 << INTRO_2 << "\n" << MENU_MAIN;
    cin >> option;
    switch (option) {
        case 1:
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
    

    player.health = game.playerHealth;
    player.damage = game.playerDamage;
    player.level = game.playerLevel;
    player.name = game.playerName;
    player.inventory = game.playerInv;
    player.sword = game.playerSword;
    player.shield = game.playerShield;
    player.exp = 0;

    while (true) {
        string explorationResult;

        if (player.health <= 0){
            system("clear");
            gameOver = true;
            cout << GAME_OVER << "You lost all your health and were sent back to Tartarus" << endl;
            return 0;
        }

        player.levelUp(player);
        initializeItems();

        outputStats(player);
        cout << "\n\n" << MENU_ACTION;
        cin >> option;

        system("clear");

        switch (option) {
            case 1: {
                explorationResult = explore();
                break;
            }
            case 2:
                outputInv(player.inventory);
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
                break;
            case 3: {
                outputInv(player.inventory);
                cout << "Choose an item: ";
                int choice;
                cin >> choice;

                choice--;

                useSelectedItem(choice, player, itemVector);
                system("clear");
                break;
            }
            default:
                cout << "Invalid option.\n";
                break;
        }
        

        if (explorationResult == "chest") {
            int gendItem = generateItem(player);
            player.inventory.push_back(gendItem);
            cout << "You found a chest! It contains a " << GREEN << items[gendItem].name << RESET << endl;
            cin.ignore();
        } else if (explorationResult == "enemy"){
            fight(player);
        } else if (explorationResult == "trap"){
            system("clear");
            int damage = (rand() % 10) * (player.health / 10);
            cout << "Oh no there was a trap!\nYou took " << RED << damage << RESET << " damage\n";
            player.health -= damage;
            cin.ignore();
        } else if (explorationResult == "nothing"){
            cout << "Nothing happened and you continue on your journey.\n";
            cin.ignore();
        }
    }

    return 0;
}
