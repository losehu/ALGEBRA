#include "screen.h"
#include "calc.h"
#include "dbpaint.h"

unsigned char sprite[7][7]={
1,1,1,1,1,1,1,      // 0 : pixel blanc
1,0,0,3,0,2,1,      // 1 : pixel noir
1,0,3,3,3,0,1,      // 2 : pixel gris
1,0,0,3,0,0,1,      // 3 : inversion de pixel
1,0,3,3,3,0,1,
1,2,0,3,0,0,1,
1,1,1,1,1,1,1};

unsigned char ss[8]={ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF }; // que du noir


void drawsprite(unsigned int x,unsigned char y) // une boucle dessinant le sprite
{ int k,l;
  for(l=0;l<7;l++)                 
  for(k=0;k<7;k++)
  pxl(k+x,l+y,sprite[l][k],SEG1);} 

void main()
{ int i,j;
  clear3(SEG1);    // le traditionnel debut de programme :
  mode(0xdb);      // on efface les 3 premiers buffers, on passe en mode db,
  contrast(15);    // on met le contraste pour du gris, et on desactive
  desactint9();    // l'interruption 9.
  for(j=0;j<64;j+=8) // boucle parcourant la partie gauche de l'ecran 
  for(i=0;i<32;i+=8)
  { d8_or(i,j,ss,SEG1);  // affiche du noir a l'ecran en mettant des pixels allumes
    d8_or(i,j,ss,SEG3);} // sur la couche 1 et 3 de la memoire video
  for(j=0;j<64;j+=8)  // boucle parcourant le milieu de l'ecran
  for(i=32;i<96;i+=8) // affiche du gris a l'ecran en mettant des pixels 
  d8_or(i,j,ss,SEG1); // allumes que sur la couche 1 de la memoire video
  copyscreen(SEG1,SEG4); // copie sur d'autres buffers l'ecran actuel
  copyscreen(SEG3,SEG5); 
  j=0;i=0;drawsprite(i,j); // dessine le sprite
  do{                      // debut de la boucle
  if (testkey(4,7)) goto end; // si ESC, quitte le programme
  if (testkey(6,10)){ shutcalc();mode(0xdb);} // si F6, eteinds la calculatrice
  if (testkey(1,10)){   // si F1, inverse l'ecran
  copyscreen(SEG4,SEG1);
  copyscreen(SEG5,SEG3);
  invscreen(SEG1);drawsprite(i,j);}
  if (testgroup(0x78,0x200)){ // si une des touches multidirectionnelles est appuyee
  if (testkey(2,9)) j--;      // si c'est HAUT decremente j
  if (testkey(3,9)) j++;      // si c'est BAS incremente j
  if (testkey(1,9)) i--;      // si c'est GAUCHE decremente i
  if (testkey(4,9)) i++;      // si c'est DROITE incremente i
  copyscreen(SEG4,SEG1);      // affiche l'ecran tel qu'on l'a sauvegarde
  copyscreen(SEG5,SEG3);
  drawsprite(i,j);}           // puis dessine le sprite au coordonnees (i,j)
  }while (1);                 // fin de la boucle

  end:                        // la traditionnelle fin de programme:
  actint9();                  // reactivation de l'interruption 9
  contrast(8);                // retourne un contraste pour mode c3
  clear3(SEG1);               // efface les 3 premiers buffers de la memoire video
  mode(0xc3);
}                // revient en mode c3
