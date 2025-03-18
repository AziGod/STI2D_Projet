#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include <DFRobot_RGBMatrix.h>  // Inclure la librairie de la matrice

class Player {
private:
    int x, y;  // Position du joueur
    int speed; // Vitesse de déplacement
    DFRobot_RGBMatrix* matrix;
    short pixelColors[64][64];

public:
    // Constructeur qui prend un pointeur vers la matrice
    Player(DFRobot_RGBMatrix* matrix, int startX, int startY, short pixelColors[64][64]);

    // Getters
    int getX();
    int getY();

    // Déplacements
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    // Collisions
    bool checkWallCollision(int x, int y);

    // Autres actions
    void placeBomb();
};

#endif
