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
#include <VirtualWire.h>
#include <TimerFive.h>


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


int RF_RX_PIN = 8;
int RF_TX_PIN = 7;
int RF_PTT_PIN = 30;

float X, Y;
int X2 = 512;
int Y2 = 512;
int speed = 1;
int tailleJoueur = 3;
  // JOYSTICK
int axeX = A6; // signal de l'axe X sur entrée A0
int axeY = A7; // signal de l'axe Y sur entrée A1


// Variables pour contrôler la fréquence de mise à jour
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 40; // 60 FPS (~16ms)

/* 
* Tableau pour stocker la couleur des pixels de la matrix
* Ici seulement déclaré
*/
bool caseColors[13][13];

bool isPlay = true;

DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, _HIGH);


//Structure coordonnées pour les bombes
struct Bombe {
  int x;
  int y;
  unsigned long timePosed;
  bool active;
  int explosionHits[12];
};

struct Coord{
  int x;
  int y;
};

struct Player {
  //numero joueur
  int num;
  //Cordonnées du joueur
  Coord c;
  int color[3];
  //Tableau des coordonnées des bombes posées
  Bombe bombes[6];
  int bombCount = 0;
  // Permet de compter le temps écoulé
  unsigned long lastAddTime = 0; 
  unsigned long bombTimes[6]; // Stocke les temps de pose
  bool bombActive[6]; // Indique si une bombe est active
};

Player P1;  
Player P2;  



// FONCTIONS LIEE A UN JOUEUR
Coord movePlayer(Coord c, float X, float Y){

  int posX = c.x;
  int posY = c.y;

  if(X < 450 && ! checkWallCollision(posX+1,posY)){
    c = moveUp(c);
  }
  if(X > 550 && X < 1023 && ! checkWallCollision(posX-1,posY)){
    c = moveDown(c);
  }
  if(Y > 550 && ! checkWallCollision(posX,posY+1)){
    c = moveRight(c);
  }
  if(Y < 450 && ! checkWallCollision(posX,posY-1)){
    c = moveLeft(c);
  }

  return c;
}

/*
* Vérifie la colision entre un joueur et les murs + obstacles
*/
bool checkWallCollision(int x, int y){
  if (x < 6 || x > 58 - tailleJoueur || y < 6 || y > 58 - tailleJoueur){
    return true;
  }
 
  for (int i = 0; i < tailleJoueur; i++) { // Première boucle (lignes)
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

Coord moveUp(Coord c) { 
  effacerJoueur(c.x, c.y);
  c.x = min(60, c.x + speed);
  return c;
}
Coord moveDown(Coord c) { 
  effacerJoueur(c.x, c.y);
  c.x = max(0, c.x - speed);
  return c;
}
Coord moveRight(Coord c) { 
  effacerJoueur(c.x, c.y);
  c.y = min(60, c.y + speed);
  return c;
}
Coord moveLeft(Coord c) { 
  effacerJoueur(c.x, c.y);
  c.y = max(0, c.y - speed);
  return c;
}

void effacerJoueur(int posX, int posY){
    matrix.fillRect(posX, posY, tailleJoueur, tailleJoueur, matrix.Color333(0, 0, 0));
}
void dessinerJoueur(int posX, int posY, int color[]){
    matrix.fillRect(posX, posY, tailleJoueur, tailleJoueur, matrix.Color333(color[0], color[1], color[2]));
}



// FONCTIONS LIEE A UNE BOMBE

//Fonction pour ajouter une bombe au tableau
Player addBomb(Player p) {
  unsigned long currentTime = millis();
  int x = p.c.x;
  int y = p.c.y;

  if (currentTime - p.lastAddTime >= 200) {
    if (p.bombCount < 6) {
      for(int i = p.bombCount-1; i >= 0; i--){
        p.bombes[i+1] = p.bombes[i];
      }
      p.bombes[0] = {x, y, currentTime, true}; // Nouvelle bombe active
      p.bombCount++;
      p.lastAddTime = currentTime;
    } else {
      Serial.println("Liste de bombes pleine !");
    }
  }
  return p;
}

void dessinerBombes(int bombCount, Bombe bombes[6]){
  for(int i=0; i < bombCount; i++){
      if (bombes[i].active) {
          dessinerBombe(bombes[i].x, bombes[i].y);
      }
  }
}

void dessinerBombe(int posX, int posY) {
  	matrix.drawLine(posX+1, posY, posX+1, posY+2, matrix.Color333(7, 0, 0));
    matrix.drawLine(posX, posY+1, posX+2, posY+1, matrix.Color333(7, 0, 0));
}

void verifierBombes(Player &P) {
  unsigned long currentTime = millis();

  for (int i = 0; i < P.bombCount; i++) {
    if (P.bombes[i].active && currentTime - P.bombes[i].timePosed >= 2600) {
        P.bombes[i] = handleExplosion(P.bombes[i]);
        P.bombes[i].active = false; // Désactiver la bombe
    }
    if (!P.bombes[i].active && currentTime - P.bombes[i].timePosed >= 3000) {
        P.bombes[i] = handleExplosion(P.bombes[i]);
        P.bombes[i] = {}; //ici supprimer la bombe sinon lag parce que vérification en boucle
        for(int j = i; j < P.bombCount; j++){ // redessine les bombes en train d'exploser après avoir effacé celle qui a finit
          P.bombes[i] = handleExplosion(P.bombes[j]);
        }
    }
  }
  P.bombCount = countBombNumber(P.bombes);
  
}

Bombe handleExplosion(Bombe b) {
  b = fillExplosionHits(b);
  dessinerExplosion(b);

  return b;
}

/*
* Remplit le tableau explosionHits
*/
Bombe fillExplosionHits(Bombe b){
  //haut 
  for(int i = 0; i < 3; i++){
    int coordY = b.y;
    while(! checkBlockGetExplosion(b.x+i, coordY) && coordY > 5){
      coordY--;
    }
    b.explosionHits[i] = coordY +1;
  }
  //droite
  for(int i = 0; i < 3; i++){
    int coordX = b.x;
    while(! checkBlockGetExplosion(coordX, b.y +i) && coordX < 58){
      coordX++;
    }
    b.explosionHits[i+3] = coordX -1;
  }
  //bas 
  for(int i = 0; i < 3; i++){
    int coordY = b.y;
    while(! checkBlockGetExplosion(b.x+i, coordY) && coordY < 58){
      coordY++;
    }
    b.explosionHits[i+6] = coordY -1;
  }
  //gauche
  for(int i = 0; i < 3; i++){
    int coordX = b.x;
    while(! checkBlockGetExplosion(coordX, b.y +i) && coordX > 5){
      coordX--;
    }
    b.explosionHits[i+9] = coordX +1;
  }

  return b;
}

/*
* Regarde si un bloc est touché par l'explosion
*/
bool checkBlockGetExplosion(int x, int y){
    int caseX = (x-6)/4;
    int caseY = (y-6)/4;
    if (caseColors[caseX][caseY]){
      return true;
    } 
    return false;
}

/*
* Dessine l'explosion sur la matrice
*/
void dessinerExplosion(Bombe b) {
  
  uint16_t color;

  if(b.active){
    color = matrix.Color333(7, 0, 0);
  }
  else {
    color = matrix.Color333(0, 0, 0);
  }

  int posX = b.x;
  int posY = b.y;
  //Explosion du haut
  matrix.drawLine(posX, posY, posX, b.explosionHits[0], color);
  matrix.drawLine(posX+1, posY, posX+1, b.explosionHits[1], color);
  matrix.drawLine(posX+2, posY, posX+2, b.explosionHits[2], color);

  //Explosion de droite
  matrix.drawLine(posX+2, posY, b.explosionHits[3], posY, color);
  matrix.drawLine(posX+2, posY+1, b.explosionHits[4], posY+1, color);
  matrix.drawLine(posX+2, posY+2, b.explosionHits[5], posY+2, color);

  //Explosion du bas
  matrix.drawLine(posX, posY, posX, b.explosionHits[6], color);
  matrix.drawLine(posX+1, posY, posX+1, b.explosionHits[7], color);
  matrix.drawLine(posX+2, posY, posX+2, b.explosionHits[8], color);

  //Explosion de gauche
  matrix.drawLine(posX, posY, b.explosionHits[9], posY, color);
  matrix.drawLine(posX, posY+1, b.explosionHits[10], posY+1, color);
  matrix.drawLine(posX, posY+2, b.explosionHits[11], posY+2, color);
}

/*
* Met à jour le nombre de bombe d'un joueur
*/
int countBombNumber(Bombe bombes[6]){
  int bombNumber = 0;
  while(bombes[bombNumber].timePosed > 0 ){
    bombNumber ++;
    if (bombNumber == 6){
      break;
    }
  }
  return bombNumber;
}



// PROGRAMME PRINCIPAL 

void setup() {

  P1.num = 1;
  P2.num = 2;

  // Position de départ
  P1.c.x = 6;
  P1.c.y = 6;
  P2.c.x = 55;
  P2.c.y = 55;
  
  //Couleur des joueurs 
  P1.color[2] = 7;
  P2.color[1] = 7;

  
  // Récepteur
  Serial.begin(9600);
  vw_set_rx_pin(RF_RX_PIN);  // initialisation de la broche de reception
  vw_set_tx_pin(RF_TX_PIN);  
  vw_set_ptt_pin(RF_PTT_PIN);  
  vw_setup(2200); // choix de la vitesse de transmission
  vw_rx_start(); //démarrage du récepteur

  // matrix
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
  dessinerJoueur(P1.c.x, P1.c.y, P1.color);
  dessinerJoueur(P2.c.x, P2.c.y, P2.color);

 }



void loop() {

  // Gestion du récepteur
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // non-blocking I/O
  {
    buf[buflen] = '\0';
    // si on a reçu un message
    Serial.print("Reçu : ");
    String message = (char*) buf;
    Serial.println(message);
    sscanf((char*)buf, "%d;%d", &X2, &Y2);
  }
  // Gestion du jeu
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= updateInterval && isPlay) {
    
    // joueur 1
    X = analogRead(axeX);
    Y = analogRead(axeY);

    dessinerBombes(P1.bombCount, P1.bombes);
    verifierBombes(P1);
    if(X == 1023){
      P1 = addBomb(P1);
    }
    P1.c = movePlayer(P1.c, X, Y);
    dessinerJoueur(P1.c.x, P1.c.y, P1.color);
    
    // joueur 2
    dessinerBombes(P2.bombCount, P2.bombes);
    verifierBombes(P2);
    if(X2 == 1023){
      P2 = addBomb(P2);
    }
    P2.c = movePlayer(P2.c, X2, Y2);
    dessinerJoueur(P2.c.x, P2.c.y, P2.color);

    isPlay = !(checkPlayerGetHit(P1, P2) || checkPlayerGetHit(P2, P1) || checkPlayerGetHit(P1, P1) || checkPlayerGetHit(P2, P2));
    lastUpdate = currentTime;
  }
  
 
}


/*
* Fonction qui vérifie si le joueur est bombed
*/
bool checkPlayerGetHit(Player p1, Player p2){
  int posX = p1.c.x;
  int posY = p1.c.y;
  // Vérifie si le joueur se trouve sur le même axe qu'une bombe
  for(int i=0; i<p2.bombCount; i++){
    unsigned long currentTime = millis();
    if(! p2.bombes[i].active){
      // axe X
      if(abs(posX - p2.bombes[i].x) < 3){
        // pour chaque ligne de l'explosion
        for(int y=0; y<3; y++ ){
          // pour chaque ligne du joueur
          for(int k=0; k<3; k++){
            // si le joueur est touché par une ligne
            if(p2.bombes[i].x+y == posX+k){
              // vérifier que l'explosion n'est pas arrêtée pas un obstacle
              if(p2.bombes[i].explosionHits[y] <= posY && posY <= p2.bombes[i].x+y  || p2.bombes[i].explosionHits[y+6] >= posY && posY >= p2.bombes[i].x+y){
                
                ecran_de_fin(p1.num, p2.num);
                return true;
              }
            }
          }
        }
      }
      // axe Y
      if(abs(posY - p2.bombes[i].y) < 3){
         // pour chaque colonne de l'explosion
        for(int y=0; y<3; y++ ){
          // pour chaque colonne du joueur
          for(int k=0; k<3; k++){
            // si le joueur est touché par une colonne
            if(p2.bombes[i].y+y == posY+k){
              // vérifier que l'explosion n'est pas arrêtée pas un obstacle
              if(p2.bombes[i].explosionHits[y+9] <= posX && posX <= p2.bombes[i].y+y  || p2.bombes[i].explosionHits[y+3] >= posX && posX >= p2.bombes[i].y+y){

                ecran_de_fin(p1.num, p2.num);
                return true;
              } 
            }
          }
        }
       
      }
    } 
  }
  return false;
}

/*
* Affiche l'écran de fin de jeu
*/
void ecran_de_fin(int joueurX, int joueurY) {
  // Effacer l'écran
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
  // Définir la couleur du texte (bleu)
  uint16_t textColor = matrix.Color333(0, 0, 7);
  
  // Message à afficher : "Joueur X bombed by Joueur Y"
  char message[30];
  sprintf(message, "J%d bombed by J%d", joueurX, joueurY);
  
  // Position initiale pour centrer le texte 
  int x = 5; // Début à gauche avec une petite marge
  int y = 20; // Centré verticalement 
  
  // Afficher chaque caractère du message
  for (int i = 0; i < strlen(message); i++) {
    matrix.drawChar(x, y, message[i], textColor, matrix.Color333(0, 0, 0), 1);
    x += 6; // Chaque caractère prend environ 6 pixels de large
    // Si on dépasse la largeur de l'écran, passer à la ligne suivante
    if (x > 60) {
      x = 5;
      y += 12; // Nouvelle ligne (hauteur d'un caractère)
    }
  }
}

