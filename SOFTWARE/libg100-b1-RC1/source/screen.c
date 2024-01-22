/*####################################################################################
 #		This Code is part of the libg100 library : http://libg100.sf.net             #
 #			 	  	 	  	 	 		 		   									 #
 #		THIS LIBRARY IS DISTRIBUTED "AS IS".                						 #
 #		NO WARRANTY OF ANY KIND IS EXPRESSED OR IMPLIED.  							 #
 #		YOU USE AT YOUR OWN RISK.                  									 #
 #		THE AUTHOR WILL NOT BE LIABLE FOR DATA LOSS, DAMAGES,  						 #
 #		LOSS OF PROFITS OR ANY OTHER KIND OF LOSS            						 #
 #		WHILE USING OR MISUSING THIS LIBRARY.           							 #
 #                                                                                   #
 #		This library is Distributed in LGPL License, read lgpl.txt or go to :		 #
 #		   http://opensource.org/licenses/lgpl-license.php 		   	  	 			 #                                                                          #/
 ####################################################################################*/
 
/************
 *
 *           libcg100 : screen.c
 *
**************/

#include "screen.h"

static int peek(unsigned segm, unsigned offs)
{
	asm{
		mov ax, segm
		mov es, ax
		mov ax, offs
		mov di, ax
		mov ax, es:[di]
 	}
	return _AX;
}

typedef unsigned char BMPTabType[1024];

short 			 getvideoseg		   (void)
{
	short SEGMENT = peek(0x4E, 0);
	asm mov ax, SEGMENT;
	asm mov es, ax;
	return SEGMENT;
}

void             mode                  (unsigned char modetype)
{
  asm{
       mov al,modetype
       out 0x2,al
     }
}

unsigned char    db_on                 ()
{
  asm{
       mov al,0xdb
       out 0x2,al
       mov dl,0xF
       mov ax,0xC000
       mov es,ax
       mov di,0xB614
       mov al,es:[di]
       cmp al,0x54
       jne contraste
       xor dl,0x1F
     }
  contraste:
  contrast(_DL);
  return (_DL & 0x10);
}

void             db_off                ()
{ asm{
       mov dl,0x8
       mov ax,0xC000
       mov es,ax
       mov di,0xB614
       mov al,es:[di]
       cmp al,0x54
       jne contraste
       or dl,0x1
     }
  contraste:
  contrast(_DL);
  asm{
       mov al,0xc3
       out 0x2,al
     }
}

void             disp_bmp              (BMPTabType BmpTab, unsigned int segm)
{
  asm{
       mov ax,segm
       mov es,ax
       mov si,BmpTab
       xor di,di
       mov cx,0x200
       cld
       rep movsw
     }
}

void             clear                 (unsigned int segm)
{
  asm{
       mov ax,segm
       mov es,ax
       xor ax,ax
       xor di,di
       mov cx,0x200
       cld
       rep stosw
     }
}

void             clear3                (unsigned int segm)
{
  asm{
       mov ax,segm
       mov es,ax
       xor ax,ax
       xor di,di
       mov cx,0x600
       cld
       rep stosw
     }
}

void             movevideo             (unsigned int segm)
{
  asm{
       mov ax,segm
       xor dx,dx
       shl ax,0x1
       rcl dx,0x1
       shl ax,0x1
       rcl dx,0x1
       shl ax,0x1
       rcl dx,0x1
       shl ax,0x1
       rcl dx,0x1
       out 0x05,ax
       mov al,dl
       out 0x07,al
     }
}

void             invscreen             (unsigned int segm)
{
  asm{
       mov ax,segm
       mov es,ax
       or ax,0xFFFF
       xor di,di
       mov cx,0x200
     }
  loop:
  asm{
       xor es:[di],ax
       xor es:[di+0x800],ax
       add di,0x2
       dec cx
       jne loop
     }
}

void             contrast              (unsigned char niv)
{
  asm mov ch,0x16
  init:
  asm{
       mov ah,0x22         // ÚÄÄÄÄÄÂÄÄÄÄÄ¿
       mov bl,0x1          // ³g 100³g100+³
       int 0x7C            // ÃÄÄÄÄÄÅÄÄÄÄÄ´
       dec ch              // ³  0  ³  0  ³  contraste minimal
       jne init            // ³  8  ³  9  ³  contraste normal
       mov ch,niv          // ³  12 ³  12 ³  contraste initial
     }                     // ³  15 ³  16 ³  contraste optimal pour le gris
  loop:                    // ³  22 ³  22 ³  contraste maximal
  asm{                     // ÀÄÄÄÄÄÁÄÄÄÄÄÙ
       mov ah,0x22
       xor bl,bl
       int 0x7C
       dec ch
       jne loop
     }
}

void             contrast_chg          (unsigned char sens, unsigned char niv)
{
  unsigned char i;
  if (sens)
  {
    for (i=0;i<niv;i++)
    {
      asm{
           mov ah,0x22
           xor bl,bl
           int 0x7C
         }
    }
  }
  else 
  {
    for (i=0;i<niv;i++)
    {
      asm{
           mov ah,0x22
           mov bl,0x1
           int 0x7C
         }
    }
  }
}

void             rightscroll           (unsigned int x, unsigned int srcsegm, unsigned int segm)
{
  asm{
       mov si,0xF       // met 15 dans si (bloc de donnee en bas a gauche du buffer)
       xor di,di        // efface di, di=0 (bloc de donnee en bas a droite)
       mov ax,segm      // met le buffer choisi
       mov es,ax
       mov ax,srcsegm   // met dans ax le buffer source
       sub ax,segm      // ax=srcsegm-segm;
       shl ax,0x4       // ax*=16; calcule le nombre de bloc de donnee entre srcsegm et segm
       add si,ax        // met dans si la ligne du bloc de donnee de la colonne a faire apparaitre
       mov dh,0x40      // dh=64; nombre de fois a repeter la boucle(nombre de lignes d'un buffer)
       mov cx,x         // cx prends la valeur de x
       mov ax,cx        // ax prends la valeur de x
       shr ax,0x3       // ax/=8; calcul pour determiner "l'abscisse" du bloc de donnee source
       sub si,ax        // si = valeur exacte du bloc de donnee source a utiliser
       and cl,0x7       // calcul du decalage en fonction de x
       inc cl           // ajoute 1 a cl pour que le calcul soit exact
     }
  loopline:             // debut de la double boucle
  asm{
       mov bl,es:[si]   // bl = bloc de donnee source
       rol bl,cl        // decalage de bl pour faire correspondre le bit voulu au niveau du bit 0
       and bl,0x1       // efface les bits inutiles (de 1 a 7) et garde le bit 0
       mov ch,0x8       // ch=8; nombre de fois a repeter la deuxieme boucle (nombre de mots sur une ligne 8*16=64)
       add si,0x10      // si+=16; augmente le bloc de donnee source d'une ligne
     }
  loop:                 // la deuxieme boucle
  asm{
       mov ax,es:[di]   // ax = bloc de destination
       mov dl,ah        // la partie haute de ax dans dl
       rol dl,0x1       // passe le 7eme bit au bit 0 par un decalage
       and dl,0x1       // on garde le bit 0 de dl
       shl ax,0x1       // ax decale de 1
       or al,bl         // on met le contenu de dl dans al
       mov es:[di],ax   // on reaffiche le tout sur le buffer choisi (segm)
       mov bl,dl        // on met le contenu de dl dans bl
       add di,0x2       // passe au bloc de destination suivant
       dec ch           // decremente ch (ch correspond au nombre de bytes dans une ligne)
       jne loop         // continue la boucle tant que ch different de 0
       dec dh           // decremente dh (dh correspond au nombre de ligne dans un buffer)
       jne loopline
     }
}

void             leftscroll            (unsigned int x, unsigned int srcsegm, unsigned int segm)
{
  asm{
       mov si,0x3FF     // met 1023 dans si (bloc de donnee en haut a gauche du buffer)
       mov di,0x3FE     // met 1022 dans di (bloc de donnee en haut a gauche-1)
       mov ax,segm      // met le buffer choisi
       mov es,ax
       mov ax,srcsegm   // met dans ax le buffer source
       sub ax,segm      // ax=srcsegm-segm;
       shl ax,0x4       // ax*=16; calcule le nombre de bloc de donnee entre srcsegm et segm
       add si,ax        // met dans si la ligne du bloc de donnee de la colonne a faire apparaitre
       mov dh,0x40      // dh=64; nombre de fois a repeter la boucle(nombre de lignes d'un buffer)
       mov cx,x         // cx prends la valeur de x
       mov ax,cx        // ax prends la valeur de x
       shr ax,0x3       // ax/=8; calcul pour determiner "l'abscisse" du bloc de donnee source
       sub si,ax        // si = valeur exacte du bloc de donnee source a utiliser
       and cl,0x7       // calcul du decalage en fonction de x
     }
  loopline:             // debut de la double boucle
  asm{
       mov bl,es:[si]   // bl = bloc de donnee source
       shl bl,cl        // decalage de bl pour faire correspondre le bit voulu au niveau du bit 7
       and bl,0x80      // efface les bits inutiles (de 0 a 6) et garde le bit 7
       mov ch,0x8       // ch=8; nombre de fois a repeter la deuxieme boucle (nombre de mots sur une ligne 8*16=64)
       sub si,0x10      // si-=16; descend le bloc de donnee source d'une ligne
     }
  loop:                 // la deuxieme boucle
  asm{
       mov ax,es:[di]   // ax = bloc de destination
       mov dl,al        // la partie basse de ax dans dl
       and dl,0x1       // on garde le bit 0 de dl
       ror dl,0x1       // et on le passe au 7eme bit de dl
       shr ax,0x1       // on decale ax de 1
       or ah,bl         // on met le contenu de dl dans ah
       mov es:[di],ax   // on reaffiche le tout sur le buffer choisi (segm)
       mov bl,dl        // on met le contenu de dl dans bl
       sub di,0x2       // passe au bloc de destination suivant
       dec ch           // decremente ch (ch correspond au nombre de bytes dans une ligne)
       jne loop         // continue la boucle tant que ch different de 0
       dec dh           // decremente dh (dh correspond au nombre de ligne dans un buffer)
       jne loopline
     }
}

void             upscroll              (unsigned int y, unsigned int srcsegm, unsigned int segm)
{
  asm{
       mov ax,segm      // met le buffer choisi
       mov es,ax
       mov di,0x10      // di=16; (bloc de donnee en bas a droite au dessus de l'octet 0)
       mov si,srcsegm   // si=srcsegm
       sub si,segm      // si=(srcsegm-segm)
       sub si,y         // si=(srcsegm-segm-y)
       shl si,0x4       // si=(srcsegm-segm-y)*16
       add si,0x3F0     // si=((srcsegm-segm-y)*16)+1007 -> si pointe vers le bon octet
       mov cx,0x1F8     // nombre de fois a repeter la boucle (8*63=504)
     }
  loopline:
  asm{
       mov ax,es:[di]   // copie dans ax le mot pointe par di
       mov es:[di-0x10],ax // copie ax sur le mot en dessous du mot pointe par di
       add di,0x2       // passe au mot suivant
       dec cx           // decremente cx
       jne loopline     // tant que cx different de 0, continue la boucle
       or ch,0x8        // ch=0
       sub di,0x10      // retire 16 a di pour qu'il pointe sur le bon octet
     }
  loop:
  asm{
       mov ax,es:[si]   // copie le mot de la ligne choisie dans ax
       mov es:[di],ax   // copie ax sur le mot pointe par di
       add di,0x2       // passe au mot suivant sur le buffer choisi (segm)
       add si,0x2       // passe au mot de la ligne choisie suivant
       dec ch           // decremente ch
       jne loop         // tant que ch different de 0, continue la boucle
     }
}

void             downscroll            (unsigned int y, unsigned int srcsegm, unsigned int segm)
{
  asm{
       mov ax,segm      // met le buffer choisi
       mov es,ax
       mov di,0x3EE     // di=1006; (bloc de donnee en haut a gauche au dessous de l'octet 1022)
       mov si,srcsegm   // si=srcsegm
       sub si,segm      // si=(srcsegm-segm)
       sub si,y         // si=(srcsegm-segm-y)
       shl si,0x4       // si=(srcsegm-segm-y)*16
       add si,0x3FE     // si=((srcsegm-segm-y)*16)+1022 -> si pointe vers le bon octet
       mov cx,0x1F8     // nombre de fois a repeter la boucle (8*63=504)
     }
  loopline:
  asm{
       mov ax,es:[di]   // copie dans ax le mot pointe par di
       mov es:[di+0x10],ax // copie ax sur le mot au dessus du mot pointe par di
       sub di,0x2       // passe au mot suivant
       dec cx           // decremente cx
       jne loopline     // tant que cx different de 0, continue la boucle
       or ch,0x8        // ch=0
       add di,0x10      // ajoute 16 a di pour qu'il pointe sur le bon octet
    }
  loop:
  asm{
       mov ax,es:[si]   // copie le mot de la ligne choisie dans ax
       mov es:[di],ax   // copie ax sur le mot pointe par di
       sub di,0x2       // passe au mot suivant sur le buffer choisi (segm)
       sub si,0x2       // passe au mot de la ligne choisie suivant
       dec ch           // decremente ch
       jne loop         // tant que ch different de 0, continue la boucle
     }
}
