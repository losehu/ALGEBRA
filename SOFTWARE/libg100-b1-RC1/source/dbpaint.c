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
 *           libcg100 : dbpaint.c
 *
**************/

#include "dbpaint.h"

// Affiche un sprite 8*8 pour mode D3 ou DB avec
// OR sans clipping, au buffer voulu.
void d8_or(int x,int y,void* spr,unsigned int segm)
{
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       mov ch,0x8
     }
  loop:
  asm{
       xor al,al
       mov ah,ds:[si]
       shr ax,cl
       or es:[di],ax
       sub di,0x10
       inc si
       dec ch
       jne loop
     }
}


// Affiche un sprite 8*8 pour mode D3 ou DB avec
// AND (masque) sans clipping, au buffer voulu.
void d8_and(int x,int y,void* spr,unsigned int segm)
{
  asm{
       mov cx,x
       mov si,spr
       mov di,0x23E
       mov ax,0x1C
       add ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       mov ch,0x8
     }
  loop:
  asm{
       or ax,0xFFFF
       and ah,ds:[si]
       ror ax,cl
       and es:[di],ax
       sub di,0x10
       inc si
       dec ch
       jne loop
     }
}


// Affiche un sprite 8*8 pour mode D3 ou DB avec
// XOR sans clipping, au buffer voulu.
void d8_xor(int x,int y,void* spr,unsigned int segm)
{
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       mov ch,0x8
     }
  loop:
  asm{
       xor al,al
       mov ah,ds:[si]
       shr ax,cl
       xor es:[di],ax
       sub di,0x10
       inc si
       dec ch
       jne loop
     }
}


// Affiche un sprite 8*8 pour mode D3 ou DB avec
// OR avec clipping, au buffer voulu.
void d8clip_or(int x,int y,void* spr,unsigned int segm)
{
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       cmp ax,0
       jge clipping
       add ax,0x80
       add di,0x10
     }
  clipping:
  asm{
       shr ax,0x3
       sub di,ax
       or bx,0xFFFF
       cmp cx,0x78   // on commence les tests du clipping ici
       jle leftclip
       cmp cx,0x7F
       jg end
       xor bl,bl
     }
  leftclip:
  asm{
       cmp cx,0
       jge downclip
       cmp cx,-7
       jl end
       xor bh,bh
     }
  downclip:
  asm{
       and cl,0x7
       mov ch,0x8
       mov ax,y
       cmp ax,0x38
       jle upclip
       cmp ax,0x3F
       jg end
       shl ch,0x3
       sub ch,al
       jmp loop
     }
  upclip:
  asm{
       cmp ax,0
       jge loop
       cmp ax,-7
       jl end
       sub si,ax
       neg ax
       sub ch,al
       mov ax,0x1C
       add ax,segm
       mov es,ax   // fin des tests de clipping
     }
  loop:
  asm{
       xor al,al
       mov ah,ds:[si]
       shr ax,cl
       and ax,bx
       or es:[di],ax
       sub di,0x10
       inc si
       dec ch
       jne loop
     }
  end:
  asm nop;
}


// Affiche un sprite 8*8 pour mode D3 ou DB avec
// AND avec clipping, au buffer voulu.
void d8clip_and(int x,int y,void* spr,unsigned int segm)
{
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       cmp ax,0
       jge clipping
       add ax,0x80
       add di,0x10
     }
  clipping:
  asm{
       shr ax,0x3
       sub di,ax
       mov ax,y
       xor bx,bx
       cmp cx,0x78   // on commence les tests du clipping ici
       jle leftclip
       cmp cx,0x7F
       jg end
       or bl,0xFF
       jmp downclip
     }
  leftclip:
  asm{
       cmp cx,0
       jge downclip
       cmp cx,-7
       jl end
       or bh,0xFF
     }
  downclip:
  asm{
       and cl,0x7
       mov ch,0x8
       cmp ax,0x38
       jle upclip
       cmp ax,0x3F
       jg end
       shl ch,0x3
       sub ch,al
       jmp loop
     }
  upclip:
  asm{
       cmp ax,0
       jge loop
       cmp ax,-7
       jl end
       sub si,ax
       neg ax
       sub ch,al
       mov ax,0x1C
       add ax,segm
       mov es,ax   // fin des tests de clipping
     }
  loop:
  asm{
       or ax,0xFFFF
       and ah,ds:[si]
       ror ax,cl
       or ax,bx
       and es:[di],ax
       sub di,0x10
       inc si
       dec ch
       jne loop
     }
  end:
  asm nop;
}


// Affiche un sprite 8*8 pour mode D3 ou DB avec
// XOR avec clipping, au buffer voulu.
void d8clip_xor(int x,int y,void* spr,unsigned int segm)
{
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       cmp ax,0
       jge clipping
       add ax,0x80
       add di,0x10
     }
  clipping:
  asm{
       shr ax,0x3
       sub di,ax
       or bx,0xFFFF
       cmp cx,0x78   // on commence les tests du clipping ici
       jle leftclip
       cmp cx,0x7F
       jg end
       xor bl,bl
     }
  leftclip:
  asm{
       cmp cx,0
       jge downclip
       cmp cx,-7
       jl end
       xor bh,bh
     }
  downclip:
  asm{
       and cl,0x7
       mov ch,0x8
       mov ax,y
       cmp ax,0x38
       jle upclip
       cmp ax,0x3F
       jg end
       shl ch,0x3
       sub ch,al
       jmp loop
     }
  upclip:
  asm{
       cmp ax,0
       jge loop
       cmp ax,-7
       jl end
       sub si,ax
       neg ax
       sub ch,al
       mov ax,0x1C
       add ax,segm
       mov es,ax   // fin des tests de clipping
     }
  loop:
  asm{
       xor al,al
       mov ah,ds:[si]
       shr ax,cl
       and ax,bx
       xor es:[di],ax
       sub di,0x10
       inc si
       dec ch
       jne loop
     }
  end:
  asm nop;
}


/* ----------------------------------------
    Fonctions d'affichage de sprites 16*16
      uniquement pour les modes d3 et db
   ---------------------------------------- */


// Affiche un sprite 16*16 pour mode D3 ou DB
// avec OR sans clipping, au buffer voulu.
void d16_or(int x,int y,void* spr,unsigned int segm)
{
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       mov ch,0x10
     }
  loop:
  asm{
       xor dl,dl
       mov ax,ds:[si]
       mov dh,al
       shr ax,cl
       shr dx,cl
       or es:[di],ax
       or es:[di-0x1],dl
       sub di,0x10
       add si,0x2
       dec ch
       jne loop
     }
}


// Affiche un sprite 16*16 pour mode D3 ou DB
// avec AND sans clipping, au buffer voulu.
void d16_and(int x,int y,void* spr,unsigned int segm)
{
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       mov ch,0x10
     }
  loop:
  asm{
       or dl,0xFF
       mov ax,ds:[si]
       mov dh,al
       not ax
       shr ax,cl
       shr dx,cl
       not ax
       and es:[di],ax
       and es:[di-0x1],dl
       sub di,0x10
       add si,0x2
       dec ch
       jne loop
     }
}


// Affiche un sprite 16*16 pour mode D3 ou DB
// avec XOR sans clipping, au buffer voulu.
void d16_xor(int x,int y,void* spr,unsigned int segm)
{
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       mov ch,0x10
     }
  loop:
  asm{
       xor dl,dl
       mov ax,ds:[si]
       mov dh,al
       shr ax,cl
       shr dx,cl
       xor es:[di],ax
       xor es:[di-0x1],dl
       sub di,0x10
       add si,0x2
       dec ch
       jne loop
     }
}


// Affiche un sprite 16*16 pour mode D3 ou DB
// avec OR avec clipping, au buffer voulu.
void d16clip_or(int x,int y,void* spr,unsigned int segm)
{
  unsigned char el=0xFF;
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       cmp ax,0
       jge clipping
       add ax,0x80
       add di,0x10
     }
  clipping:
  asm{
       shr ax,0x3
       sub di,ax
       or bx,0xFFFF
       cmp cx,0x70   // on commence les tests du clipping ici
       jle leftclip
       cmp cx,0x7F
       jg end
       xor el,bl
       cmp cx,0x78
       jl downclip
       xor bl,bl
       jmp downclip
     }
  leftclip:
  asm{
       cmp cx,0
       jge downclip
       cmp cx,-15
       jl end
       xor bh,bh
       cmp cx,-8
       jge downclip
       xor bl,bl
     }
  downclip:
  asm{
       and cl,0x7
       mov ch,0x10
       mov ax,y
       cmp ax,0x30
       jle upclip
       cmp ax,0x3F
       jg end
       shl ch,0x2
       sub ch,al
       jmp loop
     }
  upclip:
  asm{
       cmp ax,0
       jge loop
       cmp ax,-15
       jl end
       mov dx,ax
       shl ax,0x1
       sub si,ax
       neg dx
       sub ch,dl
       mov ax,0x18
       add ax,segm
       mov es,ax   // fin des tests de clipping
     }
  loop:
  asm{
       xor dl,dl
       mov ax,ds:[si]
       mov dh,al
       shr ax,cl
       shr dx,cl
       and ax,bx
       and dl,el
       or es:[di],ax
       or es:[di-1],dl
       sub di,0x10
       add si,0x2
       dec ch
       jne loop
     }
  end:
  asm nop;
}


// Affiche un sprite 16*16 pour mode D3 ou DB
// avec AND avec clipping, au buffer voulu.
void d16clip_and(int x,int y,void* spr,unsigned int segm)
{
  unsigned char el=0;
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       cmp ax,0
       jge clipping
       add ax,0x80
       add di,0x10
     }
  clipping:
  asm{
       shr ax,0x3
       sub di,ax
       xor bx,bx
       cmp cx,0x70   // on commence les tests du clipping ici
       jle leftclip
       cmp cx,0x7F
       jg end
       or al,0xFF
       or el,al
       cmp cx,0x78
       jl downclip
       or bl,0xFF
       jmp downclip
     }
  leftclip:
  asm{
       cmp cx,0
       jge downclip
       cmp cx,-15
       jl end
       or bh,0xFF
       cmp cx,-8
       jge downclip
       or bl,0xFF
     }
  downclip:
  asm{
       and cl,0x7
       mov ch,0x10
       mov ax,y
       cmp ax,0x30
       jle upclip
       cmp ax,0x3F
       jg end
       shl ch,0x2
      sub ch,al
      jmp loop
    }
  upclip:
  asm{
       cmp ax,0
       jge loop
       cmp ax,-15
       jl end
       mov dx,ax
       shl ax,0x1
       sub si,ax
       neg dx
       sub ch,dl
       mov ax,0x18
       add ax,segm
       mov es,ax   // fin des tests de clipping
     }
  loop:
  asm{
       or dl,0xFF
       mov ax,ds:[si]
       mov dh,al
       not ax
       shr ax,cl
       shr dx,cl
       not ax
       or ax,bx
       or dl,el
       and es:[di],ax
       and es:[di-1],dl
       sub di,0x10
       add si,0x2
       dec ch
       jne loop
     }
  end:
  asm nop;
}


// Affiche un sprite 16*16 pour mode D3 ou DB
// avec XOR avec clipping, au buffer voulu.
void d16clip_xor(int x,int y,void* spr,unsigned int segm)
{
  unsigned char el=0xFF;
  asm{
       mov cx,x
       mov si,spr
       mov di,0x3FE
       mov ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       cmp ax,0
       jge clipping
       add ax,0x80
       add di,0x10
     }
  clipping:
  asm{
       shr ax,0x3
       sub di,ax
       or bx,0xFFFF
       cmp cx,0x70   // on commence les tests du clipping ici
       jle leftclip
       cmp cx,0x7F
       jg end
       xor el,bl
       cmp cx,0x78
       jl downclip
       xor bl,bl
       jmp downclip
     }
  leftclip:
  asm{
       cmp cx,0
       jge downclip
       cmp cx,-15
       jl end
       xor bh,bh
       cmp cx,-8
       jge downclip
       xor bl,bl
     }
  downclip:
  asm{
       and cl,0x7
       mov ch,0x10
       mov ax,y
       cmp ax,0x30
       jle upclip
       cmp ax,0x3F
       jg end
       shl ch,0x2
       sub ch,al
       jmp loop
     }
  upclip:
  asm{
       cmp ax,0
       jge loop
       cmp ax,-15
       jl end
       mov dx,ax
       shl ax,0x1
       sub si,ax
       neg dx
       sub ch,dl
       mov ax,0x18
       add ax,segm
       mov es,ax   // fin des tests de clipping
     }
  loop:
  asm{
       xor dl,dl
       mov ax,ds:[si]
       mov dh,al
       shr ax,cl
       shr dx,cl
       and ax,bx
       and dl,el
       xor es:[di],ax
       xor es:[di-1],dl
       sub di,0x10
       add si,0x2
       dec ch
       jne loop
     }
  end:
  asm nop;
}


/* ---------------------------------------
           Fonctions concernant la
             gestion d'une ligne
   --------------------------------------- */


// Affiche une ligne verticale noire en mode DB.
void bvline(unsigned int x,unsigned int ymin,unsigned int ymax,unsigned int segm)
{
  asm{
      mov cx,x
      mov dl,0x80
      mov di,0x1FF
      mov ax,0x20
      add ax,segm
      sub ax,ymax
      mov es,ax
      mov ax,cx
      shr ax,0x3
      sub di,ax
      and cl,0x7
      shr dl,cl
      mov cx,ymax
      sub cx,ymin
      inc cx
    }
  loop:
  asm{
       or es:[di],dl
       or es:[di+0x800],dl
       add di,0x10
       dec cx
       jne loop
     }
}


// Affiche une ligne verticale blanche en mode DB.
void wvline(unsigned int x,unsigned int ymin,unsigned int ymax,unsigned int segm)
{
  asm{
       mov cx,x
       mov dl,0x7F
       mov di,0x1FF
       mov ax,0x20
       add ax,segm
       sub ax,ymax
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       ror dl,cl
       mov cx,ymax
       sub cx,ymin
       inc cx
     }
  loop:
  asm{
       and es:[di],dl
       and es:[di+0x800],dl
       add di,0x10
       dec cx
       jne loop
     }
}


// Affiche une ligne verticale grise en mode DB.
void gvline(unsigned int x,unsigned int ymin,unsigned int ymax,unsigned int segm)
{
  asm{
       mov cx,x
       mov dx,0x807F
       mov di,0x1FF
       mov ax,0x20
       add ax,segm
       sub ax,ymax
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dh,cl
       ror dl,cl
       mov cx,ymax
       sub cx,ymin
       inc cx
     }
  loop:
  asm{
       or es:[di],dh
       and es:[di+0x800],dl
       add di,0x10
       dec cx
       jne loop
     }
}


// Inverse une ligne verticale en mode DB.
void invvline(unsigned int x,unsigned int ymin,unsigned int ymax,unsigned int segm)
{
  asm{
       mov cx,x
       mov dl,0x80
       mov di,0x1FF
       mov ax,0x20
       add ax,segm
       sub ax,ymax
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dl,cl
       mov cx,ymax
       sub cx,ymin
       inc cx
     }
  loop:
  asm{
       xor es:[di],dl
       xor es:[di+0x800],dl
       add di,0x10
       dec cx
       jne loop
     }
}


// Affiche une ligne verticale en mode DB avec la couleur choisie.
// Vitesse incomparable par rapport a une fonction du type :
// for (...) pxl(...);
void vline(unsigned int x,unsigned int ymin,unsigned int ymax,unsigned char couleur,unsigned int segm)
{
  switch (couleur)
  {
    case 2  : gvline(x,ymin,ymax,segm);break;
    case 1  : bvline(x,ymin,ymax,segm);break;
    case 0  : wvline(x,ymin,ymax,segm);break;
    default : invvline(x,ymin,ymax,segm);
  }
}


// Affiche une ligne horizontale noire en mode DB.
void bhline(unsigned int xmin,unsigned int xmax,unsigned int y,unsigned int segm)
{
  asm{
       mov cx,xmin
       or dx,0xFFFF
       mov di,0x1FF
       mov si,di
       mov ax,0x20
       add ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dh,cl
       mov cx,xmax
       mov ax,cx
       shr ax,0x3
       sub si,ax
       and cl,0x7
       inc cl
       shr dl,cl
       not dl
       mov cx,di
       sub cx,si
       je samebyte
       dec cx
       je sameword
       or es:[di],dh
       or es:[di+0x800],dh
       or es:[si],dl
       or es:[si+0x800],dl
       or al,0xFF
     }
  loop:
  asm{
       inc si
       or es:[si],al
       or es:[si+0x800],al
       dec cx
       jne loop
     }
  return;
  samebyte:
  asm{
       and dl,dh
       or es:[di],dl
       or es:[di+0x800],dl
     }
  return;
  sameword:
  asm{
       or es:[si],dx
       or es:[si+0x800],dx
     }
}


// Affiche une ligne horizontale blanche en mode DB.
void whline(unsigned int xmin,unsigned int xmax,unsigned int y,unsigned int segm)
{
  asm{
       mov cx,xmin
       or dx,0xFFFF
       mov di,0x1FF
       mov si,di
       mov ax,0x20
       add ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dh,cl
       not dh
       mov cx,xmax
       mov ax,cx
       shr ax,0x3
       sub si,ax
       and cl,0x7
       inc cl
       shr dl,cl
       mov cx,di
       sub cx,si
       je samebyte
       dec cx
       je sameword
       and es:[di],dh
       and es:[di+0x800],dh
       and es:[si],dl
       and es:[si+0x800],dl
       xor al,al
     }
  loop:
  asm{
       inc si
       and es:[si],al
       and es:[si+0x800],al
       dec cx
       jne loop
     }
  return;
  samebyte:
  asm{
       or dl,dh
       and es:[di],dl
       and es:[di+0x800],dl
     }
  return;
  sameword:
  asm{
       and es:[si],dx
       and es:[si+0x800],dx
     }
}


// Affiche une ligne horizontale grise en mode DB.
void ghline(unsigned int xmin,unsigned int xmax,unsigned int y,unsigned int segm)
{
  asm{
       mov cx,xmin
       or dh,0xFF
       or bl,0xFF
       mov di,0x1FF
       mov si,di
       mov ax,0x20
       add ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dh,cl
       mov bh,dh
       not bh
       mov cx,xmax
       mov ax,cx
       shr ax,0x3
       sub si,ax
       and cl,0x7
       inc cl
       shr bl,cl
       mov dl,bl
       not dl
       mov cx,di
       sub cx,si
       je samebyte
       dec cx
       je sameword
       or es:[di],dh
       and es:[di+0x800],bh
       or es:[si],dl
       and es:[si+0x800],bl
       mov ax,0xFF00
     }
  loop:
  asm{
       inc si
       or es:[si],ah
       and es:[si+0x800],al
       dec cx
       jne loop
     }
  return;
  samebyte:
  asm{
       and dl,dh
       or bl,bh
       or es:[di],dl
       and es:[di+0x800],bl
     }
  return;
  sameword:
  asm{
       or es:[si],dx
       and es:[si+0x800],bx
     }
}


// Inverse une ligne horizontale en mode DB.
void invhline(unsigned int xmin,unsigned int xmax,unsigned int y,unsigned int segm)
{
  asm{
       mov cx,xmin
       or dx,0xFFFF
       mov di,0x1FF
       mov si,di
       mov ax,0x20
       add ax,segm
       sub ax,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dh,cl
       mov cx,xmax
       mov ax,cx
       shr ax,0x3
       sub si,ax
       and cl,0x7
       inc cl
       shr dl,cl
       not dl
       mov cx,di
       sub cx,si
       je samebyte
       dec cx
       je sameword
       xor es:[di],dh
       xor es:[di+0x800],dh
       xor es:[si],dl
       xor es:[si+0x800],dl
       or al,0xFF
     }
  loop:
  asm{
       inc si
       xor es:[si],al
       xor es:[si+0x800],al
       dec cx
       jne loop
     }
  return;
  samebyte:
  asm{
       and dl,dh
       xor es:[di],dl
       xor es:[di+0x800],dl
     }
  return;
  sameword:
  asm{
       xor es:[si],dx
       xor es:[si+0x800],dx
     }
}


// Affiche une ligne horizontale en mode DB avec la couleur choisie.
// Vitesse incomparable par rapport a une fonction du type :
// for (...) pxl(...);
void hline(unsigned int xmin,unsigned int xmax,unsigned int y,unsigned char couleur,unsigned int segm)
{
  switch (couleur)
  {
    case 2  : ghline(xmin,xmax,y,segm);break;
    case 1  : bhline(xmin,xmax,y,segm);break;
    case 0  : whline(xmin,xmax,y,segm);break;
    default : invhline(xmin,xmax,y,segm);
  }
}


/* ---------------------------------------
       Fonctions concernant la gestion
               d'un seul pixel
   --------------------------------------- */


// Dessine un pixel noir en mode DB
void bpxl(unsigned int x,unsigned char y,unsigned int segm)
{
  asm{
       mov cx,x
       mov dl,0x80
       mov di,0x1FF
       mov ax,0x20
       add ax,segm
       sub al,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dl,cl
       or es:[di],dl
       or es:[di+0x800],dl
     }
}


// Dessine un pixel blanc en mode DB
void wpxl(unsigned int x,unsigned char y,unsigned int segm)
{
  asm{
       mov cx,x
       mov dl,0x7F
       mov di,0x1FF
       mov ax,0x20
       add ax,segm
       sub al,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       ror dl,cl
       and es:[di],dl
       and es:[di+0x800],dl
     }
}


// Dessine un pixel gris en mode DB
void gpxl(unsigned int x,unsigned char y,unsigned int segm)
{
  asm{
       mov cx,x
       mov dl,0x80
       mov di,0x1FF
       mov ax,0x20
       add ax,segm
       sub al,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dl,cl
       or es:[di],dl
       not dl
       and es:[di+0x800],dl
     }
}


// Inverse un pixel en mode DB
void invpxl(unsigned int x,unsigned char y,unsigned int segm)
{
  asm{
       mov cx,x
       mov dl,0x80
       mov di,0x1FF
       mov ax,0x20
       add ax,segm
       sub al,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dl,cl
       xor es:[di],dl
       xor es:[di+0x800],dl
     }
}


// Determine si le pixel est allume ou eteint
// en mode D3 ou DB, sur un seul buffer.
unsigned char pxltest(unsigned int x,unsigned char y,unsigned int segm)
{
  asm{
       mov cx,x
       mov dx,0x8080
       mov di,0x1FF
       mov ax,0x20
       add ax,segm
       sub al,y
       mov es,ax
       mov ax,cx
       shr ax,0x3
       sub di,ax
       and cl,0x7
       shr dx,cl
       and dl,es:[di]
       and dh,es:[di+0x800]
       cmp dx,0x0
       je end
       inc cl
       rol dx,cl
       add dl,dh
       xor dl,0x3
     }
  end:
  return _DL;
}


// Dessine un pixel en fonction de la couleur (ou inversion) choisie.
void pxl(unsigned int x,unsigned char y,unsigned char couleur,unsigned int segm)
{
  switch (couleur)
  {
    case 2  : gpxl(x,y,segm);break;
    case 1  : bpxl(x,y,segm);break;
    case 0  : wpxl(x,y,segm);break;
    default : invpxl(x,y,segm);
  }
}
