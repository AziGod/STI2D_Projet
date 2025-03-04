#include "Player.h"

// Constructeur
Player::Player(DFRobot_RGBMatrix* display, int startX, int startY) {
    x = startX;
    y = startY;
    speed = 1;
    matrix = display; // Stocke le pointeur vers la matrice

    // Affiche le joueur sur la matrice
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 7));
}

// Getters
int Player::getX() { return x; }
int Player::getY() { return y; }

// Déplacements
void Player::moveUp() { 
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 0)); // Efface l'ancienne position
    y = max(0, y - speed);
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 7)); // Redessine à la nouvelle position
}

void Player::moveDown() { 
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 0));
    y = min(63, y + speed);
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 7));
}

void Player::moveLeft() { 
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 0));
    x = max(0, x - speed);
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 7));
}

void Player::moveRight() { 
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 0));
    x = min(63, x + speed);
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 7));
}

// Placer une bombe (simulation)
void Player::placeBomb() {
    Serial.println("💣 Bombe placée !");
}
