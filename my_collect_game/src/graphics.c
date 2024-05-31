#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include "game.h"

#define NUM_RAIN_DROPS 100

// Struct to represent a raindrop
typedef struct {
    float x, y, z;
    float speed;
} RainDrop;

// Function to generate random rain drops
void generateRain(RainDrop rainDrops[]) {
    for (int i = 0; i < NUM_RAIN_DROPS; i++) {
        rainDrops[i].x = (rand() % 2000 - 1000) / 100.0;
        rainDrops[i].y = (rand() % 1000) / 100.0;
        rainDrops[i].z = (rand() % 2000 - 1000) / 100.0;
        rainDrops[i].speed = (rand() % 50 + 50) / 1000.0;
    }
}

float coinRotation = 0.0f;
float backgroundColors[20][20]; // 20x20 grid for patchy green colors

// Function to generate random background colors
void generateBackgroundColors() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            backgroundColors[i][j] = (rand() % 50 + 50) / 100.0f; // Random green shade
        }
    }
}

// Function to initialize graphics settings
void initGraphics() {
    glClearColor(0.5, 1.0, 0.5, 0.0); // Light green color (RGB: 128, 255, 128)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);  // Enable color tracking

    // Set light source
    GLfloat lightPos[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Set light properties
    GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat lightDiffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Set material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 200.0);

    generateBackgroundColors(); // Generate background colors
}

// Function to draw rain drops
void drawRain(RainDrop rainDrops[]) {
    if (gameState.rainEnabled) {
        glDisable(GL_LIGHTING);
        glColor3f(0.5, 0.5, 1.0); // Light blue color for rain

        glBegin(GL_LINES);
        for (int i = 0; i < NUM_RAIN_DROPS; i++) {
            rainDrops[i].y -= rainDrops[i].speed;
            if (rainDrops[i].y < 0) {
                rainDrops[i].y = (rand() % 1000) / 100.0;
                rainDrops[i].x = (rand() % 2000 - 1000) / 100.0;
                rainDrops[i].z = (rand() % 2000 - 1000) / 100.0;
            }
            glVertex3f(rainDrops[i].x, rainDrops[i].y, rainDrops[i].z);
            glVertex3f(rainDrops[i].x, rainDrops[i].y - 0.1, rainDrops[i].z);
        }
        glEnd();

        glEnable(GL_LIGHTING);
    }
}

// Function to draw the help screen
void drawHelpScreen() {
    if (gameState.showHelp) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0, 0.0, 0.0, 0.8); // Semi-transparent black
        glBegin(GL_QUADS);
        glVertex2f(100, 100);
        glVertex2f(700, 100);
        glVertex2f(700, 500);
        glVertex2f(100, 500);
        glEnd();

        glColor3f(1.0, 1.0, 1.0); // White color for text
        drawText(120, 480, "HELP MENU");
        drawText(120, 460, "W, A, S, D - Move player");
        drawText(120, 440, "C - Toggle camera mode");
        drawText(120, 420, "Arrow Keys - Move camera (in camera mode)");
        drawText(120, 400, "Mouse - Look around (in camera mode)");
        drawText(120, 380, "Mousewheel - Set height (in camera mode)");
        drawText(120, 360, "R - Restart game");
        drawText(120, 340, "Q - Quit game");
        drawText(120, 320, "F1 - Toggle help menu");
        drawText(120, 300, "M (+) / N (-) - Adjust light intensity");

        glDisable(GL_BLEND);
    }
}

// Function to render the scene
void display() {
    static RainDrop rainDrops[NUM_RAIN_DROPS];
    static bool rainInitialized = false;

    if (!rainInitialized) {
        generateRain(rainDrops);
        rainInitialized = true;
    }

    if (gameState.gameOver) {
        glClearColor(0.0, 0.0, 0.0, 0.0); // Black background when game is over
    } else {
        glClearColor(0.5, 1.0, 0.5, 0.0); // Light green background during the game
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Calculate camera direction
    float radYaw = gameState.camera.yaw * M_PI / 180.0;
    float radPitch = gameState.camera.pitch * M_PI / 180.0;
    float dirX = cos(radPitch) * cos(radYaw);
    float dirY = sin(radPitch);
    float dirZ = cos(radPitch) * sin(radYaw);

    gluLookAt(gameState.camera.x, gameState.camera.y, gameState.camera.z,
              gameState.camera.x + dirX, gameState.camera.y + dirY, gameState.camera.z + dirZ,
              0.0, 1.0, 0.0); // Set camera view

    // Adjust light intensity based on gameState.lightIntensity
    GLfloat lightAmbient[] = { 0.2 * gameState.lightIntensity, 0.2 * gameState.lightIntensity, 0.2 * gameState.lightIntensity, 1.0 };
    GLfloat lightDiffuse[] = { 0.8 * gameState.lightIntensity, 0.8 * gameState.lightIntensity, 0.8 * gameState.lightIntensity, 1.0 };
    GLfloat lightSpecular[] = { 1.0 * gameState.lightIntensity, 1.0 * gameState.lightIntensity, 1.0 * gameState.lightIntensity, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    if (!gameState.gameOver) {
        // Draw patchy green background (ground)
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                glColor3f(0.0f, backgroundColors[i][j], 0.0f);
                glBegin(GL_QUADS);
                glVertex3f(i - 10.0f, 0.0f, j - 10.0f);
                glVertex3f(i - 9.0f, 0.0f, j - 10.0f);
                glVertex3f(i - 9.0f, 0.0f, j - 9.0f);
                glVertex3f(i - 10.0f, 0.0f, j - 9.0f);
                glEnd();
            }
        }

        // Draw player
        glPushMatrix();
        glTranslatef(gameState.playerX, 0.5, gameState.playerZ);
        glColor3f(1.0, 0.0, 0.0); // Red color for player
        glutSolidCube(0.5); // Smaller player size
        glPopMatrix();

        // Draw coins
        for (int i = 0; i < NUM_COINS; i++) {
            drawCoin(gameState.coins[i][0], gameState.coins[i][1], gameState.coins[i][2]);
        }

        // Draw trees
        for (int i = 0; i < NUM_TREES; i++) {
            drawTree(gameState.trees[i][0], gameState.trees[i][1], gameState.trees[i][2]);
        }

        // Draw rocks
        for (int i = 0; i < NUM_ROCKS; i++) {
            drawRock(gameState.rocks[i][0], gameState.rocks[i][1], gameState.rocks[i][2]);
        }

        // Set text color to white
        glColor3f(1.0, 1.0, 1.0);

        // Display score
        char scoreText[20];
        sprintf(scoreText, "Score: %d", gameState.score);
        drawText(10, 580, scoreText);

        // Display remaining points needed
        char remainingText[30];
        sprintf(remainingText, "Remaining: %d", WINNING_SCORE - gameState.score);
        drawText(10, 550, remainingText);

        // Display help text if showHelp is true
        if (gameState.showHelp) {
            drawText(600, 580, "HELP MENU");
            drawText(600, 560, "W, A, S, D - Move player");
            drawText(600, 540, "C - Toggle camera mode");
            drawText(600, 520, "Arrow Keys - Move camera (in camera mode)");
            drawText(600, 500, "Mouse - Look around (in camera mode)");
            drawText(600, 480, "Mousewheel - Set height (in camera mode)");
            drawText(600, 460, "R - Restart game");
            drawText(600, 440, "Q - Quit game");
            drawText(600, 420, "F1 - Toggle help menu");
            drawText(600, 400, "M (+) / N (-) - Adjust light intensity");
            drawText(600, 380, "E - Toggle rain");
        }
    } else {
        // Display end game message and trophy
        glColor3f(1.0, 1.0, 1.0);
        drawText(375, 300, "You Win!");
        drawText(333, 270, "Press R to Restart or Q to Quit");
        drawTrophy(0.0, 2.0, 0.0); // Draw trophy in the center
    }

    drawRain(rainDrops);

    glutSwapBuffers(); // Swap buffers for double buffering

    // Update coin rotation for animation
    coinRotation += 2.0f;
    if (coinRotation > 360.0f) {
        coinRotation -= 360.0f;
    }
}

// Function to draw a coin
void drawCoin(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y + 0.5, z);
    glRotatef(coinRotation, 0.0, 1.0, 0.0); // Rotation around y-axis

    GLfloat materialAmbient[] = { 0.25, 0.25, 0.25, 1.0 };
    GLfloat materialDiffuse[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat materialSpecular[] = { 0.774597, 0.774597, 0.774597, 1.0 };
    GLfloat materialShininess[] = { 76.8 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

    glColor3f(1.0, 0.84, 0.0); // Gold color

    GLUquadric* quad;
    quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    glTranslatef(0.0, 0.0, 0.0);

    // Cylinder
    gluCylinder(quad, 0.3, 0.3, 0.05, 30, 1); // Cylinder parameters (quadric, bottom radius, top radius, height, slices, stacks)

    // Bottom disk
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    gluDisk(quad, 0.0, 0.3, 30, 1); // Disk parameters (quadric, inner radius, outer radius, slices, loops)
    glPopMatrix();

    // Top disk
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.05);
    gluDisk(quad, 0.0, 0.3, 30, 1); // Disk parameters (quadric, inner radius, outer radius, slices, loops)
    glPopMatrix();

    gluDeleteQuadric(quad);
    glPopMatrix();
}

// Function to draw a tree
void drawTree(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glColor3f(0.55, 0.27, 0.07); // Brown color for trunk
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(0.5, 2.0, 0.5);
    glutSolidCube(1.0); // Draw trunk
    glPopMatrix();

    glColor3f(0.0, 1.0, 0.0); // Green color for leaves
    glPushMatrix();
    glTranslatef(0.0, 2.5, 0.0);
    glutSolidSphere(1.0, 20, 20); // Draw leaves
    glPopMatrix();

    glPopMatrix();
}

// Function to draw a rock
void drawRock(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.5, 0.5, 0.5); // Gray color for rock

    // Combine multiple smaller spheres to create an irregular shape
    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);
    glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2, 0.1, 0.2);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2, 0.1, -0.2);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2, -0.1, -0.2);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2, -0.1, 0.2);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

// Function to draw text on the screen
void drawText(float x, float y, char *text) {
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0); // White color for text
    glRasterPos2f(x, y);

    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
}

// Function to draw a trophy
void drawTrophy(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glColor3f(1.0, 0.84, 0.0); // Gold color
    glPushMatrix();
    glScalef(0.5, 0.1, 0.5);
    glutSolidCube(1.0); // Draw base
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.3, 0.0);
    glScalef(0.3, 0.6, 0.3);
    glutSolidCube(1.0); // Draw middle part
    glPopMatrix();

    glColor3f(1.0, 0.84, 0.0); // Gold color for handles
    glPushMatrix();
    glTranslatef(-0.3, 0.9, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.05, 0.05, 0.5, 20, 20); // Draw left handle
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3, 0.9, 0.0);
    glRotatef(-90, 0.0, 1.0, 0.0);
    gluCylinder(quad, 0.05, 0.05, 0.5, 20, 20); // Draw right handle
    gluDeleteQuadric(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 1.1, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    quad = gluNewQuadric();
    gluCylinder(quad, 0.0, 0.4, 0.6, 30, 1); // Draw cup
    gluDeleteQuadric(quad);
    glPopMatrix();

    glPopMatrix();
}
