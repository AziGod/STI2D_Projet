#include "Player.h"

// Constructeur
Player::Player(int startX, int startY, int playerSize, int moveSpeed, DFRobot_RGBMatrix& displayMatrix) 
    : posX(startX), posY(startY), tailleJoueur(playerSize), speed(moveSpeed), matrix(displayMatrix) {}

// Vérifie les collisions avec les murs
bool Player::checkWallCollision(int x, int y) {
    if (x < 6 || x > 58 - tailleJoueur || y < 6 || y > 58 - tailleJoueur) {
        return true;
    }
    return false;
}

// Efface l'ancien joueur
void Player::clear() {
    matrix.fillRect(posX, posY, tailleJoueur, tailleJoueur, matrix.Color333(0, 0, 0));
}

// Dessine le joueur à sa nouvelle position
void Player::draw() {
    matrix.fillRect(posX, posY, tailleJoueur, tailleJoueur, matrix.Color333(0, 0, 7));
}

// Déplacements
void Player::moveUp() { 
    if (!checkWallCollision(posX, posY + speed)) {
        clear();
        posY += speed;
        draw();
    }
}

void Player::moveDown() { 
    if (!checkWallCollision(posX, posY - speed)) {
        clear();
        posY -= speed;
        draw();
    }
}

void Player::moveLeft() { 
    if (!checkWallCollision(posX - speed, posY)) {
        clear();
        posX -= speed;
        draw();
    }
}

void Player::moveRight() { 
    if (!checkWallCollision(posX + speed, posY)) {
        clear();
        posX += speed;
        draw();
    }
}

// Gestion du mouvement en fonction du joystick
void Player::movePlayer(int joystickX, int joystickY) {
    if (joystickX < 450) {
        moveRight();
    } else if (joystickX > 550) {
        moveLeft();
    }
    
    if (joystickY > 550) {
        moveUp();
    } else if (joystickY < 450) {
        moveDown();
    }
}

// Getters
int Player::getX() const {
    return posX;
}

int Player::getY() const {
    return posY;
}
