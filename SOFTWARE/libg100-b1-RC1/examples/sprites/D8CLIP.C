#include <db-lib.h>

unsigned char sprite[3][8]={
0x7E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7E,
0x7E,0x81,0x81,0x81,0x81,0x81,0x81,0x7E,
0x81,0x0,0x0,0x0,0x0,0x0,0x0,0x81};

char bg[16]={ 0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,
              0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55 };

void main()
{ int i,j;
  clear3(SEG1); // on efface les 3 premiers buffers
  mode(0xdb);   // on passe en mode db
  contrast(15); // on initialise le contraste a un niveau convenable pour du gris
  desactint9(); // on desactive l'interruption 9
  for(j=0;j<64;j+=8)
  for(i=0;i<128;i+=8)
  d8_or(i,j,bg,SEG1); // affiche l'arriere plan
  copyscreen(SEG1,SEG4); // on sauve l'arriere plan sur un autre buffer
  j=i=0;
  d8clip_and(i,j,sprite[2],SEG1); // on pose le masque du sprite sur les 2 buffers
  d8clip_and(i,j,sprite[2],SEG3);
  d8clip_or(i,j,sprite[0],SEG1);  // puis on affiche le sprite sur les 2 buffers
  d8clip_or(i,j,sprite[1],SEG3);
  do{ temps(100);  // on ralentit un peu le programme
      if (testkey(5,1) || testkey(4,7)) break; // ESC ou EXE quitte le programme
      if (testgroup(0x78,0x200))
      { if (testkey(1,9)) i--; // GAUCHE
        if (testkey(4,9)) i++; // DROITE
        if (testkey(2,9)) j--; // HAUT
        if (testkey(3,9)) j++; // BAS

        copyscreen(SEG4,SEG1); // on charge l'arriere plan
        copyscreen(SEG2,SEG3);
        d8clip_and(i,j,sprite[2],SEG1); // on pose le masque du sprite sur les 2 buffers
        d8clip_and(i,j,sprite[2],SEG3);
        d8clip_or(i,j,sprite[0],SEG1);  // puis on affiche le sprite sur les 2 buffers
        d8clip_or(i,j,sprite[1],SEG3);}

  }while (1);
  clear3(SEG1); // on initialise les parametres pour du mode c3
  contrast(8);
  mode(0xc3);
  actint9();}
