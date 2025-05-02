// code du récepteur
#include <DFRobot_RGBMatrix.h> // Hardware-specific library
#include <VirtualWire.h>

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

DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, _HIGH);


int RF_RX_PIN = 4;

void setup()
{
    matrix.begin();
    matrix.setBrightness(3);
    delay(4000);
    Serial.begin(9600);
    Serial.println("setup");
    vw_set_rx_pin(RF_RX_PIN);  // initialisation de la broche de reception
    vw_setup(2000); // choix de la vitesse de transmission
    vw_rx_start(); //démarrage du récepteur
} 

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // non-blocking I/O
  {
    int i;
    // si on a reçu un message
    Serial.print("Reçu : ");
    Serial.println((char*) buf);
  }

  ecran_de_fin(2, 1);
}

void ecran_de_fin(int joueurX, int joueurY) {
  
  
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
