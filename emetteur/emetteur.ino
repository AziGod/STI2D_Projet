// code de l'émetteur
#include <VirtualWire.h>

int RF_TX_PIN = 2;
int axeX = A6; // signal de l'axe X sur entrée A0
int axeY = A7; // signal de l'axe Y sur entrée A1
int X;
int Y;


 char msg[20];
long t = 0;
void setup()
{
  Serial.begin(9600);
  Serial.println("setup");
  vw_set_tx_pin(RF_TX_PIN); // initialisation de la broche d'envoi
  vw_setup(1440); // choix de la vitesse de transmission
}

void loop()
{
  X = analogRead(axeX);
  Y = analogRead(axeY);
  sprintf(msg, "%d;%d", X, Y);

  Serial.print("envoi d'un message : ");
  Serial.println(msg);
  vw_send((uint8_t *)msg, strlen(msg));

  
}
