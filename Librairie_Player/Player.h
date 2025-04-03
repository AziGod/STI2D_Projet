#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "DFRobot_RGBMatrix.h"

class Player {
public:
    Player(int startX, int startY, int playerSize, int moveSpeed, DFRobot_RGBMatrix& matrix);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void movePlayer(int joystickX, int joystickY);
    void draw();
    void clear();
    
    int getX() const;
    int getY() const;

private:
    int posX, posY;
    int tailleJoueur;
    int speed;
    DFRobot_RGBMatrix& matrix;

    bool checkWallCollision(int x, int y);
};

#endif
