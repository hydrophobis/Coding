#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h>

// Define map dimensions
const int MAP_WIDTH = 30;
const int MAP_HEIGHT = 15;

// Define tile types
const char EMPTY_TILE = '.';
const char PLAYER_TILE = '@';
const char ENEMY_TILE = 'E';
const char OBSTACLE_TILE = '#';

// Define character stats
struct Character {
    int health;
    int attack;
};

// Define game state
struct GameState {
    std::vector<std::vector<char>> map;
    Character player;
    std::vector<Character> enemies;
};

// Function to initialize the game state
GameState initializeGame() {
    // Seed the random number generator
    std::srand(std::time(nullptr));

    GameState state;
    state.map.resize(MAP_HEIGHT, std::vector<char>(MAP_WIDTH, EMPTY_TILE));

    // Place player in the center
    state.player = { 100, 10 };
    state.map[MAP_HEIGHT / 2][MAP_WIDTH / 2] = PLAYER_TILE;

    // Generate random enemies
    for (int i = 0; i < 5; ++i) {
        int enemyX, enemyY;
        do {
            enemyX = std::rand() % MAP_WIDTH;
            enemyY = std::rand() % MAP_HEIGHT;
        } while (state.map[enemyY][enemyX] != EMPTY_TILE);

        state.enemies.push_back({ 20, 5 });
        state.map[enemyY][enemyX] = ENEMY_TILE;
    }

    // Generate random obstacles
    for (int i = 0; i < 10; ++i) {
        int obstacleX, obstacleY;
        do {
            obstacleX = std::rand() % MAP_WIDTH;
            obstacleY = std::rand() % MAP_HEIGHT;
        } while (state.map[obstacleY][obstacleX] != EMPTY_TILE);

        state.map[obstacleY][obstacleX] = OBSTACLE_TILE;
    }

    return state;
}

// Function to display the game state
void displayGame(const GameState& state) {
    system("cls");  // Clear the console

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            std::cout << state.map[y][x] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "Player Health: " << state.player.health << "\n\n";
}

// Function to handle player input and update game state
void handleInput(GameState& state) {
    char input = _getch();  // Get a single character from the console

    int playerX, playerY;
    findPlayer(state.map, playerX, playerY);

    // Handle movement
    int newPlayerX = playerX;
    int newPlayerY = playerY;

    switch (input) {
    case 'w':
        --newPlayerY;
        break;
    case 's':
        ++newPlayerY;
        break;
    case 'a':
        --newPlayerX;
        break;
    case 'd':
        ++newPlayerX;
        break;
    }

    // Check if the new position is within bounds and not an obstacle
    if (newPlayerX >= 0 && newPlayerX < MAP_WIDTH && newPlayerY >= 0 && newPlayerY < MAP_HEIGHT &&
        state.map[newPlayerY][newPlayerX] != OBSTACLE_TILE) {
        // Move the player
        state.map[playerY][playerX] = EMPTY_TILE;
        state.map[newPlayerY][newPlayerX] = PLAYER_TILE;
    }

    // Handle player attack
    if (input == ' ') {
        int enemyX, enemyY;
        findCharacter(state.map, ENEMY_TILE, enemyX, enemyY, 0);

        if (isAdjacent(playerX, playerY, enemyX, enemyY)) {
            // Perform a basic attack
            state.enemies[0].health -= state.player.attack;

            // Check if the enemy is defeated
            if (state.enemies[0].health <= 0) {
                state.map[enemyY][enemyX] = EMPTY_TILE;
                state.enemies.erase(state.enemies.begin());
            }
        }
    }
}

// Function to update the game state
void update(GameState& state) {
    // Update player attack cooldown
    if (state.player.attackCooldown > 0) {
        state.player.attackCooldown -= 1;
    }

    // Update enemy attacks
    for (size_t i = 0; i < state.enemies.size(); ++i) {
        // Check if enemy can attack
        if (state.enemies[i].attackCooldown <= 0) {
            int playerX, playerY;
            findPlayer(state.map, playerX, playerY);

            if (isAdjacent(playerX, playerY, state.enemies[i].x, state.enemies[i].y)) {
                // Perform a basic attack
                state.player.health -= state.enemies[i].attack;

                // Check if the player is defeated
                if (state.player.health <= 0) {
                    std::cout << "Game Over! You were defeated by an enemy.\n";
                    std::exit(0);
                }
            }

            // Reset enemy attack cooldown
            state.enemies[i].attackCooldown = ENEMY_ATTACK_COOLDOWN;
        }
        else {
            // Decrease enemy attack cooldown
            state.enemies[i].attackCooldown -= 1;
        }
    }
}

// Function to find the position of a character on the map
void findCharacter(const std::vector<std::vector<char>>& map, char character, int& x, int& y, size_t index) {
    size_t count = 0;

    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            if (map[i][j] == character) {
                if (count == index) {
                    x = j;
                    y = i;
                    return;
                }
                ++count;
            }
        }
    }

    x = -1;  // Character not found
    y = -1;
}

// Function to find the position of the player on the map
void findPlayer(const std::vector<std::vector<char>>& map, int& x, int& y) {
    findCharacter(map, PLAYER_TILE, x, y, 0);
}

// Function to check if two positions are adjacent
bool isAdjacent(int x1, int y1, int x2, int y2) {
    return (std::abs(x1 - x2) == 1 && y1 == y2) || (std::abs(y1 - y2) == 1 && x1 == x2);
}

int main() {
    GameState gameState = initializeGame();

    while (true) {
        displayGame(gameState);
        handleInput(gameState);

        // Check for game over condition
        if (gameState.player.health <= 0 || gameState.enemies.empty()) {
            std::cout << "Game Over! ";
            if (gameState.player.health <= 0) {
                std::cout << "You were defeated.\n";
            } else {
                std::cout << "You defeated all enemies.\n";
            }
            break;
        }
    }

    return 0;
}
