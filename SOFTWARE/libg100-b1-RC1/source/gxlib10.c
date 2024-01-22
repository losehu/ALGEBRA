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

 /////////////////////////////////////////////////////////////////////////////////////
//*                          <gxLib pour CASIO Graph100>                            *//
//                                                                                   //
//                       Fichier à inclure dans votre projet                         //
//                                                                                   //
//*  Version 1.0                                           (c) Julien PATTE - 2004  *//
 /////////////////////////////////////////////////////////////////////////////////////

#include "gxlib10.h"

static uint gxSegVideo;
static void interrupt (*gxOldVideoInt)();

///////////////////////////////////////////////////////////////////////////////////////

void interrupt gxNewVideoInt()
{
asm {
	//push ax
	mov ax,gxSegVideo
	xor gxSegVideo,0x800
	add ax,0xA200
	out 05h,ax
	mov al,1
	out 07h,al
	//pop ax
	}

	(*gxOldVideoInt)();
}

///////////////////////////////////////////////////////////////////////////////////////

void gxSetMode(bool enabled)
{
	static bool state = false, firstTime=true;
	static uchar oldCont;
	static int numVideoInt;

asm {
	mov al,firstTime
	or al,al
	jz deja_fait
					// if(firstTime)
	mov ax,0x0040
	mov es,ax
	mov al,es:0x00E5
	sub al,0x12
	shr al,1
	mov oldCont,al	// oldCont = ((*((uchar far*)0x004000E5L))-0x12)>>1;

	mov ax,0xC000
	mov es,ax
	mov al,es:0x000C
	mov bx,0x2
	cmp al,0x41
	je getnumvideo1
	mov bx,0x53

LBL(getnumvideo1)
	
	mov numVideoInt,bx	// numVideoInt = ( (*((uchar far*)0xC000000CL)==0x41) ? 0x2 : 0x53 );

	mov firstTime,0		// firstTime=false;

LBL(deja_fait)

	mov bl,enabled
	cmp bl,state
	je quit			// if(state==enabled) return;

	xor ax,ax
	mov es,ax
	mov di,numVideoInt
	shl di,2		// es:di -> adresse de l'int video
	
	or bl,bl
	jz mode_off
					// if(enabled)

		// copie l'adresse de l'ancienne int video dans le pointeur
	mov ax,es:[di]
	mov word ptr gxOldVideoInt,ax
	mov ax,es:[di+2]
	mov word ptr gxOldVideoInt+2,ax

		// copie l'adresse de la nouvelle int dans le vecteur
	mov ax,offset gxNewVideoInt
	mov es:[di],ax
	mov ax,seg gxNewVideoInt
	mov es:[di+2],ax

		// passe en mode DB
	mov al,2
	out 4,al
	mov al,0xDB	
	out 2,al

		// efface l'ecran et les pages intermediaires
	mov ax,0x1A20
	mov es,ax
	xor di,di
	cld
	xor ax,ax
	mov cx,3072	// (6*512) 6 pages à effacer: 0x1A20 -> 0x1B60
    rep stosw

		// definit le contraste a appliquer
	mov cl,GX_CONT_G100
	mov ax,0xC000
	mov es,ax
	mov al,0x41
	cmp es:0xC,al
	jne fin
	mov cl,GX_CONT_G100PLUS
	jmp fin

LBL(mode_off)

		// restaure l'ancienne int
	mov ax,word ptr gxOldVideoInt
	mov es:[di],ax
	mov ax,word ptr gxOldVideoInt+2
	mov es:[di+2],ax

		// repasse en mode C3
	mov al,4
	out 4,al
	mov al,0xC3
	out 2,al
	
		// restaure et efface la première page
	mov ax,0x1A20
	mov es,ax
	xor di,di
	cld
	xor ax,ax
	mov cx,512
	rep stosw
	mov ax,0xA200
	out 05h,ax
	mov al,1
	out 07h,al

	mov cl,oldCont

LBL(fin)

		// definit le nouveau contraste
	mov ax,0x40
	mov es,ax
	mov al,cl
	shl al,0x1
	add ax,0x2210
	mov es:0xE5,al
	xor bl,bl
	int 0x7C

	mov al,enabled
	mov state,al

LBL(quit)
	}
}

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_REFRESH
void gxRefresh()
{
asm {
	push ds
	les di,gxScreen1
	lds si,gxBuffer1
	cld
	mov cx,512
	rep movsw
	add di,1024
	add si,1024
	mov cx,512
	rep movsw
	add di,1024
	add si,1024
	mov cx,512
	rep movsw
	pop ds
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_DRAWBMP
void gxDrawBmp(void far* bmp)
{
asm {
	push ds
	les di,gxBuffer1
	lds si,bmp
	cld
	mov cx,512
	rep movsw
	add di,1024
	mov cx,512
	rep movsw
	add di,1024
	mov cx,512
	rep movsw
	pop ds
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_PIXEL
void gxPixel(uint x,uint y, uchar coul)	// affiche un pixel
{
	switch(coul)
	{
#ifdef FUNC_GX_PIXW
		case GX_WHITE:		gxPixW(x,y);	break; 
#endif
#ifdef FUNC_GX_PIXLG
		case GX_LIGHTGRAY:	gxPixLG(x,y);	break; 
#endif
#ifdef FUNC_GX_PIXMG
		case GX_MIDGRAY:	gxPixMG(x,y);	break; 
#endif		
#ifdef FUNC_GX_PIXDG
		case GX_DARKGRAY:	gxPixDG(x,y);	break; 
#endif
#ifdef FUNC_GX_PIXB
		case GX_BLACK:		gxPixB(x,y);	break; 
#endif
#ifdef FUNC_GX_PIXINV
		case GX_INVERSE:	gxPixInv(x,y);	break; 
#endif
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_PIXW
void gxPixW(uint x,uint y)	// affiche un pixel en blanc
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov dx,y
	shl dx,4
	sub di,dx		// es:[di+bx] -> gauche de la bonne ligne 

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax		// es:[di+bx] -> bon octet
	and cx,7
	mov ch,128
	shr ch,cl
	not ch
	
	and es:[di],ch
	and es:[di+0x800],ch
	and es:[di+0x1000],ch
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_PIXLG
void gxPixLG(uint x,uint y)	// affiche un pixel en gris clair
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov dx,y
	mov cx,dx
	shl cx,4
	sub di,cx			// es:[di+bx] -> gauche de la bonne ligne 

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax			// es:[di+bx] -> bon octet
	and cx,7
	mov ch,128
	shr ch,cl			// ch = octet avec bit a allumer
	mov dh,ch
	not dh				// dh = octet avec bit a eteindre

	add cl,dl
	shr cl,1
	jc inversion
						// on affiche la combinaison 001
	and es:[di],dh
	and es:[di+0x800],dh
	or es:[di+0x1000],ch

	jmp fin

LBL(inversion)
						// on affiche la combinaison 100
	or es:[di],ch
	and es:[di+0x800],dh
	and es:[di+0x1000],dh

LBL(fin)
	}

}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_PIXMG
void gxPixMG(uint x,uint y)	// affiche un pixel en gris moyen
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov dx,y
	mov cx,dx
	shl cx,4
	sub di,cx			// es:[di+bx] -> gauche de la bonne ligne 

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax			// es:[di+bx] -> bon octet
	and cx,7
	mov ch,128
	shr ch,cl			// ch = octet avec bit a allumer
	mov dh,ch
	not dh				// dh = octet avec bit a eteindre

	add cl,dl
	shr cl,1
	jc inversion
						// on affiche la combinaison 010
	and es:[di],dh
	or es:[di+0x800],ch
	and es:[di+0x1000],dh

	jmp fin

LBL(inversion)
						// on affiche la combinaison 101
	or es:[di],ch
	and es:[di+0x800],dh
	or es:[di+0x1000],ch

LBL(fin)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_PIXDG
void gxPixDG(uint x,uint y)	// affiche un pixel en gris fonce
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov dx,y
	mov cx,dx
	shl cx,4
	sub di,cx			// es:[di+bx] -> gauche de la bonne ligne 

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax			// es:[di+bx] -> bon octet
	and cx,7
	mov ch,128
	shr ch,cl			// ch = octet avec bit a allumer
	mov dh,ch
	not dh				// dh = octet avec bit a eteindre

	add cl,dl
	shr cl,1
	jc inversion
						// on affiche la combinaison 011
	and es:[di],dh
	or es:[di+0x800],ch
	or es:[di+0x1000],ch

	jmp fin

LBL(inversion)
						// on affiche la combinaison 110
	or es:[di],ch
	or es:[di+0x800],ch
	and es:[di+0x1000],dh

LBL(fin)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_PIXB
void gxPixB(uint x, uint y)	// affiche un pixel en noir
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov dx,y
	shl dx,4
	sub di,dx		// es:[di+bx] -> gauche de la bonne ligne 

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax		// es:[di+bx] -> bon octet
	and cx,7
	mov ch,128
	shr ch,cl
	
	or es:[di],ch
	or es:[di+0x800],ch
	or es:[di+0x1000],ch
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_PIXINV
void gxPixInv(uint x,uint y)	// inverse un pixel
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov dx,y
	shl dx,4
	sub di,dx		// es:[di+bx] -> gauche de la bonne ligne 

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax		// es:[di+bx] -> bon octet
	and cx,7
	mov ch,128
	shr ch,cl
	
	xor es:[di],ch
	xor es:[di+0x800],ch
	xor es:[di+0x1000],ch
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_PIXTEST
uchar gxPixTest(uint x,uint y)
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov dx,y
	shl dx,4
	sub di,dx		// es:[di+bx] -> gauche de la bonne ligne 

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax		// es:[di+bx] -> bon octet
	and cx,7
	mov ch,128
	shr ch,cl
	
	sub cl,6
	neg cl			// cl = 6-(x%8)
	
	mov al,es:[di+0x800]
	and al,ch
	shr al,cl		// al = 2* valeur du pix sur le 2eme buffer

	inc cl
	mov dl,es:[di]
	and dl,ch
	shr dl,cl		// dl = valeur du pix sur le 1er buffer
	add al,dl

	mov dl,es:[di+0x1000]
	and dl,ch
	shr dl,cl		// dl = valeur du pix sur le 3eme buffer
	add al,dl
	}

	return _AL;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_VLINE
void gxVLine(uint x, uint ymin, uint ymax, uchar coul)
{
	switch(coul)
	{
#ifdef FUNC_GX_VLINEW
		case GX_WHITE:		gxVLineW(x,ymin,ymax);		break; 
#endif
#ifdef FUNC_GX_VLINELG
		case GX_LIGHTGRAY:	gxVLineLG(x,ymin,ymax);		break; 
#endif
#ifdef FUNC_GX_VLINEMG
		case GX_MIDGRAY:	gxVLineMG(x,ymin,ymax);		break; 
#endif		
#ifdef FUNC_GX_VLINEDG
		case GX_DARKGRAY:	gxVLineDG(x,ymin,ymax);		break; 
#endif
#ifdef FUNC_GX_VLINEB
		case GX_BLACK:		gxVLineB(x,ymin,ymax);		break; 
#endif
#ifdef FUNC_GX_VLINEINV
		case GX_INVERSE:	gxVLineInv(x,ymin,ymax);	break; 
#endif
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_VLINEW
void gxVLineW(uint x, uint ymin, uint ymax)	// affiche une ligne verticale en blanc
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov cx,ymax
	mov dx,cx
	shl cx,4
	sub di,cx

	sub dx,ymin
	inc dx			// dx = nbre de lignes a afficher

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax
	and cx,7
	mov al,128
	shr al,cl
	not al

	mov cx,dx

LBL(loopline)

	and es:[di],al
	and es:[di+0x800],al
	and es:[di+0x1000],al
	add di,16
	loop loopline
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_VLINELG
void gxVLineLG(uint x, uint ymin, uint ymax)	// affiche une ligne verticale en gris clair
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov cx,ymax
	mov dx,cx
	shl cx,4
	sub di,cx

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax
	and cx,7

	mov si,dx
	add si,cx
	and si,1

	mov al,128
	shr al,cl
	mov ah,al		// al = afficher
	not ah			// ah = effacer

	sub dx,ymin
	inc dx			// dx = nbre de lignes a afficher

	mov cx,dx

LBL(loopline)

	xor si,1
	jnz inversion
					// combinaison 001	
	and es:[di],ah
	and es:[di+0x800],ah
	or es:[di+0x1000],al
	jmp suite

LBL(inversion)
					// combinaison 100
	or es:[di],al
	and es:[di+0x800],ah
	and es:[di+0x1000],ah

LBL(suite)

	add di,16
	loop loopline
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_VLINEMG
void gxVLineMG(uint x, uint ymin, uint ymax)	// affiche une ligne verticale en gris moyen
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov cx,ymax
	mov dx,cx
	shl cx,4
	sub di,cx

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax
	and cx,7

	mov si,dx
	add si,cx
	and si,1

	mov al,128
	shr al,cl
	mov ah,al		// al = afficher
	not ah			// ah = effacer

	sub dx,ymin
	inc dx			// dx = nbre de lignes a afficher

	mov cx,dx

LBL(loopline)

	xor si,1
	jnz inversion
					// combinaison 010	
	and es:[di],ah
	or es:[di+0x800],al
	and es:[di+0x1000],ah
	jmp suite

LBL(inversion)
					// combinaison 101
	or es:[di],al
	and es:[di+0x800],ah
	or es:[di+0x1000],al

LBL(suite)

	add di,16
	loop loopline
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_VLINEDG
void gxVLineDG(uint x, uint ymin, uint ymax)	// affiche une ligne verticale en gris fonce
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov cx,ymax
	mov dx,cx
	shl cx,4
	sub di,cx

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax
	and cx,7

	mov si,dx
	add si,cx
	and si,1

	mov al,128
	shr al,cl
	mov ah,al		// al = afficher
	not ah			// ah = effacer

	sub dx,ymin
	inc dx			// dx = nbre de lignes a afficher

	mov cx,dx

LBL(loopline)

	xor si,1
	jnz inversion
					// combinaison 011	
	and es:[di],ah
	or es:[di+0x800],al
	or es:[di+0x1000],al
	jmp suite

LBL(inversion)
					// combinaison 110
	or es:[di],al
	or es:[di+0x800],al
	and es:[di+0x1000],ah

LBL(suite)

	add di,16
	loop loopline
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_VLINEB
void gxVLineB(uint x, uint ymin, uint ymax)	// affiche une ligne verticale en noir
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov cx,ymax
	mov dx,cx
	shl cx,4
	sub di,cx

	sub dx,ymin
	inc dx			// dx = nbre de lignes a afficher

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax
	and cx,7
	mov al,128
	shr al,cl

	mov cx,dx

LBL(loopline)

	or es:[di],al
	or es:[di+0x800],al
	or es:[di+0x1000],al
	add di,16
	loop loopline
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_VLINEINV
void gxVLineInv(uint x, uint ymin, uint ymax)
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,1023
	mov cx,ymax
	mov dx,cx
	shl cx,4
	sub di,cx

	sub dx,ymin
	inc dx			// dx = nbre de lignes a afficher

	mov cx,x
	mov ax,cx
	shr ax,3
	sub di,ax
	and cx,7
	mov al,128
	shr al,cl

	mov cx,dx

LBL(loopline)

	xor es:[di],al
	xor es:[di+0x800],al
	xor es:[di+0x1000],al
	add di,16
	loop loopline
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_HLINE
void gxHLine(uint xmin, uint xmax, uint y, uchar coul)
{
	switch(coul)
	{
#ifdef FUNC_GX_HLINEW
		case GX_WHITE:		gxHLineW(xmin,xmax,y);		break; 
#endif
#ifdef FUNC_GX_HLINELG
		case GX_LIGHTGRAY:	gxHLineLG(xmin,xmax,y);		break; 
#endif
#ifdef FUNC_GX_HLINEMG
		case GX_MIDGRAY:	gxHLineMG(xmin,xmax,y);		break; 
#endif		
#ifdef FUNC_GX_HLINEDG
		case GX_DARKGRAY:	gxHLineDG(xmin,xmax,y);		break; 
#endif
#ifdef FUNC_GX_HLINEB
		case GX_BLACK:		gxHLineB(xmin,xmax,y);		break; 
#endif
#ifdef FUNC_GX_HLINEINV
		case GX_INVERSE:	gxHLineInv(xmin,xmax,y);	break; 
#endif
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_HLINEW
void gxHLineW(uint xmin, uint xmax, uint y)
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,0x3FF
	mov ax,y
	shl ax,4
	sub di,ax
	mov si,di
	
	or dx,0xFFFF

	mov cx,xmin
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

	not dh

	mov cx,di
	sub cx,si
	je samebyte		// bx et si pointent vers le meme octet?
	dec cx
	je sameword		// si pointe vers l'octet suivant?

	and es:[di],dh
	and es:[di+0x800],dh
	and es:[di+0x1000],dh

	and es:[si],dl
	and es:[si+0x800],dl
	and es:[si+0x1000],dl

	xor al,al

LBL(milieu)

	inc si
	and es:[si],al
	and es:[si+0x800],al
	and es:[si+0x1000],al
	loop milieu
	jmp fin

LBL(samebyte)

	or dl,dh
	and es:[di],dl
	and es:[di+0x800],dl
	and es:[di+0x1000],dl
	jmp fin

LBL(sameword)

	and es:[si],dx
	and es:[si+0x800],dx
	and es:[si+0x1000],dx

LBL(fin)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_HLINELG
void gxHLineLG(uint xmin, uint xmax, uint y)
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,0x3FF
	mov ax,y
	shl ax,4
	sub di,ax
	mov si,di
	
	or dx,0xFFFF

	mov cx,xmin
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

	not dh

	mov ax,di
	shr ax,4
	mov ah,0xAA
	and al,1
	jz affich
	ror ah,1

LBL(affich)

	mov cx,di
	sub cx,si
	je samebyte		// bx et si pointent vers le meme octet?
	dec cx
	je sameword		// si pointe vers l'octet suivant?

	and es:[di],dh			// 1
	and es:[di+0x800],dh	// 2
	and es:[di+0x1000],dh	// 3

	mov al,ah				// ah = 01010101 ou 10101010
	ror al,1				// al = 10101010 ou 01010101

	not dh
	mov ch,ah				
	and ch,dh
	or es:[di],ch			// 1

	mov ch,al
	and ch,dh
	or es:[di+0x1000],ch	// 3

	and es:[si],dl			// 1
	and es:[si+0x800],dl	// 2
	and es:[si+0x1000],dl	// 3

	not dl
	mov ch,ah
	and ch,dl
	or es:[si],ch			// 1

	mov ch,al
	and ch,dl
	or es:[si+0x1000],ch	// 3

	xor ch,ch

LBL(milieu)

	inc si
	mov es:[si],al			// 1
	and es:[si+0x800],ch	// 2
	mov es:[si+0x1000],ah	// 3

	loop milieu
	jmp fin

LBL(samebyte)

	or dl,dh
	and es:[di],dl			// 1
	and es:[di+0x800],dl	// 2
	and es:[di+0x1000],dl	// 3

	mov al,ah
	not dl
	and ah,dl
	or es:[di],ah			// 1
	ror al,1
	and al,dl
	or es:[di+0x1000],al	// 3
	jmp fin

LBL(sameword)

	and es:[si],dx			// 1
	and es:[si+0x800],dx	// 2
	and es:[si+0x1000],dx	// 3

	mov al,ah
	not dx
	mov cx,ax
	and ax,dx
	or es:[si],ax			// 1
	ror cx,1
	and cx,dx
	or es:[si+0x1000],cx	// 3

LBL(fin)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_HLINEMG
void gxHLineMG(uint xmin, uint xmax, uint y)
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,0x3FF
	mov ax,y
	shl ax,4
	sub di,ax
	mov si,di
	
	or dx,0xFFFF

	mov cx,xmin
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

	not dh

	mov ax,di
	shr ax,4
	mov ah,0xAA
	and al,1
	jz affich
	ror ah,1

LBL(affich)

	mov cx,di
	sub cx,si
	je samebyte		// bx et si pointent vers le meme octet?
	dec cx
	je sameword		// si pointe vers l'octet suivant?

	and es:[di],dh			// 1
	and es:[di+0x800],dh	// 2
	and es:[di+0x1000],dh	// 3

	mov al,ah				// ah = 01010101 ou 10101010
	ror al,1				// al = 10101010 ou 01010101

	not dh
	mov ch,ah				
	and ch,dh
	or es:[di],ch			// 1
	or es:[di+0x1000],ch	// 3

	mov ch,al
	and ch,dh
	or es:[di+0x800],ch		// 2

	and es:[si],dl			// 1
	and es:[si+0x800],dl	// 2
	and es:[si+0x1000],dl	// 3

	not dl
	mov ch,ah
	and ch,dl
	or es:[si],ch			// 1
	or es:[si+0x1000],ch	// 3

	mov ch,al
	and ch,dl
	or es:[si+0x800],ch		// 2

	xor ch,ch

LBL(milieu)

	inc si
	mov es:[si],ah			// 1
	mov es:[si+0x800],al	// 2
	mov es:[si+0x1000],ah	// 3

	loop milieu
	jmp fin

LBL(samebyte)

	or dl,dh
	and es:[di],dl			// 1
	and es:[di+0x800],dl	// 2
	and es:[di+0x1000],dl	// 3

	mov al,ah
	not dl
	and ah,dl
	or es:[di],ah			// 1
	or es:[di+0x1000],ah	// 3
	ror al,1
	and al,dl
	or es:[di+0x800],al		// 2
	jmp fin

LBL(sameword)

	and es:[si],dx			// 1
	and es:[si+0x800],dx	// 2
	and es:[si+0x1000],dx	// 3

	mov al,ah
	not dx
	mov cx,ax
	and ax,dx
	or es:[si],ax			// 1
	or es:[si+0x1000],ax	// 3
	ror cx,1
	and cx,dx
	or es:[si+0x800],cx		// 2

LBL(fin)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_HLINEDG
void gxHLineDG(uint xmin, uint xmax, uint y)
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,0x3FF
	mov ax,y
	shl ax,4
	sub di,ax
	mov si,di
	
	or dx,0xFFFF

	mov cx,xmin
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

	not dh

	mov ax,di
	shr ax,4
	mov ah,0xAA
	and al,1
	jz affich
	ror ah,1

LBL(affich)

	mov cx,di
	sub cx,si
	je samebyte		// bx et si pointent vers le meme octet?
	dec cx
	je sameword		// si pointe vers l'octet suivant?

	and es:[di],dh			// 1
	and es:[di+0x1000],dh	// 3

	mov al,ah				// ah = 01010101 ou 10101010
	ror al,1				// al = 10101010 ou 01010101

	not dh
	or es:[di+0x800],dh		// 2
	mov ch,ah				
	and ch,dh
	or es:[di],ch			// 1

	mov ch,al
	and ch,dh
	or es:[di+0x1000],ch	// 3

	and es:[si],dl			// 1
	and es:[si+0x1000],dl	// 3

	not dl
	or es:[si+0x800],dl		// 2
	mov ch,ah
	and ch,dl
	or es:[si],ch			// 1

	mov ch,al
	and ch,dl
	or es:[si+0x1000],ch	// 3

	xor ch,ch
	or dl,0xFF

LBL(milieu)

	inc si
	mov es:[si],al			// 1
	or es:[si+0x800],dl		// 2
	mov es:[si+0x1000],ah	// 3

	loop milieu
	jmp fin

LBL(samebyte)

	or dl,dh
	and es:[di],dl			// 1
	and es:[di+0x1000],dl	// 3

	mov al,ah
	not dl
	or es:[di+0x800],dl		// 2
	and ah,dl
	or es:[di],ah			// 1
	ror al,1
	and al,dl
	or es:[di+0x1000],al	// 3
	jmp fin

LBL(sameword)

	and es:[si],dx			// 1
	and es:[si+0x1000],dx	// 3

	mov al,ah
	not dx
	or es:[si+0x800],dx		// 2
	mov cx,ax
	and ax,dx
	or es:[si],ax			// 1
	ror cx,1
	and cx,dx
	or es:[si+0x1000],cx	// 3

LBL(fin)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_HLINEB
void gxHLineB(uint xmin, uint xmax, uint y)
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,0x3FF
	mov ax,y
	shl ax,4
	sub di,ax
	mov si,di
	
	or dx,0xFFFF

	mov cx,xmin
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
	je samebyte		// bx et si pointent vers le meme octet?
	dec cx
	je sameword		// si pointe vers l'octet suivant?

	or es:[di],dh
	or es:[di+0x800],dh
	or es:[di+0x1000],dh

	or es:[si],dl
	or es:[si+0x800],dl
	or es:[si+0x1000],dl

	or al,0xFF

LBL(milieu)

	inc si
	or es:[si],al
	or es:[si+0x800],al
	or es:[si+0x1000],al
	loop milieu
	jmp fin

LBL(samebyte)

	and dl,dh
	or es:[di],dl
	or es:[di+0x800],dl
	or es:[di+0x1000],dl
	jmp fin

LBL(sameword)

	or es:[si],dx
	or es:[si+0x800],dx
	or es:[si+0x1000],dx

LBL(fin)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_HLINEINV
void gxHLineInv(uint xmin, uint xmax, uint y)
{
asm{
	mov ax,0x1A60
	mov es,ax
	mov di,0x3FF
	mov ax,y
	shl ax,4
	sub di,ax
	mov si,di
	
	or dx,0xFFFF

	mov cx,xmin
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
	je samebyte		// bx et si pointent vers le meme octet?
	dec cx
	je sameword		// si pointe vers l'octet suivant?

	xor es:[di],dh
	xor es:[di+0x800],dh
	xor es:[di+0x1000],dh

	xor es:[si],dl
	xor es:[si+0x800],dl
	xor es:[si+0x1000],dl

	or al,0xFF

LBL(milieu)

	inc si
	xor es:[si],al
	xor es:[si+0x800],al
	xor es:[si+0x1000],al
	loop milieu
	jmp fin

LBL(samebyte)

	and dl,dh
	xor es:[di],dl
	xor es:[di+0x800],dl
	xor es:[di+0x1000],dl
	jmp fin

LBL(sameword)

	xor es:[si],dx
	xor es:[si+0x800],dx
	xor es:[si+0x1000],dx

LBL(fin)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_FILL
void gxFill(uchar coul)
{
	switch(coul)
	{
#ifdef FUNC_GX_FILLW
		case GX_WHITE:		gxFillW();		break; 
#endif
#ifdef FUNC_GX_FILLLG
		case GX_LIGHTGRAY:	gxFillLG();		break; 
#endif
#ifdef FUNC_GX_FILLMG
		case GX_MIDGRAY:	gxFillMG();		break; 
#endif		
#ifdef FUNC_GX_FILLDG
		case GX_DARKGRAY:	gxFillDG();		break; 
#endif
#ifdef FUNC_GX_FILLB
		case GX_BLACK:		gxFillB();		break; 
#endif
#ifdef FUNC_GX_INVERSE
		case GX_INVERSE:	gxInverse();	break; 
#endif
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_FILLW
void gxFillW()
{
asm {
	les di,gxBuffer1
	xor ax,ax
	cld
	mov cx,512
	rep stosw
	add di,1024
	mov cx,512
	rep stosw
	add di,1024
	mov cx,512
	rep stosw
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_FILLLG
void gxFillLG()
{
asm {
	cld
	les di,gxBuffer1
	mov bl,64			// 64 lignes
	mov ax,0xAAAA		// 0b1010101010101010

LBL(buffer1) 

	mov cx,8
	rep stosw
	ror ax,1
	dec bl
	jnz buffer1

LBL(buffer2) 

	xor ax,ax
	add di,1024
	mov cx,512
	rep stosw

	mov bl,64			// 64 lignes
	mov ax,0x5555		// 0b0101010101010101
	add di,1024

LBL(buffer3) 

	mov cx,8
	rep stosw
	ror ax,1
	dec bl
	jnz buffer3
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_FILLMG
void gxFillMG()
{
asm {
	les di,gxBuffer1
	mov bl,64			// 64 lignes
	mov ax,0xAAAA		// 0b1010101010101010
	mov dx,0x5555		// 0b0101010101010101

LBL(buffer1_3) 

	mov cx,8

LBL(buffer1_3loop) 

	mov es:[di],ax
	mov es:[di+2048],dx
	mov es:[di+4096],ax
	add di,2
	loop buffer1_3loop

	ror ax,1
	ror dx,1
	dec bl
	jnz buffer1_3
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_FILLDG
void gxFillDG()
{
asm {
	les di,gxBuffer1
	cld
	mov bl,64			// 64 lignes
	mov ax,0xAAAA		// 0b1010101010101010

LBL(buffer1) 

	mov cx,8
	rep stosw
	ror ax,1
	dec bl
	jnz buffer1

LBL(buffer2) 

	or ax,0xFFFF
	add di,1024
	mov cx,512
	rep stosw

	mov bl,64			// 64 lignes
	mov ax,0x5555		// 0b0101010101010101
	add di,1024

LBL(buffer3) 

	mov cx,8
	rep stosw
	ror ax,1
	dec bl
	jnz buffer3
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_FILLB
void gxFillB()
{
asm {
	les di,gxBuffer1
	or ax,0xFFFF
	cld
	mov cx,512
	rep stosw
	add di,1024
	mov cx,512
	rep stosw
	add di,1024
	mov cx,512
	rep stosw
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_INVERSE
void gxInverse()
{
asm {
	les di,gxBuffer1
	mov cx,512
	or ax,0xFFFF

LBL(buffer1)

	xor es:[di],ax
	xor es:[di+0x800],ax
	xor es:[di+0x1000],ax
	dec cx
	jnz buffer1
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

// SURTOUT NE PAS MODIFIER CES ADRESSES!!
const void far* gxScreen1 = (void far*) 0x1A200000L;
const void far* gxScreen2 = (void far*) 0x1AA00000L;
const void far* gxScreen3 = (void far*) 0x1B200000L;
const void far* gxBuffer1 = (void far*) 0x1A600000L;
const void far* gxBuffer2 = (void far*) 0x1AE00000L;
const void far* gxBuffer3 = (void far*) 0x1B600000L;

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_SPR_M_C
void gxSpr_M_C(int x,int y,void far* spr)
{
		// decalage de la position propre du sprite par rapport à (x,y)
	x -= *(((signed char far*)spr)++);
	y -= *(((signed char far*)spr)++);

asm { push ds

	mov ax,0x1A60
	mov es,ax
	lds si,spr			// ds = segment du pointeur spr, si = offset
						// si pointe sur la hauteur du sprite
	cld					// si sera incremente
	lodsb				// al = hauteur du sprite
	
	xor ah,ah
	push ax				// hauteur du sprite dans la pile
						// si pointe vers la largeur du bloc
	push 1

LBL(aspr_debut)
	
	lds si,spr			// ds:si pointe vers le debut du sprite
	mov bx,sp
	mov dx,ss:[bx]		// recupere l'emplacement de debut du bloc suivant 
	add si,dx
	inc dx
							// si pointe vers la largeur du bloc a afficher
	lodsb					// al = largeur (restante) du sprite
	cmp al,0
	je aspr_byebye
	
	mov di,1023
	
LBL(aspr_deb2)
	
	mov bx,sp
	mov bx,ss:[bx+2]	// bx recupere la hauteur du sprite dans la pile
	mov cl,bl			// cl contient lui aussi la hauteur
	xor ch,ch
	shl bx,1
	add bx,cx			// bx = 3* hauteur
	cmp al,8
	jng aspr_deb1
	shl bx,1			// bx contient la taille du bloc (sans la largeur)
	mov al,16
	
LBL(aspr_deb1)

	mov ch,cl
	add dx,bx			// dx contient l'emplacement de debut du bloc suivant
	mov bx,sp
	mov ss:[bx],dx		// on sauvegarde cette donnee pour le prochain saut vers spr_debut

	mov dx,y
	add cl,dl			// cl = y+h
						// pret pour la verif du clip haut
	cmp dl,GX_CLIPH
	jl aspr_c1
						// le sprite ne dépasse pas en haut
						// si est ok, ch est bon pour le clip haut
	shl dx,4
	sub di,dx			// di ok
	cmp cl,GX_CLIPB
	jnl aspr_c3b

	jmp aspr_c3c		// acces direct vers le clip horz si pas de clip vertic

LBL(aspr_byebye)	// on se barre

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

	jmp quit

LBL(aspr_c1)			// le sprite dépasse en haut 

	sub dl,GX_CLIPH
	neg dl				// nbre de lignes a ne pas afficher
	sub ch,dl			// ch ok
	mov bx,dx
	shl dx,1
	add dx,bx			// dx = 3*dx
	cmp al,8
	jng aspr_c1a
	shl dx,1

LBL(aspr_c1a)

	add si,dx			// si ok
	mov dx,GX_CLIPH
	shl dx,4
	sub di,dx			// di ok
						// pret pour la verif du clip bas
	cmp cl,GX_CLIPB
	jl aspr_c3

LBL(aspr_c3b)		// le sprite dépasse en bas

	sub cl,GX_CLIPB
	sub ch,cl			// on retire les lignes qui dépassent

LBL(aspr_c3)

	cmp ch,0
	jng aspr_byebye		// plus de lignes à afficher => bye bye

						// di -> octet de gauche de la ligne ou afficher
						// si -> premier octet de la premiere ligne du bloc a afficher
						// ch = nbre de lignes a afficher
						// pret pour la verif du clipping horiz
						// al = largeur reelle majoree à 16
LBL(aspr_c3c)

	mov bx,x
	cmp al,8
	jg aspr_16clip
	jmp aspr_8clip

LBL(aspr_byebye2)	// on se barre

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

	jmp quit

LBL(aspr_suiv)

	add x,16				// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp aspr_debut			// on retourne a la case depart

LBL(aspr_16clip)	// on va s'occuper des morceaux >= 16 pix

	// objectifs:
	//	- creer un masque sur 2 octets
	//	- positionner di
	//  - mettre la bonne valeur de décalage dans cl (x%8)
	// pas touche à ch!

	mov dx,0xFFFF		// masque
	mov ax,bx
	add ax,16			// ax = x+16
	cmp bx,GX_CLIPG
	jnl aspr_c4
						// si le sprite dépasse à gauche
	mov dx,bx
	sub dx,GX_CLIPG
	cmp dx,-15
	jl aspr_suiv		// on est tout-a-fait a gauche
	neg dx
	mov cl,dl
	mov dx,0xFFFF
	shr dx,cl
	add di,10
	add bx,80
	
LBL(aspr_c4)

	shr bx,3
	sub di,bx			// di ok
						// partie gauche du masque ok
	cmp ax,GX_CLIPD
	jl aspr_c5
						// si le sprite depasse à droite
	sub ax,GX_CLIPD
	cmp al,15
	jg aspr_byebye2		// on est tout-a-fait a droite
	mov cl,al
	shr dx,cl
	shl dx,cl

LBL(aspr_c5)		// masque terminé

	push bp				// push avant le test!
	or dx,dx
	jz aspr16_fin		// plus rien à afficher

	mov ax,x
	mov bp,dx
	mov cl,al
	and cl,7
	dec di

LBL(affich16_0)

	mov dx, ds:[si+4]
	or dx, ds:[si+2]
	lodsw
	or dx,ax
	and dx,bp			// masque inverse dans dx
	and ax,bp
	xor ax,dx			// 1ere couche dans ax

	mov bh,dl
	xor bl,bl
	shr dx,cl
	shr bx,cl
	not dx					
	not bl				// dx et bl = masque décalé

	and es:[di],dx	
	and es:[di-1],bl
	and es:[di+0x800],dx
	and es:[di+0x800-1],bl
	and es:[di+0x1000],dx	// application du masque sur les 3 buffers
	and es:[di+0x1000-1],bl

	mov dh,al				// ax = premiere couche non décalée
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = premiere couche décalée

	or es:[di],ax			// affiche le mot sur le buffer 1
	or es:[di-1],dl

	lodsw
	and ax,bp
	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = deuxieme couche décalée

	or es:[di+0x800],ax		// affiche le mot sur le buffer 2
	or es:[di+0x800-1],dl

	lodsw
	and ax,bp
	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = troisieme couche décalée

	or es:[di+0x1000],ax		// affiche le mot sur le buffer 2
	or es:[di+0x1000-1],dl

	sub di,16				// passe a la ligne suivante dans la memoire video

	dec ch
	jnz affich16_0		// on doit afficher ch lignes

LBL(aspr16_fin)

	inc di
	pop bp					// retire bp de la pile
	add x,16				// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp aspr_debut			// on retourne a la case depart
		
LBL(aspr_byebye3)

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

	jmp quit

LBL(aspr_8clip)	

	mov dx,bx
	and dl,7			// décalage
	mov dh,0xFF			// masque
	mov ax,bx
	add ax,8
	cmp bx,GX_CLIPG
	jnl aspr_c6
						// si le sprite dépasse à gauche
	sub ax,GX_CLIPG+8
	cmp ax,-7
	jl aspr_byebye3		// on est tout-a-fait a gauche
	neg ax
	mov cl,al
	shr dh,cl
	add di,10
	add bx,80
	
LBL(aspr_c6)

	mov ax,bx
	add ax,8
	shr bx,3
	sub di,bx			// di ok
						// partie gauche du masque ok
	cmp ax,GX_CLIPD
	jng aspr_c7
						// si le sprite depasse à droite
	sub ax,GX_CLIPD
	cmp al,7
	jg aspr_byebye3		// on est tout-a-fait a droite
	mov cl,al
	shr dh,cl
	shl dh,cl

LBL(aspr_c7)		// masque de clipping terminé

	mov cl,dl
	mov dl,dh
	mov bp,dx
	dec di

LBL(affich8_0)			

	lodsw				// ah = 3eme couche, al = 2eme couche
	and ax,bp			// applique le masque de clipping aux couches
	mov dx,ax			// sauvegarde dans dx

	lodsb				// 1ere couche dans al
	and ax,bp			// applique le masque de clipping
	mov bh,al
	or bh,dl
	or bh,dh
	jz affich8_1
	xor al,bh

	xor bl,bl
	shr bx,cl			// decalage et application du masque
	not bx
	and es:[di],bx	
	and es:[di+0x800],bx
	and es:[di+0x1000],bx	

	mov bh,al
	xor bl,bl	
	shr bx,cl			// decalage de la premiere couche
	or es:[di],bx	

	mov bh,dl
	xor bl,bl	
	shr bx,cl			// decalage de la deuxieme couche
	or es:[di+0x800],bx	

	xor dl,dl	
	shr dx,cl			// decalage de la troisieme couche
	or es:[di+0x1000],dx	

LBL(affich8_1)

	sub di,16			// passe a la ligne suivante dans la memoire video
	dec ch
	jnz affich8_0		// on doit afficher ch lignes

	add sp,4			// oublie les donnees présentes dans la pile (proch bloc + hauteur)
	pop ds

LBL(quit)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_SPR_NOM_C
void gxSpr_noM_C(int x,int y,void far* spr)
{
		// decalage de la position propre du sprite par rapport à (x,y)
	x -= *(((signed char far*)spr)++);
	y -= *(((signed char far*)spr)++);

asm { push ds

	mov ax,0x1A60
	mov es,ax
	lds si,spr			// ds = segment du pointeur spr, si = offset
						// si pointe sur la hauteur du sprite
	cld					// si sera incremente
	lodsb				// al = hauteur du sprite
	
	xor ah,ah
	push ax				// hauteur du sprite dans la pile
						// si pointe vers la largeur du bloc
	push 1

LBL(aspr_debut)
	
	lds si,spr			// ds:si pointe vers le debut du sprite
	mov bx,sp
	mov dx,ss:[bx]		// recupere l'emplacement de debut du bloc suivant 
	add si,dx
	inc dx
							// si pointe vers la largeur du bloc a afficher
	lodsb					// al = largeur (restante) du sprite
	cmp al,0
	je aspr_byebye
	
	mov di,1023
	
LBL(aspr_deb2)
	
	mov bx,sp
	mov bx,ss:[bx+2]	// bx recupere la hauteur du sprite dans la pile
	mov cl,bl			// cl contient lui aussi la hauteur
	xor ch,ch
	shl bx,1
	add bx,cx			// bx = 3* hauteur
	cmp al,8
	jng aspr_deb1
	shl bx,1			// bx contient la taille du bloc (sans la largeur)
	mov al,16
	
LBL(aspr_deb1)

	mov ch,cl
	add dx,bx			// dx contient l'emplacement de debut du bloc suivant
	mov bx,sp
	mov ss:[bx],dx		// on sauvegarde cette donnee pour le prochain saut vers spr_debut

	mov dx,y
	add cl,dl			// cl = y+h
						// pret pour la verif du clip haut
	cmp dl,GX_CLIPH
	jl aspr_c1
						// le sprite ne dépasse pas en haut
						// si est ok, ch est bon pour le clip haut
	shl dx,4
	sub di,dx			// di ok
	cmp cl,GX_CLIPB
	jnl aspr_c3b

	jmp aspr_c3c		// acces direct vers le clip horz si pas de clip vertic

LBL(aspr_byebye)	// on se barre

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

	jmp quit

LBL(aspr_c1)			// le sprite dépasse en haut 

	sub dl,GX_CLIPH
	neg dl				// nbre de lignes a ne pas afficher
	sub ch,dl			// ch ok
	mov bx,dx
	shl dx,1
	add dx,bx			// dx = 3*dx
	cmp al,8
	jng aspr_c1a
	shl dx,1

LBL(aspr_c1a)

	add si,dx			// si ok
	mov dx,GX_CLIPH
	shl dx,4
	sub di,dx			// di ok
						// pret pour la verif du clip bas
	cmp cl,GX_CLIPB
	jl aspr_c3

LBL(aspr_c3b)		// le sprite dépasse en bas

	sub cl,GX_CLIPB
	sub ch,cl			// on retire les lignes qui dépassent

LBL(aspr_c3)

	cmp ch,0
	jng aspr_byebye		// plus de lignes à afficher => bye bye

						// di -> octet de gauche de la ligne ou afficher
						// si -> premier octet de la premiere ligne du bloc a afficher
						// ch = nbre de lignes a afficher
						// pret pour la verif du clipping horiz
						// al = largeur reelle majoree à 16
LBL(aspr_c3c)

	mov bx,x
	cmp al,8
	jg aspr_16clip
	jmp aspr_8clip

LBL(aspr_byebye2)	// on se barre

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

	jmp quit

LBL(aspr_suiv)

	add x,16				// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp aspr_debut			// on retourne a la case depart

LBL(aspr_16clip)	// on va s'occuper des morceaux >= 16 pix

	// objectifs:
	//	- creer un masque sur 2 octets
	//	- positionner di
	//  - mettre la bonne valeur de décalage dans cl (x%8)
	// pas touche à ch!

	mov dx,0xFFFF		// masque
	mov ax,bx
	add ax,16			// ax = x+16
	cmp bx,GX_CLIPG
	jnl aspr_c4
						// si le sprite dépasse à gauche
	mov dx,bx
	sub dx,GX_CLIPG
	cmp dx,-15
	jl aspr_suiv		// on est tout-a-fait a gauche
	neg dx
	mov cl,dl
	mov dx,0xFFFF
	shr dx,cl
	add di,10
	add bx,80
	
LBL(aspr_c4)

	shr bx,3
	sub di,bx			// di ok
						// partie gauche du masque ok
	cmp ax,GX_CLIPD
	jl aspr_c5
						// si le sprite depasse à droite
	sub ax,GX_CLIPD
	cmp al,15
	jg aspr_byebye2		// on est tout-a-fait a droite
	mov cl,al
	shr dx,cl
	shl dx,cl

LBL(aspr_c5)		// masque terminé

	push bp				// push avant le test!
	or dx,dx
	jz aspr16_fin		// plus rien à afficher

	mov ax,x
	mov bp,dx
	mov cl,al
	and cl,7
	dec di

LBL(affich16_0)

	mov dx, ds:[si+4]
	lodsw
	mov bx,ax
	or dx,bx
	lodsw
	or dx,ax
	xor bx,dx
	and bx,bp				// 1ere couche dans bx

	mov dh,bl				// bx = premiere couche non décalée
	xor dl,dl
	shr bx,cl
	shr dx,cl				// bx et dl = premiere couche décalée

	or es:[di],bx			// affiche le mot sur le buffer 1
	or es:[di-1],dl

	and ax,bp
	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = deuxieme couche décalée

	or es:[di+0x800],ax		// affiche le mot sur le buffer 2
	or es:[di+0x800-1],dl

	lodsw
	and ax,bp
	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = troisieme couche décalée

	or es:[di+0x1000],ax		// affiche le mot sur le buffer 2
	or es:[di+0x1000-1],dl

	sub di,16				// passe a la ligne suivante dans la memoire video

	dec ch
	jnz affich16_0		// on doit afficher ch lignes

LBL(aspr16_fin)

	inc di
	pop bp					// retire bp de la pile
	add x,16				// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp aspr_debut			// on retourne a la case depart
		
LBL(aspr_byebye3)

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

	jmp quit

LBL(aspr_8clip)	

	mov dx,bx
	and dl,7			// décalage
	mov dh,0xFF			// masque
	mov ax,bx
	add ax,8
	cmp bx,GX_CLIPG
	jnl aspr_c6
						// si le sprite dépasse à gauche
	sub ax,GX_CLIPG+8
	cmp ax,-7
	jl aspr_byebye3		// on est tout-a-fait a gauche
	neg ax
	mov cl,al
	shr dh,cl
	add di,10
	add bx,80
	
LBL(aspr_c6)

	mov ax,bx
	add ax,8
	shr bx,3
	sub di,bx			// di ok
						// partie gauche du masque ok
	cmp ax,GX_CLIPD
	jng aspr_c7
						// si le sprite depasse à droite
	sub ax,GX_CLIPD
	cmp al,7
	jg aspr_byebye3		// on est tout-a-fait a droite
	mov cl,al
	shr dh,cl
	shl dh,cl

LBL(aspr_c7)		// masque de clipping terminé

	mov cl,dl
	mov dl,dh
	mov bp,dx
	dec di

LBL(affich8_0)			

	lodsw				// ah = 3eme couche, al = 2eme couche
	and ax,bp			// applique le masque de clipping aux couches
	mov dx,ax			// sauvegarde dans dx

	lodsb				// 1ere couche dans al
	and ax,bp			// applique le masque de clipping
	mov bh,al
	or bh,dl
	or bh,dh
	jz affich8_1
	xor al,bh

	mov bh,al
	xor bl,bl	
	shr bx,cl			// decalage de la premiere couche
	or es:[di],bx	

	mov bh,dl
	xor bl,bl	
	shr bx,cl			// decalage de la deuxieme couche
	or es:[di+0x800],bx	

	xor dl,dl	
	shr dx,cl			// decalage de la troisieme couche
	or es:[di+0x1000],dx	

LBL(affich8_1)

	sub di,16			// passe a la ligne suivante dans la memoire video
	dec ch
	jnz affich8_0		// on doit afficher ch lignes

	add sp,4			// oublie les donnees présentes dans la pile (proch bloc + hauteur)
	pop ds

LBL(quit)
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_SPR_M_NOC
void gxSpr_M_noC(int x,int y,void far* spr)
{
		// decalage de la position propre du sprite par rapport à (x,y)
	x -= *(((signed char far*)spr)++);
	y -= *(((signed char far*)spr)++);

asm { push ds

	mov ax,0x1A60
	mov es,ax
	lds si,spr			// ds = segment du pointeur spr, si = offset
						// si pointe sur la hauteur du sprite
	cld					// si sera incremente
	lodsb				// al = hauteur du sprite
	
	xor ah,ah
	push ax				// sauvegarde la hauteur du sprite dans la pile

	push 1		// on place l'offset de debut du bloc suivant (ici le premier bloc) dans la pile

LBL(cspr_debut)
	
	lds si,spr			// ds:si pointe vers le debut du sprite
	mov bx,sp
	mov dx,ss:[bx]		// recupere l'emplacement de debut du bloc suivant 
	add si,dx
	inc dx
						// si pointe vers la largeur du bloc a afficher
	lodsb				// al = largeur (restante) du sprite
	cmp al,0
	jne cpas_fini
	push 0				// on termine la fonction par deux pop
	jmp cspr_fin	

LBL(cpas_fini)

	cmp al,16
	jng cspr_deb2
	mov al,16
	
LBL(cspr_deb2)
	
	mov bx,sp
	mov bx,ss:[bx+2]	// bx recupere la hauteur du sprite dans la pile
	mov cx,bx			// cx contient lui aussi la hauteur
	shl bx,1
	add bx,cx			// bx = bx*3
	cmp al,8
	jle cspr_deb1
	shl bx,1			// cx contient la taille du bloc (sans la largeur)
	
LBL(cspr_deb1)

	add dx,bx			// dx contient l'emplacement de debut du bloc suivant
	mov bx,sp
	mov ss:[bx],dx		// on sauvegarde cette donnee pour le prochain saut vers spr_debut

	mov di,1022

	mov ch,cl			// nbre de lignes a afficher
	mov dx,y
	shl dx,4			// y = 16*y
	sub di,dx

	mov bx,x
	mov cl,bl
	shr bx,3
	sub di,bx			// di pointe vers l'octet le plus a gauche pouvant contenir la ligne du sprite
	and cl,7			// cl contient le decalage droit

	cmp al,8
	jng affich8_0
	jmp affich16_0

LBL(affich8_0)			

	lodsw				// ah = 3eme couche, al = 2eme couche
	mov dx,ax			// sauvegarde dans dx

	lodsb				// 1ere couche dans al
	mov bh,al
	or bh,dl
	or bh,dh
	jz affich8_1
	xor al,bh

	xor bl,bl
	shr bx,cl			// decalage et application du masque
	not bx
	and es:[di],bx	
	and es:[di+0x800],bx
	and es:[di+0x1000],bx	

	mov bh,al
	xor bl,bl	
	shr bx,cl			// decalage de la premiere couche
	or es:[di],bx	

	mov bh,dl
	xor bl,bl	
	shr bx,cl			// decalage de la deuxieme couche
	or es:[di+0x800],bx	

	xor dl,dl	
	shr dx,cl			// decalage de la troisieme couche
	or es:[di+0x1000],dx	

LBL(affich8_1)

	sub di,16			// passe a la ligne suivante dans la memoire video
	dec ch
	jnz affich8_0		// on doit afficher ch lignes

	jmp cspr_fin			// le sprite a été affiché entièrement, on saute a la fin

LBL(affich16_0)

	mov dx, ds:[si+4]
	or dx, ds:[si+2]
	lodsw
	or dx,ax
	xor ax,dx			// 1ere couche dans ax

	mov bh,dl
	xor bl,bl
	shr dx,cl
	shr bx,cl
	not dx					
	not bl				// dx et bl = masque décalé

	and es:[di],dx	
	and es:[di-1],bl
	and es:[di+0x800],dx
	and es:[di+0x800-1],bl
	and es:[di+0x1000],dx	// application du masque sur les 3 buffers
	and es:[di+0x1000-1],bl

	mov dh,al				// ax = premiere couche non décalée
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = premiere couche décalée

	or es:[di],ax			// affiche le mot sur le buffer 1
	or es:[di-1],dl

	lodsw
	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = deuxieme couche décalée

	or es:[di+0x800],ax		// affiche le mot sur le buffer 2
	or es:[di+0x800-1],dl

	lodsw
	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = troisieme couche décalée

	or es:[di+0x1000],ax		// affiche le mot sur le buffer 2
	or es:[di+0x1000-1],dl

	sub di,16				// passe a la ligne suivante dans la memoire video

	dec ch
	jnz affich16_0		// on doit afficher ch lignes

	add x,16			// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp cspr_debut		// on retourne a la case depart

LBL(cspr_fin)

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_SPR_NOM_NOC
void gxSpr_noM_noC(int x,int y,void far* spr)
{
		// decalage de la position propre du sprite par rapport à (x,y)
	x -= *(((signed char far*)spr)++);
	y -= *(((signed char far*)spr)++);

asm { push ds

	mov ax,0x1A60
	mov es,ax
	lds si,spr			// ds = segment du pointeur spr, si = offset
						// si pointe sur la hauteur du sprite
	cld					// si sera incremente
	lodsb				// al = hauteur du sprite
	
	xor ah,ah
	push ax				// sauvegarde la hauteur du sprite dans la pile

	push 1		// on place l'offset de debut du bloc suivant (ici le premier bloc) dans la pile

LBL(cspr_debut)
	
	lds si,spr			// ds:si pointe vers le debut du sprite
	mov bx,sp
	mov dx,ss:[bx]		// recupere l'emplacement de debut du bloc suivant 
	add si,dx
	inc dx
						// si pointe vers la largeur du bloc a afficher
	lodsb				// al = largeur (restante) du sprite
	cmp al,0
	jne cpas_fini
	push 0				// on termine la fonction par deux pop
	jmp cspr_fin	

LBL(cpas_fini)

	cmp al,16
	jng cspr_deb2
	mov al,16
	
LBL(cspr_deb2)
	
	mov bx,sp
	mov bx,ss:[bx+2]	// bx recupere la hauteur du sprite dans la pile
	mov cx,bx			// cx contient lui aussi la hauteur
	shl bx,1
	add bx,cx			// bx = bx*3
	cmp al,8
	jle cspr_deb1
	shl bx,1			// cx contient la taille du bloc (sans la largeur)
	
LBL(cspr_deb1)

	add dx,bx			// dx contient l'emplacement de debut du bloc suivant
	mov bx,sp
	mov ss:[bx],dx		// on sauvegarde cette donnee pour le prochain saut vers spr_debut

	mov di,1022

	mov ch,cl			// nbre de lignes a afficher
	mov dx,y
	shl dx,4			// y = 16*y
	sub di,dx

	mov bx,x
	mov cl,bl
	shr bx,3
	sub di,bx			// di pointe vers l'octet le plus a gauche pouvant contenir la ligne du sprite
	and cl,7			// cl contient le decalage droit

	cmp al,8
	jng affich8_0
	jmp affich16_0

LBL(affich8_0)			

	lodsw				// ah = 3eme couche, al = 2eme couche
	mov dx,ax			// sauvegarde dans dx

	lodsb				// 1ere couche dans al
	mov bh,al
	or bh,dl
	or bh,dh
	jz affich8_1
	xor al,bh

	mov bh,al
	xor bl,bl	
	shr bx,cl			// decalage de la premiere couche
	or es:[di],bx	

	mov bh,dl
	xor bl,bl	
	shr bx,cl			// decalage de la deuxieme couche
	or es:[di+0x800],bx	

	xor dl,dl	
	shr dx,cl			// decalage de la troisieme couche
	or es:[di+0x1000],dx	

LBL(affich8_1)

	sub di,16			// passe a la ligne suivante dans la memoire video
	dec ch
	jnz affich8_0		// on doit afficher ch lignes

	jmp cspr_fin			// le sprite a été affiché entièrement, on saute a la fin

LBL(affich16_0)

	mov dx, ds:[si+4]
	lodsw
	mov bx,ax
	or dx,bx
	lodsw
	or dx,ax
	xor bx,dx				// 1ere couche dans bx

	mov dh,bl				// bx = premiere couche non décalée
	xor dl,dl
	shr bx,cl
	shr dx,cl				// bx et dl = premiere couche décalée

	or es:[di],bx			// affiche le mot sur le buffer 1
	or es:[di-1],dl

	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = deuxieme couche décalée

	or es:[di+0x800],ax		// affiche le mot sur le buffer 2
	or es:[di+0x800-1],dl

	lodsw
	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = troisieme couche décalée

	or es:[di+0x1000],ax		// affiche le mot sur le buffer 2
	or es:[di+0x1000-1],dl

	sub di,16				// passe a la ligne suivante dans la memoire video

	dec ch
	jnz affich16_0		// on doit afficher ch lignes

	add x,16			// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp cspr_debut		// on retourne a la case depart

LBL(cspr_fin)

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_GX_SPRITE
void gxSprite(int x, int y, void far* spr, uchar mode)	// affiche un sprite
{
	switch(mode)
	{
#ifdef FUNC_GX_SPR_M_C
		case GX_SPR_MASK_CLIP:		gxSpr_M_C(x,y,spr);		break; 
#endif
#ifdef FUNC_GX_SPR_NOM_C
		case GX_SPR_NOMASK_CLIP:	gxSpr_noM_C(x,y,spr);	break; 
#endif
#ifdef FUNC_GX_SPR_M_NOC
		case GX_SPR_MASK_NOCLIP:	gxSpr_M_noC(x,y,spr);	break; 
#endif		
#ifdef FUNC_GX_SPR_NOM_NOC
		case GX_SPR_NOMASK_NOCLIP:	gxSpr_noM_noC(x,y,spr);	break; 
#endif
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////

