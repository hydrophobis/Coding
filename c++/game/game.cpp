#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <algorithm>

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

Game loadGame() {
    Game game;
    int lineCount;
    char** data = readFile("hadean_terminal.save", lineCount);

    game.playerHealth = stoi(base64_decode(data[0]));
    game.playerDamage = stoi(base64_decode(data[1]));
    game.playerLevel = stoi(base64_decode(data[2]));
    game.playerName = base64_decode(data[3]);
    // Assuming playerInv is a vector of strings
    string inv = base64_decode(data[4]);
    istringstream iss(inv);
    vector<int> playerInv((istream_iterator<int>(iss)), istream_iterator<int>());
    game.playerInv = playerInv;

    for (int i = 0; i < lineCount; ++i) {
        delete[] data[i];
    }
    delete[] data;

    return game;
}

Entity genEnemy(int lvl) {
    string enemyName;
    int enemyLvl = rand() % lvl + (lvl * 1.5); 
    int health = enemyLvl * (rand() % 10);
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
    Entity entity = {enemyName, health, damage, enemyLvl};
    return entity;
}

int fight(Player player) {
    int hp = player.health;
    int damage = player.damage + player.sword.damage;
    vector<int> inv = player.inventory;
    int input;
    vector<Entity> enemies;
    bool working = true;
    bool blocking = false;

    int numEnemies = rand() % 5 + 1; // Ensure at least one enemy
    for (int i = 0; i < numEnemies; ++i) {
        Entity enemy = genEnemy(player.level);
        enemies.push_back(enemy);
    }

    system("clear");
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
                } else {cout << "\nYou missed!\n"}
                break;
            }
            default: {
                cout << "Invalid choice" << endl;
                break;
            }
        }

        if (enemies[i].health < 1 ){
            enemies.erase(enemies.begin() + i)
        }
    }

    return 0;
}

string outputStats(int health, int damage, int level, string name) {
    return name + "'s stats:\n" + "Health: " + to_string(health) + "\nLevel: " + to_string(level) + "\nDamage: " + to_string(damage);
}

int generateItem(Player player) {
    int id = rand() % TOTAL_ITEMS;
    return id;
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

void outputInv(const vector<Item*>& inventory) {
    for (size_t counter = 0; counter < inventory.size(); ++counter) {
        cout << counter + 1 << ": " << inventory[counter]->name << endl;
    }
}

int main() {
    Game game;
    Player player;

    system("clear");
    int option;
    cout << INTRO_1 << INTRO_2 << "\n" << MENU_MAIN;
    cin >> option;
    switch (option) {
        case 1:
            game = loadGame();
            break;
        case 2: {
            string name;
            cout << endl << "Player Name: ";
            cin >> name;
            game = newGame(name);
            break;
        }
        case 3:
            return 0;
        default:
            cout << "Invalid option.\n";
            return 1;
    }

    srand(time(0));

    player.health = game.playerHealth;
    player.damage = game.playerDamage;
    player.level = game.playerLevel;
    player.name = game.playerName;
    player.inventory = game.playerInv;

    while (true) {
        cout << outputStats(player.health, player.damage, player.level, player.name);
        cout << "\n\n" << MENU_ACTION;
        cin >> option;

        system("clear");

        switch (option) {
            case 1: {
                string explorationResult = explore();
                // Handle exploration result
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

                // Check if the selected item is consumable
                if (choice >= 0 && choice < player.inventory.size()) {
                    Item* selectedItem = lookup(player.inventory[choice]);
                    if (Food* food = dynamic_cast<Food*>(selectedItem)) {
                        player.health += food->nutrition;
                    } else {
                        cout << "That item isn't usable\n";
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
            fight(player&);
        }
    }
}