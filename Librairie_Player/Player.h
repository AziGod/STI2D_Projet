#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include <DFRobot_RGBMatrix.h>  // Inclure la librairie de la matrice

class Player {
private:
    int x, y;  // Position du joueur
    int speed; // Vitesse de déplacement
    DFRobot_RGBMatrix* matrix;
    uint32_t pixelColors[64][64];

public:
    // Constructeur qui prend un pointeur vers la matrice
    Player(DFRobot_RGBMatrix* matrix, int startX, int startY);

    // Getters
    int getX();
    int getY();

    // Déplacements
    void moveUp(short tabColor[64][64] );
    void moveDown(short tabColor[64][64] );
    void moveLeft(short tabColor[64][64] );
    void moveRight(short tabColor[64][64] );

    // Collisions
    bool checkWallCollision(int x, int y,short tabColor[64][64]);

    // Autres actions
    void placeBomb();
};

#endif
