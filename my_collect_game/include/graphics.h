#ifndef GRAPHICS_H
#define GRAPHICS_H

// Function to initialize the graphics settings
void initGraphics();

// Function to handle the rendering of the display
void display();

// Function to draw a coin at the specified position
void drawCoin(float x, float y, float z);

// Function to draw a tree at the specified position
void drawTree(float x, float y, float z);

// Function to draw a rock at the specified position
void drawRock(float x, float y, float z);

// Function to draw text at the specified screen coordinates
void drawText(float x, float y, char *text);

// Function to draw a trophy at the specified position
void drawTrophy(float x, float y, float z);

// Function to draw the help screen
void drawHelpScreen();

// Function to draw rain
void drawRain();

#endif
