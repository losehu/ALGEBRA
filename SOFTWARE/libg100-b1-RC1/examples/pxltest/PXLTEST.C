#include <stdio.h>
#include <conio.h>

#include <dbpaint.h>
#include <calc.h>
#include <screen.h>
#include <keyboard.h>

unsigned char ss[8]={
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; 

unsigned char ecran[64][128]={0}; // pour sauvegarder les donnees

void main()
{ int i,j;
  clear3(SEG1); // le traditionnel debut de programme :
  mode(0xdb);   // on efface les 3 premiers buffers, on passe en mode db,
  contrast(15); // on met le contraste pour du gris
  for(j=0;j<64;j+=8) // on dessine un arriere plan
  for(i=0;i<32;i+=8)
  { d8_or(i,j,ss,SEG1);
    d8_or(i,j,ss,SEG3);}
  for(j=0;j<64;j+=8)
  for(i=32;i<96;i+=8)
  d8_or(i,j,ss,SEG1);
  getch();getch(); // on attends une touche
  desactint9(); // on desactive l'interruption 9.
  for (j=0;j<64;j++)  // on teste chaque pixel de l'ecran
  for (i=0;i<128;i++) // et on inscrit sa valeur dans une variable
  ecran[j][i]=pxltest(i,j,SEG1);
  mode(0xc3); // on revient en mode noir et blanc (pour printf)
  contrast(8); // on remet donc le contraste noir et blanc
  clear3(SEG1); // on efface les buffers ou on a affiche
  //gotoxy(1,1); // on positionne le curseur 
  for (j=0;j<64;j++)
  for (i=0;i<128;i++)
  { printf("%d",ecran[j][i]); // puis on reaffiche les valeurs des pixels sauves dans la variable
    if (testkey(4,7)) goto end; // si ESC, quitte le programme
    while (!testgroup(0x78,0x200) && !testkey(4,7));} // tant qu'on appuie pas sur une touche du pad ou sur ESC

  end:
  actint9();
  contrast(8);
  clear3(SEG1);}
