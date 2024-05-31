#ifndef GAME_H
#define GAME_H
#define MAP_SIZE 10.0f  // Half size of the map

#include <stdbool.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846  // Define PI if not already defined
#endif

// Function declarations for initializing, updating the game, and checking collisions
void initGame();
void updateGame();
void checkCollisions();

#define NUM_COINS 10  // Number of coins
#define NUM_TREES 10  // Number of trees
#define NUM_ROCKS 10  // Number of rocks
#define WINNING_SCORE 10 // Score needed to win the game

// Structure to hold the state of the game
typedef struct {
    float playerX, playerZ;  // Player's position
    int score;  // Player's score
    bool gameRunning;  // Flag indicating if the game is running
    bool gameOver;  // Flag indicating if the game is over
    bool showHelp;  // Flag indicating if the help screen is shown
    bool cameraMode;  // Flag indicating if the camera mode is enabled
    bool rainEnabled;  // Flag indicating if the rain effect is enabled
    struct {
        float x, y, z;  // Camera position
        float yaw, pitch;  // Camera orientation
    } camera;
    float lightIntensity;  // Light intensity for the scene
    float coins[NUM_COINS][3];  // Positions of the coins
    float trees[NUM_TREES][3];  // Positions of the trees
    float rocks[NUM_ROCKS][3];  // Positions of the rocks
} GameState;

// Extern declaration of the game state variable
extern GameState gameState;

#endif
