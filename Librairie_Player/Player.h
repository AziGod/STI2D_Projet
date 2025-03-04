#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include <DFRobot_RGBMatrix.h>  // Inclure la librairie de la matrice

class Player {
private:
    int x, y;  // Position du joueur
    int speed; // Vitesse de déplacement
    DFRobot_RGBMatrix* matrix; // Pointeur vers la matrice LED

public:
    // Constructeur qui prend un pointeur vers la matrice
    Player(DFRobot_RGBMatrix* display, int startX, int startY);

    // Getters
    int getX();
    int getY();

    // Déplacements
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    // Autres actions
    void placeBomb();
};

#endif
