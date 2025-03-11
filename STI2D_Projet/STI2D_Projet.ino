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
#include <Player.h>

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

DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, _HIGH);

Player player(&matrix, 6, 6);  // Passe la matrice en paramètre

// tableau pour stocker la couleur des pixels de la matrix
short pixelColors[64][64];

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
        // stocker le mur
        pixelColors[i][j] = matrix.Color333(3, 3, 0);
      }
    }
  }
  /*
  for (int i = 0; i <= 5; i++) {
    matrix.drawRect(i, i, matrix.width()-2*i, matrix.height()-2*i, matrix.Color333(3, 3, 0));
    // stocker le mur
    pixelColors[]
  }*/

  for (int k = 0; k <= 5; k++) {
    for (int j = 0; j <= 5; j++) {
      matrix.fillRect(10+k*8, 10+j*8, 4, 4, matrix.Color333(3, 3, 0));
      for (int x=0; x<=3; x++){
        for (int y=0; y<=3; y++){
          pixelColors[10+k*8+x][10+j*8+y] = matrix.Color333(3, 3, 0);
        }
      }
      }
  }

  // matrix.fillRect(6, 6, 4, 4, matrix.Color333(0, 0, 7));
}

void loop() {
  X = analogRead (axeX);
  Y = analogRead (axeY);

  Serial.print ("Axe X:");
  Serial.println (X);
  Serial.print ("Axe Y:");
  Serial.println (Y);

  if(X < 450){
    player.moveRight(pixelColors);
  }
  else if(X > 550){
    player.moveLeft(pixelColors);
  }
  else if(Y > 550){
    player.moveUp(pixelColors);
  }
  else if(Y < 450){
    player.moveDown(pixelColors);
  }


  delay(50); // Simule le mouvement toutes les secondes

}


// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos) {
  if(WheelPos < 8) {
   return matrix.Color333(7 - WheelPos, WheelPos, 0);
  } else if(WheelPos < 16) {
   WheelPos -= 8;
   return matrix.Color333(0, 7-WheelPos, WheelPos);
  } else {
   WheelPos -= 16;
   return matrix.Color333(0, WheelPos, 7 - WheelPos);
  }
}
