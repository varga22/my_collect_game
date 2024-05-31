#ifndef INPUT_H
#define INPUT_H

// Function to handle standard keyboard input
void handleKeyboard(unsigned char key, int x, int y);

// Function to handle special keys (e.g., arrow keys, function keys)
void handleSpecialKeys(int key, int x, int y);

// Function to handle mouse motion events
void handleMouseMotion(int x, int y);

// Function to handle mouse button events
void handleMouse(int button, int state, int x, int y);

#endif
