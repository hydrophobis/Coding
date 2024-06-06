#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

#include "def.hpp"
#include "inc.hpp"

using namespace std;

Game newGame(string name){
    Game game;

    game.playerHealth = 10;
    game.playerDamage = 3;
    game.playerLevel = 1;
    game.playerName = name;

    return game;
}

Game loadGame(){
    Game game
    // vector<vector<char>>
    char** data = readFile("hadean_terminal.save", 4);
    
    game.playerHealth = base64_decode(data[0]);
    game.playerDamage = base64_decode(data[1]);
    game.playerLevel = base64_decode(data[2]);
    game.playerName = base64_decode(data[3]);

    return game;
}

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
    int numEnemies = rand() % 5; // Determine the number of enemies
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

string outputStats(int health, int damage, int level, string name){
    cout << RESET << name << "'s stats:\n" << "Health: " << health << "\nLevel: " << level << "\nDamage: " << damage;
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


vector<int> inventoryB(vector<int> inv){
    int i = -1;
    vector<int> list;

    system("clear");

    for(i++; i < inv.size();){
        list.push_back(inv[i]);
    }

    return list;
}

void outputInv(const vector<string>& inventory) {
    for (size_t counter = 0; counter < inventory.size(); ++counter) {
        cout << counter + 1 << ": " << lookup(inventory[counter]).name << endl;
    }
}

int main(){
    Game game;

    system("clear");
    int option;
    cout << INTRO_1 << INTRO_2 << "\n" << MENU_MAIN;
    cin >> option;
    switch(option){
        case 1:
            game = loadGame();
        case 2:
            string name;
            cout << endl << "Player Name: ";
            cin >> name;
            game = newGame(name);
        case 3:
            return 0;
    }

    srand(time(0));




    while(true){
        outputStats();
        ENDL; ENDL;
        cout << MENU_ACTION;
        cin >> option;

        switch(option){
            case 1:
                explore();
            case 2:
                outputInv();
            case 3:
                cout
        }
    }
}