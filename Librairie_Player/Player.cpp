#include "Player.h"

// Constructeur
Player::Player(DFRobot_RGBMatrix* matrix, int startX, int startY, short pixelColors[64][64]) {
    this->matrix = matrix;  // Stocke le pointeur
    this->x = startX;
    this->y = startY;
    this->pixelColors[64][64] = pixelColors[64][64];
    this->speed = 1;  // Par défaut, vitesse = 1

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
    if (! checkWallCollision(x,y)){
        matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 7)); // Redessine à la nouvelle position
    }
}

void Player::moveDown() { 
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 0));
    y = min(60, y + speed);
    if (! checkWallCollision(x,y)){
        matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 7));
    }
}

void Player::moveLeft() { 
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 0));
    x = max(0, x - speed);
    if (! checkWallCollision(x,y)){
        matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 7));
    }
}

void Player::moveRight() { 
    matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 0));
    x = min(60, x + speed);
    if (! checkWallCollision(x,y)){
        matrix->fillRect(x, y, 4, 4, matrix->Color333(0, 0, 7));
    }
}

//Collision
bool Player::checkWallCollision(int x, int y){
    for (int i = 0; i < 4; i++) { // Première boucle (lignes)
        for (int j = 0; j < 4; j++) { // Deuxième boucle (colonnes)
            if (pixelColors[x][y] == true){
                return true;
            } 
        }
    }
    return false;
}

// Placer une bombe (simulation)
void Player::placeBomb() {
    Serial.println("💣 Bombe placée !");
}
