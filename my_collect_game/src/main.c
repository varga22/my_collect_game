#include <GL/glut.h>
#include "game.h"
#include "graphics.h"
#include "input.h"

// Function to update the game state and redraw the screen
void update(int value) {
    if (gameState.gameRunning) {
        updateGame(); // Update game logic
    }
    glutPostRedisplay(); // Redraw the screen
    glutTimerFunc(16, update, 0); // Call this function again after 16 ms
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Set display mode
    glutGameModeString("1920x1080:32@60"); // Set screen resolution and refresh rate
    glutEnterGameMode(); // Switch to full screen mode

    // Initialize graphics and game
    initGraphics();
    initGame();

    // Register callback functions
    glutDisplayFunc(display); // Display callback
    glutKeyboardFunc(handleKeyboard); // Keyboard input callback
    glutSpecialFunc(handleSpecialKeys); // Special keys input callback
    glutPassiveMotionFunc(handleMouseMotion); // Mouse motion callback
    glutMouseFunc(handleMouse); // Mouse button callback
    glutIdleFunc(display); // Idle callback
    glutTimerFunc(16, update, 0); // Timer callback

    // Enter the GLUT main loop
    glutMainLoop();
    return 0;
}
