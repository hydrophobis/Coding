#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <algorithm>
#include <iterator>

#include "def.hpp"
#include "inc.hpp"

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

    health = health * multi;
    damage = damage * multi;
    
    Entity entity = {enemyName, health, damage, enemyLvl};
    return entity;
}

int generateItem(Player player) {
    int id = rand() % TOTAL_ITEMS + 1;
    return id;
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
                exp = (enemies[i].level / 5.0);
            }
        }

        for (int i = 0; i < enemies.size(); ++i) {
            cout << "A " << enemies[i].name << " has appeared!\n1. Block\n2. Attack";
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

            // Check if enemy is alive before attacking
            if (enemies[i].health > 0) {
                // Enemy attacks the player
                int enemyDamage = enemies[i].damage;
                player.health -= enemyDamage;
                cout << "The " << enemies[i].name << " attacks you for " << enemyDamage << " damage!\n";
            }

            if (enemies[i].health < 1 ){
                enemies.erase(enemies.begin() + i);
            }
        }
        system("clear");
    }

    player.inventory.push_back(generateItem(player));
    player.exp += exp;

    return 0;
}



void outputStats(int health, int damage, int level, string name, double exp) {
    cout << name << "'s stats:\n" << "\nLevel: " << YELLOW << to_string(level) << RESET << "\nExperience: " << CYAN << to_string(exp) << RESET << "\nHealth: " << RED << to_string(health) << RESET << "\nDamage: " << MAGENTA << to_string(damage) << RESET;
}

string explore() {
    int r = rand() % 5;

    switch (r) {
        case 0:
            return "chest";
        case 1:
            return "nothing";
        case 2:
            return "enemy";
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
        cout << counter + 1 << ": " << lookup(inventory[counter]).name << endl;
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

    while (true) {
        string explorationResult;

        if (player.health <= 0){
            system("clear");
            gameOver = true;
            cout << GAME_OVER << "You lost all your health and were sent back to Tartarus" << endl;
            return 0;
        }

        outputStats(player.health, player.damage, player.level, player.name, player.exp);
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

                // Check if the selected item is within the inventory range
                if (choice >= 0 && choice < player.inventory.size()) {
                    // Retrieve the item from the inventory using lookup
                    Item selectedItem = lookup(player.inventory[choice]);
                    
                    // Check if the item is a Food type
                    if (Food* food = dynamic_cast<Food*>(&selectedItem)) {
                        player.health += food->healAmount;
                        player.inventory.erase(player.inventory.begin() + choice);
                    } else {
                        if (selectedItem.damage > 0){
                            player.sword = selectedItem;
                            player.inventory.erase(player.inventory.begin() + choice);
                        } else {
                            if (selectedItem.block > 0){
                                player.shield = selectedItem;
                                player.inventory.erase(player.inventory.begin() + choice);
                            } else {
                                cout << "\nInvalid Item";
                            }
                    }
                } else {
                    cout << "Invalid choice\n";
                }
                break;
            }
            default:
                cout << "Invalid option.\n";
                break;
        }

        if (explorationResult == "chest") {
            generateItem(player);
        } else if (explorationResult == "enemy"){
            fight(player);
        }
    }

    return 0;
}
