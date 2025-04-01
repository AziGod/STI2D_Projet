/*!
 * @file testRGBMatrix.ino
 * @brief Run the routine to test the RGB LED Matrix Panel
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie]](jie.tang@dfrobot.com)
 * @version  V1.0.1
 * @date  2022-03-23
 * @url https://github.com/DFRobot/DFRobot_RGBMatrix
 */
 
#include <DFRobot_RGBMatrix.h> // Hardware-specific library

#define OE   	9
#define LAT 	10
#define CLK 	11
#define A   	A0
#define B   	A1
#define C   	A2
#define D   	A3
#define E   	A4
#define WIDTH 64
#define _HIGH	64

// JOYSTICK
int axeX = A6; // signal de l'axe X sur entrée A0
int axeY = A7; // signal de l'axe Y sur entrée A1

float X, Y;
int posX = 6, posY = 6;
int speed = 1;

int tailleJoueur = 3;

DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, _HIGH);



//Structure coordonnées pour les bombes
struct Bombe {
  int x;
  int y;
  unsigned long timePosed;
  bool active;
};

//Tableau des coordonnées des bombes posées
Bombe bombes[6];
int bombeCount = 0;

// Permet de compter le temps écoulé
unsigned long lastAddTime = 0; 

//Fonction pour ajouter une bombe au tableau
void addCoord(int x, int y) {
  unsigned long currentTime = millis();
  
  if (currentTime - lastAddTime >= 200) {
    if (bombeCount < 6) {
      bombes[bombeCount] = {x, y, currentTime, true}; // Nouvelle bombe active
      bombeCount++;
      lastAddTime = currentTime;
    } else {
      Serial.println("Liste de bombes pleine !");
    }
  }
}



// tableau pour stocker la couleur des pixels de la matrix
bool caseColors[13][13];

bool checkWallCollision(int x, int y){
  if (x < 6 || x > 58 - tailleJoueur || y < 6 || y > 58 - tailleJoueur){
    return true;
  }
 
  
  for (int i = 0; i < 3; i++) { // Première boucle (lignes)
      for (int j = 0; j < tailleJoueur; j++) { // Deuxième boucle (colonnes)
          int caseX = ((x-6)+i)/4 ;
          int caseY = ((y-6)+j)/4;

 
          // test si les coord du joueur sont sur un obstacle
          if (caseColors[caseX][caseY]){
            return true;
          } 

      }
  }
  return false;
}

void moveUp() { 
  effacerJoueur(posX, posY);  
  posY = min(60, posY + speed);
  dessinerJoueur(posX, posY);
}

void moveDown() { 
  effacerJoueur(posX, posY);
  posY = max(0, posY - speed);
  dessinerJoueur(posX, posY);
}

void moveLeft() { 
  effacerJoueur(posX, posY);
  posX = max(0, posX - speed);
  dessinerJoueur(posX, posY);
}

void moveRight() { 
  effacerJoueur(posX, posY);
  posX = min(60, posX + speed);
  dessinerJoueur(posX, posY);
}

void effacerJoueur(int posX, int posY){
    matrix.fillRect(posX, posY, tailleJoueur, tailleJoueur, matrix.Color333(0, 0, 0));
}

void dessinerJoueur(int posX, int posY){
    matrix.fillRect(posX, posY, tailleJoueur, tailleJoueur, matrix.Color333(0, 0, 7));
}

void dessinerBombe(int posX, int posY) {
  	matrix.drawLine(posX+1, posY, posX+1, posY+2, matrix.Color333(7, 0, 0));
    matrix.drawLine(posX, posY+1, posX+2, posY+1, matrix.Color333(7, 0, 0));
}

void dessinerExplosion(int posX, int posY) {
  //Explosion du haut
  matrix.drawLine(posX, posY, posX, 6, matrix.Color333(7, 0, 0));
  matrix.drawLine(posX+1, posY, posX+1, 6, matrix.Color333(7, 0, 0));
  matrix.drawLine(posX+2, posY, posX+2, 6, matrix.Color333(7, 0, 0));

  //Explosion de droite
  matrix.drawLine(posX+3, posY, 57, posY, matrix.Color333(7, 0, 0));
  matrix.drawLine(posX+3, posY+1, 57, posY+1, matrix.Color333(7, 0, 0));
  matrix.drawLine(posX+3, posY+2, 57, posY+2, matrix.Color333(7, 0, 0));

  //Explosion du bas
  matrix.drawLine(posX, posY-2, posX, 57, matrix.Color333(7, 0, 0));
  matrix.drawLine(posX+1, posY-2, posX+1, 57, matrix.Color333(7, 0, 0));
  matrix.drawLine(posX+2, posY-2, posX+2, 57, matrix.Color333(7, 0, 0));

  //Explosion de gauche
  matrix.drawLine(posX, posY, 6, posY, matrix.Color333(7, 0, 0));
  matrix.drawLine(posX, posY+1, 6, posY+1, matrix.Color333(7, 0, 0));
  matrix.drawLine(posX, posY+2, 6, posY+2, matrix.Color333(7, 0, 0));
}


unsigned long bombTimes[6]; // Stocke les temps de pose
bool bombActive[6]; // Indique si une bombe est active

void verifierBombes() {
    unsigned long currentTime = millis();
    for (int i = 0; i < bombeCount; i++) {
        if (bombes[i].active && currentTime - bombes[i].timePosed >= 3000) {
            bombes[i].active = false; // Désactiver la bombe
            explosion(bombes[i].x, bombes[i].y);
        }
    }
}


void explosion(int x, int y) {
    dessinerExplosion(x, y);
}




void setup() {
  pinMode (axeX, INPUT); // définition de A6 comme une entrée
  pinMode (axeY, INPUT); // définition de A7 comme une entrée
  Serial.begin (9600);

  matrix.begin();
  
  // dessine le mur 
  for (int i = 0; i <= 63; i++) {
    for (int j = 0; j <= 63; j++) {
      if( i<=5 || j<=5 || i >=58 || j>=58){
        matrix.drawPixel(i,j, matrix.Color333(3, 3, 0));
      }
    }
  }

  // dessiner les obstacles
  for (int k = 0; k <= 5; k++) {
    for (int j = 0; j <= 5; j++) {
      matrix.fillRect(10+k*8, 10+j*8, 4, 4, matrix.Color333(3, 3, 0));
      }
    }

  // enregistrer les obstacles
  for (int i = 0; i < 13; i++){
    for (int j = 0; j < 13; j++){
      if (i % 2 == 1 && j % 2 == 1) {
        caseColors[i][j] = true;
      }
      else {
        caseColors[i][j] = false;
      }
    }
  }

  //dessine le joueur
  dessinerJoueur(posX, posY);

 }



void loop() {
  const bool tabFinal = caseColors;
  X = analogRead (axeX);
  Y = analogRead (axeY);

  /*Serial.print("axeX : ");
  Serial.print("axeY : ");
    */
  for(int i=0; i < bombeCount; i++){
      if (bombes[i].active) {
          dessinerBombe(bombes[i].x, bombes[i].y);
      }
  }
  verifierBombes();


  if(X == 1023){
    addCoord(posX, posY);
  }

  if(X < 450 && ! checkWallCollision(posX+1,posY)){
    moveRight();
  }
  if(X > 550 && X < 1023 && ! checkWallCollision(posX-1,posY)){
    moveLeft();
  }
  if(Y > 550 && ! checkWallCollision(posX,posY+1)){
    moveUp();
  }
  if(Y < 450 && ! checkWallCollision(posX,posY-1)){
    moveDown();
  }
  
  delay(40); // Simule le mouvement toutes les secondes

}



