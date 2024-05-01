#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include "items.hpp"

using namespace std;

// Uses player level to return an enemy
Entity genEnemy(int lvl){
    cout << "h\n";
    string enemyName;

    // Algorithim for health stat
    int enemyLvl = rand() % lvl + (lvl * 1.5); 
    int health = enemyLvl * (rand() % 10);
    double multi;

    // Algorithim for damage
    int damage = (rand() % 5) * (health / enemyLvl);

    // Defines names and health multipliers
    int name = rand() % 4;
    switch (name) {
        case 1:
            enemyName = "Goblin";
            multi = 0.5;
            break;
        case 2:
            enemyName = "Orc";
            multi = 1.5;
            break;
        case 3:
            enemyName = "Troll";
            multi = 1.25;
            break;
        case 4:
            enemyName = "Dragon";
            multi = 2.0;
            break;
        case 5:
            enemyName = "Vampire";
            break;
        default:
            enemyName = "Unknown";
            break;
    };

    health = health * multi;
    Entity entity = {enemyName, health, damage, enemyLvl};
    return entity;
}

int fight(Player player) {
    int hp = player.health;
    int damage = player.damage;
    vector<int> inv = player.inventory;
    int input;
    int defense = 0;
    vector<Entity> enemies;
    bool working = true;

    // Create enemies
    int numEnemies = rand() % 3; // Determine the number of enemies
    for (int i = 0; i < numEnemies; ++i) {
        Entity enemy = genEnemy(player.level);
        enemies.push_back(enemy);
    }

    // Actual fight loop
    system("clear");
    for (int i = 0; i < enemies.size(); ++i) {
        cout << "A " << enemies[i].name << " has appeared!\n1. Block\n2. Attack";
        cin >> input;
        switch (input) {
            case 1: {
                // Find if player has a defensive item (id 1, 3, 5, or 7)
                 return any_of(inv.begin(), inv.end(), [](int i) {
                    return i == 1 || i == 3 || i == 5 || i == 7;
                });
                break;
            }
            case 2:
                // Handle attack
                break;
            default:
                cout << "Invalid input.\n";
        }
    }

    return 0;
}


// Generate an item
int generateItem(Player player){
    int id = (rand() % 8);
    return id;
}

string explore(){
    int r = rand() % 5;

    switch (r){
        case 1:
            return "chest";

        case 2:
            return "nothing";

        case 3:
            return "enemy";

        case 4:
            return "trap";

        case 5: 
            return "nothing";

        default:
            return "err";
    }
};

// No idea what this does
vector<int> inventoryB(vector<int> inv){
    int i = -1;
    vector<int> list;

    system("clear");

    for(i++; i < inv.size();){
        list.push_back(inv[i]);
    }

    return list;
}

int main(){

    system("clear");

    srand(time(0));

    Player player;
    vector<int> inventory = player.inventory;
    int action;
    int ch;
    string exploreS;
    int input;

    inventory.push_back(generateItem(player));

    cout << "You've entered the dungeon\n";
    while(true){
        cout << "Select an action:\n" << "1. Explore\n" << "2. Use an item\nAction: ";
        cin >> input;
        

        switch (input) {
            case 1:{

                string e = explore();
                if(e == "chest"){
                    int itemId = generateItem(player); 
                    Item item = lookup(itemId); // Retrieve Item from ID
                    std::cout << "You found a " << item.name << " in a chest!" << std::endl;
                    break;     
                } else if (e == "nothing"){
                    cout << "Nothing here...\n";
                }else if (e == "enemy"){
                    fight(player);
                }
                break;
                

            }
            case 2:{

                system("clear");
                int i = 0;
                while(i < inventory.size()){
                    i++;
                    cout << i << ". " << lookup(inventory[i]).name << "\n";
                }
                cout << "Use item number: ";
                cin >> input;
                useItem(lookup(inventory[input]).id, player);
                system("clear");
                
                break;
            }

            default:{
                cout << "Invalid input\n";
                break;
            }
        }
    }
    return 0;
}