#include <GL/glut.h>
#include <math.h>
#include "game.h"
#include "graphics.h"
#include "input.h"
#include <stdbool.h>

// Function to check if the player can move to the specified coordinates
bool canMoveTo(float newX, float newZ) {
    // Check if the new position is within the map boundaries
    if (newX < -MAP_SIZE || newX > MAP_SIZE || newZ < -MAP_SIZE || newZ > MAP_SIZE) {
        return false;
    }

    // Check collisions with trees
    for (int i = 0; i < NUM_TREES; i++) {
        if (fabs(newX - gameState.trees[i][0]) < 0.5 && fabs(newZ - gameState.trees[i][2]) < 0.5) {
            return false;
        }
    }

    // Check collisions with rocks
    for (int i = 0; i < NUM_ROCKS; i++) {
        if (fabs(newX - gameState.rocks[i][0]) < 0.5 && fabs(newZ - gameState.rocks[i][2]) < 0.5) {
            return false;
        }
    }
    return true;
}

// Function to handle standard keyboard input
void handleKeyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.2;

    if (gameState.gameRunning && !gameState.showHelp) {
        switch (key) {
            case 'w':
                if (!gameState.cameraMode && canMoveTo(gameState.playerX, gameState.playerZ - moveSpeed)) 
                    gameState.playerZ -= moveSpeed;
                break;
            case 's':
                if (!gameState.cameraMode && canMoveTo(gameState.playerX, gameState.playerZ + moveSpeed)) 
                    gameState.playerZ += moveSpeed;
                break;
            case 'a':
                if (!gameState.cameraMode && canMoveTo(gameState.playerX - moveSpeed, gameState.playerZ)) 
                    gameState.playerX -= moveSpeed;
                break;
            case 'd':
                if (!gameState.cameraMode && canMoveTo(gameState.playerX + moveSpeed, gameState.playerZ)) 
                    gameState.playerX += moveSpeed;
                break;
            case 'c':
                gameState.cameraMode = !gameState.cameraMode; // Toggle camera mode
                break;
            case 'm':
                gameState.lightIntensity += 0.1f; // Increase light intensity
                if (gameState.lightIntensity > 1.0f) gameState.lightIntensity = 1.0f;
                break;
            case 'n':
                gameState.lightIntensity -= 0.1f; // Decrease light intensity
                if (gameState.lightIntensity < 0.0f) gameState.lightIntensity = 0.0f;
                break;
            case 'e':
                gameState.rainEnabled = !gameState.rainEnabled; // Toggle rain effect
                break;
            case 27: // Escape key
                exit(0);
        }
    } else {
        if (key == 'r' || key == 'R') {
            initGame(); // Restart the game
        } else if (key == 'q' || key == 'Q') {
            exit(0); // Quit the game
        }
    }
}

// Function to handle special keyboard input (e.g., arrow keys, function keys)
void handleSpecialKeys(int key, int x, int y) {
    if (gameState.cameraMode && !gameState.showHelp) {
        float moveSpeed = 0.5;
        float radYaw = gameState.camera.yaw * M_PI / 180.0;
        float dirX = cos(radYaw);
        float dirZ = sin(radYaw);

        switch (key) {
            case GLUT_KEY_UP:
                gameState.camera.x += dirX * moveSpeed;
                gameState.camera.z += dirZ * moveSpeed;
                break;
            case GLUT_KEY_DOWN:
                gameState.camera.x -= dirX * moveSpeed;
                gameState.camera.z -= dirZ * moveSpeed;
                break;
            case GLUT_KEY_LEFT:
                gameState.camera.x -= dirZ * moveSpeed;
                gameState.camera.z += dirX * moveSpeed;
                break;
            case GLUT_KEY_RIGHT:
                gameState.camera.x += dirZ * moveSpeed;
                gameState.camera.z -= dirX * moveSpeed;
                break;
            case GLUT_KEY_F1:
                gameState.showHelp = !gameState.showHelp; // Toggle help screen
                break;
        }
    } else if (key == GLUT_KEY_F1) {
        gameState.showHelp = !gameState.showHelp; // Toggle help screen
    }
}

// Function to handle mouse motion events
void handleMouseMotion(int x, int y) {
    if (gameState.cameraMode && !gameState.showHelp) {
        static bool warpPointer = false;

        if (!warpPointer) {
            int dx = x - 400;
            int dy = y - 300;

            gameState.camera.yaw += dx * 0.1;
            gameState.camera.pitch -= dy * 0.1;

            if (gameState.camera.pitch > 89.0) gameState.camera.pitch = 89.0;
            if (gameState.camera.pitch < -89.0) gameState.camera.pitch = -89.0;

            glutWarpPointer(400, 300); // Reset mouse position to the center of the screen
            warpPointer = true;
        } else {
            warpPointer = false;
        }
    }
}

// Function to handle mouse button events
void handleMouse(int button, int state, int x, int y) {
    if (gameState.cameraMode && !gameState.showHelp) {
        if (button == 3 && state == GLUT_DOWN) { // Scroll up
            gameState.camera.y += 1.0f;
        } else if (button == 4 && state == GLUT_DOWN) { // Scroll down
            gameState.camera.y -= 1.0f;
            if (gameState.camera.y < 1.0) {
                gameState.camera.y = 1.0;
            }
        }
    }
}
