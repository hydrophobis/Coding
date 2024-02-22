const MAP_WIDTH = 30;
const MAP_HEIGHT = 15;
const EMPTY_TILE = '.';
const PLAYER_TILE = '@';
const ENEMY_TILE = 'E';
const OBSTACLE_TILE = '#';
const PLAYER_MAX_STAMINA = 100;
const PLAYER_ATTACK_STAMINA_COST = 20;
const PLAYER_DODGE_STAMINA_COST = 30;

class Character {
    constructor(health, attack) {
        this.health = health;
        this.attack = attack;
    }
}

class GameState {
    constructor() {
        this.map = Array.from({ length: MAP_HEIGHT }, () => Array(MAP_WIDTH).fill(EMPTY_TILE));
        this.player = new Character(100, 10);
        this.enemies = [];
    }
}

class Armor {
    constructor(name, defense) {
        this.name = name;
        this.defense = defense;
    }
}

class BossAttack {
    constructor(name, damage) {
        this.name = name;
        this.damage = damage;
    }
}

class Boss {
    constructor(level) {
        this.level = level;
        this.name = `Boss Level ${level}`;
        this.health = 50 + level * 10;
        this.attacks = [];

        switch (level) {
            case 5:
                this.attacks.push(new BossAttack("Fireball", 15));
                break;
            case 10:
                this.attacks.push(new BossAttack("Ice Beam", 20));
                break;
            case 15:
                this.attacks.push(new BossAttack("Thunderstorm", 25));
                break;
            case 20:
                this.attacks.push(new BossAttack("Earthquake", 30));
                break;
            case 30:
                this.attacks.push(new BossAttack("Final Blast", 50));
                break;
        }
    }
}

function generateRandomArmor() {
    const name = generateArmorName();
    const defense = Math.floor(Math.random() * 10) + 1;
    return new Armor(name, defense);
}

function generateBoss(level) {
    return new Boss(level);
}

function bossAttack(state, boss) {
    const attack = boss.attacks[Math.floor(Math.random() * boss.attacks.length)];
    state.player.health -= attack.damage;
    console.log(`Boss ${boss.name} used ${attack.name} and dealt ${attack.damage} damage!`);
}

function bossEncounter(state, level) {
    const boss = generateBoss(level);
    console.log(`You encounter ${boss.name}!`);

    while (true) {
        displayGame(state);
        handleInput(state);
        bossAttack(state, boss);

        if (state.player.health <= 0) {
            console.log(`Game Over! You were defeated by ${boss.name}.`);
            break;
        } else if (boss.health <= 0) {
            console.log(`You defeated ${boss.name}!`);
            break;
        }
    }
}

function pickUpArmor(state, armor) {
    const existingArmor = state.equippedArmor.find(equipped => equipped.name.includes(armor.name));

    if (existingArmor) {
        console.log(`You already have a ${armor.name}.`);
    } else {
        state.equippedArmor.push(armor);
        console.log(`You found a ${armor.name}. Equipped!`);
    }
}

function displayGame(state) {
    console.clear();

    for (let y = 0; y < MAP_HEIGHT; ++y) {
        console.log(state.map[y].join(' '));
    }

    console.log(`Player Health: ${state.player.health}\n\n`);
}

function generateArmorName() {
    const adjectives = ["Golden", "Silver", "Enchanted", "Mystic", "Radiant"];
    const nouns = ["Helmet", "Chestplate", "Leggings", "Boots"];

    const adjective = adjectives[Math.floor(Math.random() * adjectives.length)];
    const noun = nouns[Math.floor(Math.random() * nouns.length)];

    return `${adjective} ${noun}`;
}

const readlineSync = require('readline-sync'); // Node.js readline module for synchronous input

function handleInput(state) {
    const playerPosition = findPlayer(state.map);

    let newPlayerX = playerPosition.x;
    let newPlayerY = playerPosition.y;

    const input = readlineSync.keyIn('', { hideEchoBack: true, mask: '' });

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
        case ' ':
            // Perform player attack with stamina cost
            if (state.player.stamina >= PLAYER_ATTACK_STAMINA_COST) {
                attackEnemies(state);
                state.player.stamina -= PLAYER_ATTACK_STAMINA_COST;
            } else {
                console.log("Not enough stamina to attack.");
            }
            break;
        case 'x':
            // Perform player dodge with stamina cost
            if (state.player.stamina >= PLAYER_DODGE_STAMINA_COST) {
                dodge(state, newPlayerX - playerPosition.x, newPlayerY - playerPosition.y);
                state.player.stamina -= PLAYER_DODGE_STAMINA_COST;
            } else {
                console.log("Not enough stamina to dodge.");
            }
            break;
    }

    // Handle picking up armor from chests
    for (let i = 0; i < state.chests.length; ++i) {
        const chestPosition = findCharacter(state.map, CHEST_TILE, i);

        if (isAdjacent(playerPosition, chestPosition)) {
            // Pick up the armor from the chest
            pickUpArmor(state, state.chests[i]);
            state.chests.splice(i, 1);
            --i;  // Adjust index after splicing
        }
    }

    // Check if the new position is within bounds and not an obstacle
    if (newPlayerX >= 0 && newPlayerX < MAP_WIDTH && newPlayerY >= 0 && newPlayerY < MAP_HEIGHT &&
        state.map[newPlayerY][newPlayerX] !== OBSTACLE_TILE) {
        state.map[playerPosition.y][playerPosition.x] = EMPTY_TILE;
        state.map[newPlayerY][newPlayerX] = PLAYER_TILE;
        state.player.stamina = Math.min(state.player.stamina + 1, PLAYER_MAX_STAMINA);
    }
}


function attackEnemies(state) {
    // Check if the player is currently in a cooldown period
    const currentTime = new Date().getTime();
    const elapsedTime = currentTime - state.lastAttackTime;

    if (elapsedTime < PLAYER_ATTACK_COOLDOWN) {
        console.log("Attack is on cooldown. Wait for a moment.");
        return;
    }

    const playerPosition = findPlayer(state.map);

    // Iterate through enemies and check for collisions
    for (let i = 0; i < state.enemies.length; ++i) {
        const enemyPosition = findCharacter(state.map, ENEMY_TILE, i);

        if (isAdjacent(playerPosition, enemyPosition)) {
            // Perform a basic attack
            state.enemies[i].health -= state.player.attack;

            // Check if the enemy is defeated
            if (state.enemies[i].health <= 0) {
                state.map[enemyPosition.y][enemyPosition.x] = EMPTY_TILE;
                state.enemies.splice(i, 1);
                --i;  // Adjust index after splicing
            }

            // Set the last attack time for cooldown
            state.lastAttackTime = new Date().getTime();
        }
    }
}


function update(state) {
    // Update player attack cooldown
    if (state.player.attackCooldown > 0) {
        state.player.attackCooldown -= 1;
    }

    // Update enemy attacks
    for (let i = 0; i < state.enemies.length; ++i) {
        // Check if enemy can attack
        if (state.enemies[i].attackCooldown <= 0) {
            const playerPosition = findPlayer(state.map);
            const enemyPosition = findCharacter(state.map, ENEMY_TILE, i);

            if (isAdjacent(playerPosition, enemyPosition)) {
                // Perform a basic attack
                state.player.health -= state.enemies[i].attack;

                // Check if the player is defeated
                if (state.player.health <= 0) {
                    console.log("Game Over! You were defeated by an enemy.");
                    process.exit(0); // Note: process.exit(0) in Node.js is similar to std::exit(0) in C++
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


function findCharacter(map, character, index) {
    let count = 0;

    for (let i = 0; i < MAP_HEIGHT; ++i) {
        for (let j = 0; j < MAP_WIDTH; ++j) {
            if (map[i][j] === character) {
                if (count === index) {
                    return { x: j, y: i };
                }
                ++count;
            }
        }
    }

    return { x: -1, y: -1 };  // Character not found
}

function findPlayer(map) {
    return findCharacter(map, PLAYER_TILE, 0);
}

function dodge(state, deltaX, deltaY) {
    const playerPosition = findPlayer(state.map);
    const newPlayerPosition = { x: playerPosition.x + deltaX, y: playerPosition.y + deltaY };

    // Check if the new position is within bounds and not an obstacle
    if (
        newPlayerPosition.x >= 0 && newPlayerPosition.x < MAP_WIDTH &&
        newPlayerPosition.y >= 0 && newPlayerPosition.y < MAP_HEIGHT &&
        state.map[newPlayerPosition.y][newPlayerPosition.x] !== OBSTACLE_TILE
    ) {
        state.map[playerPosition.y][playerPosition.x] = EMPTY_TILE;
        state.map[newPlayerPosition.y][newPlayerPosition.x] = PLAYER_TILE;

        console.log("You dodged!");

        // Delay to visualize the dodge
        setTimeout(() => {}, 500);
    }
}

function isAdjacent(position1, position2) {
    return (Math.abs(position1.x - position2.x) === 1 && position1.y === position2.y) ||
        (Math.abs(position1.y - position2.y) === 1 && position1.x === position2.x);
}

function initializeGame() {
    // Implement game initialization logic
    const state = new GameState();
    return state;
}

// Main game loop
function mainGameLoop() {
    const state = initializeGame();

    for (let level = 1; level <= 30; ++level) {
        // Regular level
        if (level % 5 !== 0) {
            console.log(`Entering Level ${level}`);
            generateLevel(state);
            while (true) {
                displayGame(state);
                handleInput(state);
                update(state);

                if (state.player.health <= 0 || state.enemies.length === 0) {
                    console.log(`Level ${level} cleared!`);
                    break;
                }
            }
        }
        // Boss level
        else {
            bossEncounter(state, level);
            if (state.player.health <= 0) {
                console.log("Game Over! You were defeated by the final boss.");
                break;
            }
        }
    }
}

// Call the main game loop to start the game
mainGameLoop();
