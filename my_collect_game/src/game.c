#include "game.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

// Global game state variable
GameState gameState;

// Function to check if a position is occupied by any object
bool isPositionOccupied(float x, float z, float positions[][3], int count) {
    for (int i = 0; i < count; i++) {
        if (fabs(x - positions[i][0]) < 1.0 && fabs(z - positions[i][2]) < 1.0) {
            return true;
        }
    }
    return false;
}

// Function to initialize the game state
void initGame() {
    // Initialize player position and game state variables
    gameState.playerX = 0.0;
    gameState.playerZ = 0.0;
    gameState.score = 0;
    gameState.gameRunning = true;
    gameState.gameOver = false;
    gameState.cameraMode = false;  // Initialize camera mode to false
    gameState.showHelp = false;  // Initialize help screen to false
    gameState.lightIntensity = 1.0; // Initialize light intensity to 1.0
    gameState.rainEnabled = false;  // Initialize rain to be disabled

    // Initialize camera settings
    gameState.camera.x = 0.0;
    gameState.camera.y = 20.0;
    gameState.camera.z = 20.0;
    gameState.camera.yaw = 270.0;
    gameState.camera.pitch = -45.0;

    srand(time(NULL)); // Initialize random number generator

    // Fixed tree positions
    float treePositions[NUM_TREES][3] = {
        {-9.0, 0.0, -9.0},
        {9.0, 0.0, -9.0},
        {-9.0, 0.0, 9.0},
        {9.0, 0.0, 9.0},
        {0.0, 0.0, 7.0},
        {7.0, 0.0, 0.0},
        {-7.0, 0.0, 0.0},
        {0.0, 0.0, -7.0},
        {5.0, 0.0, 5.0},
        {-5.0, 0.0, -5.0}
    };

    // Set tree positions
    for (int i = 0; i < NUM_TREES; i++) {
        gameState.trees[i][0] = treePositions[i][0];
        gameState.trees[i][1] = treePositions[i][1];
        gameState.trees[i][2] = treePositions[i][2];
    }

    // Random rock positions
    for (int i = 0; i < NUM_ROCKS; i++) {
        int validPosition = 0;
        while (!validPosition) {
            validPosition = 1;
            gameState.rocks[i][0] = (rand() % 20) - 10; // Random X coordinate between -10 and 10
            gameState.rocks[i][1] = 0.0;
            gameState.rocks[i][2] = (rand() % 20) - 10; // Random Z coordinate between -10 and 10

            // Ensure the rock does not spawn on another rock or tree
            if (isPositionOccupied(gameState.rocks[i][0], gameState.rocks[i][2], gameState.rocks, i) ||
                isPositionOccupied(gameState.rocks[i][0], gameState.rocks[i][2], gameState.trees, NUM_TREES)) {
                validPosition = 0;
            }
        }
    }

    // Random coin positions
    for (int i = 0; i < NUM_COINS; i++) {
        int validPosition = 0;
        while (!validPosition) {
            validPosition = 1;
            gameState.coins[i][0] = (rand() % 20) - 10; // Random X coordinate between -10 and 10
            gameState.coins[i][1] = 0.0;
            gameState.coins[i][2] = (rand() % 20) - 10; // Random Z coordinate between -10 and 10

            // Ensure the coin does not spawn on a tree or rock
            if (isPositionOccupied(gameState.coins[i][0], gameState.coins[i][2], gameState.trees, NUM_TREES) ||
                isPositionOccupied(gameState.coins[i][0], gameState.coins[i][2], gameState.rocks, NUM_ROCKS)) {
                validPosition = 0;
            }
        }
    }
}

// Function to update the game state
void updateGame() {
    if (!gameState.gameOver) {
        checkCollisions();
        if (gameState.score >= WINNING_SCORE) {
            gameState.gameRunning = false;
            gameState.gameOver = true;
        }
    }
}

// Function to check collisions with coins
void checkCollisions() {
    for (int i = 0; i < NUM_COINS; i++) {
        if (fabs(gameState.playerX - gameState.coins[i][0]) < 1.0 && fabs(gameState.playerZ - gameState.coins[i][2]) < 1.0) {
            gameState.score++; // Increase score
            gameState.coins[i][0] = gameState.coins[i][2] = 100; // Move coin off the map
        }
    }
}
