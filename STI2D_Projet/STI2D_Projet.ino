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



//Structure coordonnées
struct Coord {
  int x;
  int y;
};
//Tableau des coordonnées des bombes posées
Coord coordBombe[6];
int coordCount = 0; 

//Fonction pour ajouter une bombe au tableau
void addCoord(int x, int y) {
  if (coordCount < 6) {
    coordBombe[coordCount] = Coord {x, y};
    coordCount++;
  } else {
    Serial.println("Liste pleine !");
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
  Serial.println(X);
  Serial.print("axeY : ");
  Serial.println(Y);*/
  for(int i; i < coordCount; i++){
    dessinerBombe(coordBombe[i].x, coordBombe[i].y);
  }

  if(X == 1023){
    Serial.println("bombe !");
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



