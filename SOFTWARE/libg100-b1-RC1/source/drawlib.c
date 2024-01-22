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
//*						   <DrawLib pour CASIO Graph100>						    *//
//																					 //
//               Fichier principal a ajouter dans le projet regroupant               //
//		              tous les fichiers .cpp ou .c du programmme		             //
//											                                         //
//*  Version 1.4b										   (c) Julien PATTE - 2004  *//
 /////////////////////////////////////////////////////////////////////////////////////

#include "drawlib.h"

#ifdef FUNC_DRAWSPR			// fonction principale
void drawspr(int x, char y, void far* spr, uchar mode) 
{ 
	static void (*draw_funx[4])(int,char,void far*) 
		= { draw_noM_noC, draw_M_noC, draw_noM_C, draw_M_C };

	(*(draw_funx[mode]))(x,y,spr); // appel de la fonction voulue
}
#else
void drawspr(int a,char b,void far* c,uchar d) { ((uchar far*)c)+=a+b+d; }
#endif

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_SIZESPRITE
int sizesprite(uchar Haut,uchar Larg)
{
	int total;

	if(Larg%8) Larg=Larg-Larg%8+8;	// si la largeur n'est pas multiple de 8

	total = 4								// origine+hauteur+fin du sprite
			+ (Larg/16)*(1+4*Haut)			// blocs de 16 pixels
			+ ((Larg%16)==8)*(1+2*Haut);	// bloc de 8 pixel si necessaire
	return total;
}
#else
int sizesprite(uchar a,uchar b) { a+=b; return 0; }
#endif

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_DRAW_MASK_CLIP
void draw_M_C(int x,char y,void far* spr)
{
		// decalage de la position propre du sprite par rapport à (x,y)
	x -= *(((signed char far*)spr)++);
	y -= *(((signed char far*)spr)++);

asm { push ds

	mov ax,DRAW_BUFFER1
	mov es,ax
	lds si,spr			// ds = segment du pointeur spr, si = offset
						// si pointe sur la hauteur du sprite
	cld					// si sera incremente
	lodsb				// al = hauteur du sprite
	
	xor ah,ah
	push ax				// hauteur du sprite dans la pile
						// si pointe vers la largeur du bloc
	push 1

} aspr_debut: asm {
	
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
	
} aspr_deb2: asm {
	
	mov bx,sp
	mov bx,ss:[bx+2]	// bx recupere la hauteur du sprite dans la pile
	mov ch,bl			// ch contient lui aussi la hauteur
	shl bx,1
	cmp al,8
	jng aspr_deb1
	shl bx,1			// cx contient la taille du bloc (sans la largeur)
	mov al,16
	
} aspr_deb1: asm {

	add dx,bx			// dx contient l'emplacement de debut du bloc suivant
	mov bx,sp
	mov ss:[bx],dx		// on sauvegarde cette donnee pour le prochain saut vers spr_debut

	xor dh,dh
	mov dl,y
	mov cl,dl
	add cl,ch			// cl = y+h
						// pret pour la verif du clip haut
	cmp dl,DRAW_CLIPH
	jl aspr_c1
						// le sprite ne dépasse pas en haut
						// si est ok, ch est bon pour le clip haut
	shl dx,4
	sub di,dx			// di ok
	cmp cl,DRAW_CLIPB
	jnl aspr_c3b

	jmp aspr_c3c		// acces direct vers le clip horz si pas de clip vertic

} aspr_byebye: asm {	// on se barre

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

}	return;

aspr_c1: asm {			// le sprite dépasse en haut 

	sub dl,DRAW_CLIPH
	neg dl				// nbre de lignes a ne pas afficher
	sub ch,dl			// ch ok
	shl dx,1
	cmp al,8
	jng aspr_c1a
	shl dx,1

} aspr_c1a: asm {

	add si,dx			// si ok
	mov dx,DRAW_CLIPH
	shl dx,4
	sub di,dx			// di ok
						// pret pour la verif du clip bas
	cmp cl,DRAW_CLIPB
	jl aspr_c3

} aspr_c3b: asm {		// le sprite dépasse en bas

	sub cl,DRAW_CLIPB
	sub ch,cl			// on retire les lignes qui dépassent

} aspr_c3: asm {

	cmp ch,0
	jng aspr_byebye		// plus de lignes à afficher => bye bye

						// di -> octet de gauche de la ligne ou afficher
						// si -> premier octet de la premiere ligne du bloc a afficher
						// ch = nbre de lignes a afficher
						// pret pour la verif du clipping horiz
						// al = largeur reelle majoree à 16
} aspr_c3c: asm {

	mov bx,x
	cmp al,8
	jg aspr_16clip
	jmp aspr_8clip

} aspr_byebye2: asm {	// on se barre

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

}	return;

aspr_suiv: asm {

	add x,16				// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp aspr_debut			// on retourne a la case depart

} aspr_16clip: asm {	// on va s'occuper des morceaux >= 16 pix

	// objectifs:
	//	- creer un masque sur 2 octets
	//	- positionner di
	//  - mettre la bonne valeur de décalage dans cl (x%8)
	// pas touche à ch!

	mov dx,0xFFFF		// masque
	mov ax,bx
	add ax,16			// ax = x+16
	cmp bx,DRAW_CLIPG
	jnl aspr_c4
						// si le sprite dépasse à gauche
	mov dx,bx
	sub dx,DRAW_CLIPG
	cmp dx,-15
	jl aspr_suiv		// on est tout-a-fait a gauche
	neg dx
	mov cl,dl
	mov dx,0xFFFF
	shr dx,cl
	add di,10
	add bx,80
	
} aspr_c4: asm {

	shr bx,3
	sub di,bx			// di ok
						// partie gauche du masque ok
	cmp ax,DRAW_CLIPD
	jl aspr_c5
						// si le sprite depasse à droite
	sub ax,DRAW_CLIPD
	cmp al,15
	jg aspr_byebye2		// on est tout-a-fait a droite
	mov cl,al
	shr dx,cl
	shl dx,cl

} aspr_c5: asm {		// masque terminé

	push bp				// push avant le test!
	or dx,dx
	jz aspr16_fin		// plus rien à afficher

	mov ax,x
	mov bp,dx
	mov cl,al
	and cl,7

} aspr16_affich2: asm {		// affichage des 3 octets							
	
	lodsw
	and ax,bp			// applique le masque de clipping
	mov dx,ax
	push dx					// dx = pile = 1er morceau sur 2 octets, ax = 2eme morceau
	lodsw
	and ax,bp

	or dx,ax				// dx = masque non décalé
	mov bh,dl
	xor bl,bl
	shr dx,cl
	shr bx,cl
	not dx					
	not bl					// dx et bl = masque décalé

	and es:[di-1],dx	
	and es:[di-2],bl
	and es:[di+DRAW_JUMP-1],dx	// affichage du masque sur les 2 buffers
	and es:[di+DRAW_JUMP-2],bl

	pop bx
	and bx,ax				// bx = premiere couche non décalée
	mov dh,bl
	xor dl,dl
	shr bx,cl
	shr dx,cl				// bx et dl = premiere couche décalée

	or es:[di-1],bx			// affiche le mot sur le buffer 1
	or es:[di-2],dl

	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = deuxieme couche décalée

	or es:[di+DRAW_JUMP-1],ax		// affiche le mot sur le buffer 2
	or es:[di+DRAW_JUMP-2],dl

	sub di,16				// passe a la ligne suivante dans la memoire video

	dec ch
	jnz aspr16_affich2		// on doit afficher ch lignes

} aspr16_fin: asm {

	pop bp					// retire bp de la pile
	add x,16				// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp aspr_debut			// on retourne a la case depart
		
} aspr_byebye3: asm {

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

}	return;

aspr_8clip: asm {	

	mov dx,bx
	and dl,7			// décalage
	mov dh,0xFF			// masque
	mov ax,bx
	add ax,8
	cmp bx,DRAW_CLIPG
	jnl aspr_c6
						// si le sprite dépasse à gauche
	sub ax,DRAW_CLIPG+8
	cmp ax,-7
	jl aspr_byebye3		// on est tout-a-fait a gauche
	neg ax
	mov cl,al
	shr dh,cl
	add di,10
	add bx,80
	
} aspr_c6: asm {

	mov ax,bx
	add ax,8
	shr bx,3
	sub di,bx			// di ok
						// partie gauche du masque ok
	cmp ax,DRAW_CLIPD
	jng aspr_c7
						// si le sprite depasse à droite
	sub ax,DRAW_CLIPD
	cmp al,7
	jg aspr_byebye3		// on est tout-a-fait a droite
	mov cl,al
	shr dh,cl
	shl dh,cl

} aspr_c7: asm {		// masque terminé

	mov cl,dl
	mov dl,dh
	push bp
	mov bp,dx

} aspr8_affich3: asm{	// affichage des deux octets							

	lodsw				// ah = premier octet, al = second octet
	and ax,bp			// applique le masque aux octets

	cmp ax,0
	je aspr8_affich3_2	// rien à afficher

	mov bh,al
	xor al,al
	xor bl,bl			// ah = premier octet, bh = second octet

	shr ax,cl
	shr bx,cl			
	mov dx,ax			// ax = dx = premier octet décalé, bx = second octet décalé

    or ax,bx
	not ax				// ax = masque, bx = couche 2

	and es:[di-1],ax	// affiche le masque sur le buffer 1
	and es:[di+DRAW_JUMP-1],ax// affiche le masque sur le buffer 2

	and dx,bx			// dx = couche 1, bx = couche 2
	jz aspr8_affich3_1	// si la premiere couche est vide on n'a rien a afficher

	or es:[di-1],dx		// affiche l'octet sur le buffer 1

} aspr8_affich3_1: asm {
	
	or es:[di+DRAW_JUMP-1],bx	// affiche l'octet sur le buffer 2

} aspr8_affich3_2: asm {

	sub di,16			// passe a la ligne suivante dans la memoire video
	dec ch
	jnz aspr8_affich3		// on doit afficher ch lignes

	add sp,6			// oublie les donnees présentes dans la pile (bp + proch bloc + hauteur)
	pop ds
	}
}
#else
void draw_M_C(int a,char b,void far* c) { ((uchar far*)c)+=a+b; }
#endif

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_DRAW_NOMASK_CLIP	// idem que la fonx précédente mais sans appliquer les masques
void draw_noM_C(int x,char y,void far* spr)
{
		// decalage de la position propre du sprite par rapport à (x,y)
	x -= *(((signed char far*)spr)++);
	y -= *(((signed char far*)spr)++);

asm { push ds

	mov ax,DRAW_BUFFER1
	mov es,ax
	lds si,spr			// ds = segment du pointeur spr, si = offset
						// si pointe sur la hauteur du sprite
	cld					// si sera incremente
	lodsb				// al = hauteur du sprite
	
	xor ah,ah
	push ax				// hauteur du sprite dans la pile
						// si pointe vers la largeur du bloc
	push 1

} bspr_debut: asm {
	
	lds si,spr			// ds:si pointe vers le debut du sprite
	mov bx,sp
	mov dx,ss:[bx]		// recupere l'emplacement de debut du bloc suivant 
	add si,dx
	inc dx
							// si pointe vers la largeur du bloc a afficher
	lodsb					// al = largeur (restante) du sprite
	cmp al,0
	je bspr_byebye
	
	mov di,1023
	
} bspr_deb2: asm {
	
	mov bx,sp
	mov bx,ss:[bx+2]	// bx recupere la hauteur du sprite dans la pile
	mov ch,bl			// ch contient lui aussi la hauteur
	shl bx,1
	cmp al,8
	jng bspr_deb1
	shl bx,1			// cx contient la taille du bloc (sans la largeur)
	mov al,16
	
} bspr_deb1: asm {

	add dx,bx			// dx contient l'emplacement de debut du bloc suivant
	mov bx,sp
	mov ss:[bx],dx		// on sauvegarde cette donnee pour le prochain saut vers spr_debut

	xor dh,dh
	mov dl,y
	mov cl,dl
	add cl,ch			// cl = y+h
						// pret pour la verif du clip haut
	cmp dl,DRAW_CLIPH
	jl bspr_c1
						// le sprite ne dépasse pas en haut
						// si est ok, ch est bon pour le clip haut
	shl dx,4
	sub di,dx			// di ok
	cmp cl,DRAW_CLIPB
	jnl bspr_c3b

	jmp bspr_c3c		// acces direct vers le clip horz si pas de clip vertic

} bspr_byebye: asm {	// on se barre

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

}	return;

bspr_c1: asm {			// le sprite dépasse en haut 

	sub dl,DRAW_CLIPH
	neg dl				// nbre de lignes a ne pas afficher
	sub ch,dl			// ch ok
	shl dx,1
	cmp al,8
	jng bspr_c1a
	shl dx,1

} bspr_c1a: asm {

	add si,dx			// si ok
	mov dx,DRAW_CLIPH
	shl dx,4
	sub di,dx			// di ok
						// pret pour la verif du clip bas
	cmp cl,DRAW_CLIPB
	jl bspr_c3

} bspr_c3b: asm {		// le sprite dépasse en bas

	sub cl,DRAW_CLIPB
	sub ch,cl			// on retire les lignes qui dépassent

} bspr_c3: asm {

	cmp ch,0
	jng bspr_byebye		// plus de lignes à afficher => bye bye

						// di -> octet de gauche de la ligne ou afficher
						// si -> premier octet de la premiere ligne du bloc a afficher
						// ch = nbre de lignes a afficher
						// pret pour la verif du clipping horiz
						// al = largeur reelle majoree à 16
} bspr_c3c: asm {

	mov bx,x
	cmp al,8
	jg bspr_16clip
	jmp bspr_8clip

} bspr_byebye2: asm {	// on se barre

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

}	return;

bspr_suiv: asm {

	add x,16				// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp bspr_debut			// on retourne a la case depart

} bspr_16clip: asm {	// on va s'occuper des morceaux >= 16 pix

	// objectifs:
	//	- creer un masque sur 2 octets
	//	- positionner di
	//  - mettre la bonne valeur de décalage dans cl (x%8)
	// pas touche à ch!

	mov dx,0xFFFF		// masque
	mov ax,bx
	add ax,16			// ax = x+16
	cmp bx,DRAW_CLIPG
	jnl bspr_c4
						// si le sprite dépasse à gauche
	mov dx,bx
	sub dx,DRAW_CLIPG
	cmp dx,-15
	jl bspr_suiv		// on est tout-a-fait a gauche
	neg dx
	mov cl,dl
	mov dx,0xFFFF
	shr dx,cl
	add di,10
	add bx,80
	
} bspr_c4: asm {

	shr bx,3
	sub di,bx			// di ok
						// partie gauche du masque ok
	cmp ax,DRAW_CLIPD
	jl bspr_c5
						// si le sprite depasse à droite
	sub ax,DRAW_CLIPD
	cmp al,15
	jg bspr_byebye2		// on est tout-a-fait a droite
	mov cl,al
	shr dx,cl
	shl dx,cl

} bspr_c5: asm {		// masque terminé

	push bp				// push avant le test!
	or dx,dx
	jz bspr16_fin		// plus rien à afficher

	mov ax,x
	mov bp,dx
	mov cl,al
	and cl,7

} bspr16_affich2: asm {		// affichage des 3 octets							
	
	lodsw
	and ax,bp				// applique le masque de clipping
	mov dx,ax
	lodsw
	and ax,bp				// dx = 1er morceau sur 2 octets, ax = 2eme morceau

	and dx,ax				// dx = premiere couche non décalée
	mov bh,dl
	xor bl,bl
	shr dx,cl
	shr bx,cl				// dx et bl = premiere couche décalée

	or es:[di-1],dx			// affiche le mot sur le buffer 1
	or es:[di-2],bl

	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = deuxieme couche décalée

	or es:[di+DRAW_JUMP-1],ax		// affiche le mot sur le buffer 2
	or es:[di+DRAW_JUMP-2],dl

	sub di,16				// passe a la ligne suivante dans la memoire video

	dec ch
	jnz bspr16_affich2		// on doit afficher ch lignes

} bspr16_fin: asm {

	pop bp					// retire bp de la pile
	add x,16				// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp bspr_debut			// on retourne a la case depart
		
} bspr_byebye3: asm {

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds 

}	return;

bspr_8clip: asm {	

	mov dx,bx
	and dl,7			// décalage
	mov dh,0xFF			// masque
	mov ax,bx
	add ax,8
	cmp bx,DRAW_CLIPG
	jnl bspr_c6
						// si le sprite dépasse à gauche
	sub ax,DRAW_CLIPG+8
	cmp ax,-7
	jl bspr_byebye3		// on est tout-a-fait a gauche
	neg ax
	mov cl,al
	shr dh,cl
	add di,10
	add bx,80
	
} bspr_c6: asm {

	mov ax,bx
	add ax,8
	shr bx,3
	sub di,bx			// di ok
						// partie gauche du masque ok
	cmp ax,DRAW_CLIPD
	jng bspr_c7
						// si le sprite depasse à droite
	sub ax,DRAW_CLIPD
	cmp al,7
	jg bspr_byebye3		// on est tout-a-fait a droite
	mov cl,al
	shr dh,cl
	shl dh,cl

} bspr_c7: asm {		// masque terminé

	mov cl,dl
	mov dl,dh
	push bp
	mov bp,dx

} bspr8_affich3: asm{	// affichage des deux octets							

	lodsw				// ah = premier octet, al = second octet
	and ax,bp			// le masque de clipping est applique aux octets

	cmp ax,0
	je bspr8_affich3_2	// rien à afficher

	mov bh,al
	xor al,al
	xor bl,bl			// ah = premier octet, bh = second octet

	shr ax,cl
	shr bx,cl			// bx = couche 1, bx = couche 2			

	and ax,bx
	jz bspr8_affich3_1	// si la premiere couche est vide on n'a rien a afficher

	or es:[di-1],ax		// affiche l'octet sur le buffer 1

} bspr8_affich3_1: asm {
	
	or es:[di+DRAW_JUMP-1],bx	// affiche l'octet sur le buffer 2

} bspr8_affich3_2: asm {

	sub di,16			// passe a la ligne suivante dans la memoire video
	dec ch
	jnz bspr8_affich3		// on doit afficher ch lignes

	add sp,6			// oublie les donnees présentes dans la pile (bp + proch bloc + hauteur)
	pop ds
	}
}
#else
void draw_noM_C(int a,char b,void far* c) { ((uchar far*)c)+=a+b; }
#endif

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_DRAW_MASK_NOCLIP
void draw_M_noC(int x,char y,void far* spr)
{
		// decalage de la position propre du sprite par rapport à (x,y)
	x -= *(((signed char far*)spr)++);
	y -= *(((signed char far*)spr)++);

asm { push ds

	mov ax,DRAW_BUFFER1
	mov es,ax
	lds si,spr			// ds = segment du pointeur spr, si = offset
						// si pointe sur la hauteur du sprite
	cld					// si sera incremente
	lodsb				// al = hauteur du sprite
	
	xor ah,ah
	push ax				// sauvegarde la hauteur du sprite dans la pile

	push 1		// on place l'offset de debut du bloc suivant (ici le premier bloc) dans la pile

/*********************************************************/

} cspr_debut: asm {
	
	lds si,spr			// ds:si pointe vers le debut du sprite
	pop dx				// recupere l'emplacement de debut du bloc suivant 
	add si,dx
	inc dx
						// si pointe vers la largeur du bloc a afficher
	lodsb				// al = largeur (restante) du sprite
	cmp al,0
	jne cpas_fini
	push 0				// on termine la fonction par deux pop
	jmp cspr_fin	

} cpas_fini: asm {

	cmp al,16
	jng cspr_deb2
	mov al,16
	
} cspr_deb2: asm {
	
	pop bx				// bx recupere la hauteur du sprite au sommet de la pile
	push bx				// et la replace dans la pile
	mov ch,bl			// ch contient lui aussi la hauteur
	shl bx,1
	cmp al,8
	jle cspr_deb1
	shl bx,1			// cx contient la taille du bloc (sans la largeur)
	
} cspr_deb1: asm {

	add dx,bx			// dx contient l'emplacement de debut du bloc suivant
	push dx				// on sauvegarde cette donnee pour le prochain saut vers spr_debut

	mov di,1023

	xor dh,dh
	mov dl,y
	shl dx,4			// y = 16*y
	sub di,dx

	mov bx,x
	mov cl,bl
	shr bx,3
	sub di,bx			// di pointe vers l'octet le plus a gauche pouvant contenir la ligne du sprite
	and cl,7			// cl contient le decalage droit

	cmp al,8
	jng cspr8_affich
	jmp cspr16_affich

/*********************************************************/

} cspr8_affich: asm{		// AFFICHAGE D'UN BLOC DE 1~8 PIXELS DE LARGE							

	lodsw				// ah = premier octet, al = second octet

	cmp ax,0
	je cspr8_affich_2	// rien à afficher

	mov bh,al
	xor al,al
	xor bl,bl			// ah = premier octet, bh = second octet

	shr ax,cl
	shr bx,cl			
	mov dx,ax			// ax = dx = premier octet décalé, bx = second octet décalé

    or ax,bx
	not ax				// ax = masque, bx = couche 2

	and es:[di-1],ax	// affiche le masque sur le buffer 1
	and es:[di+DRAW_JUMP-1],ax// affiche le masque sur le buffer 2

	and dx,bx			// dx = couche 1, bx = couche 2
	jz cspr8_affich_1	// si la premiere couche est vide on n'a rien a afficher

	or es:[di-1],dx		// affiche l'octet sur le buffer 1

} cspr8_affich_1: asm {
	
	or es:[di+DRAW_JUMP-1],bx	// affiche l'octet sur le buffer 2

} cspr8_affich_2: asm {

	sub di,16			// passe a la ligne suivante dans la memoire video
	dec ch
	jnz cspr8_affich		// on doit afficher ch lignes

	jmp cspr_fin			// le sprite a été affiché entièrement, on saute a la fin

/*********************************************************/
					
} cspr16_affich: asm {			// AFFICHAGE D'UN BLOC DE 9~16 PIXELS DE LARGE					
		
	lodsw
	mov bx,ax
	push ax
	lodsw					// bx = pile = 1er morceau sur 2 octets, ax = 2eme morceau

	or bx,ax				// bx = masque non décalé
	mov dh,bl
	xor dl,dl
	shr bx,cl
	shr dx,cl
	not bx					
	not dl					// bx et dl = masque décalé

	and es:[di-1],bx	
	and es:[di-2],dl
	and es:[di+DRAW_JUMP-1],bx	// affichage du masque sur les 2 buffers
	and es:[di+DRAW_JUMP-2],dl

	pop bx
	and bx,ax				// bx = premiere couche non décalée
	mov dh,bl
	xor dl,dl
	shr bx,cl
	shr dx,cl				// bx et dl = premiere couche décalée

	or es:[di-1],bx			// affiche le mot sur le buffer 1
	or es:[di-2],dl

	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = deuxieme couche décalée

	or es:[di+DRAW_JUMP-1],ax		// affiche le mot sur le buffer 2
	or es:[di+DRAW_JUMP-2],dl

	sub di,16				// passe a la ligne suivante dans la memoire video

	dec ch
	jnz cspr16_affich		// on doit afficher ch lignes

	add x,16			// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp cspr_debut		// on retourne a la case depart

/*********************************************************/

} cspr_fin: asm {

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds
	}
}
#else
void draw_M_noC(int a,char b,void far* c) { ((uchar far*)c)+=a+b; }
#endif

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#ifdef FUNC_DRAW_NOMASK_NOCLIP
void draw_noM_noC(int x,char y,void far* spr)
{
		// decalage de la position propre du sprite par rapport à (x,y)
	x -= *(((signed char far*)spr)++);
	y -= *(((signed char far*)spr)++);

asm { push ds

	mov ax,DRAW_BUFFER1
	mov es,ax
	lds si,spr			// ds = segment du pointeur spr, si = offset
						// si pointe sur la hauteur du sprite
	cld					// si sera incremente
	lodsb				// al = hauteur du sprite
	
	xor ah,ah
	push ax				// sauvegarde la hauteur du sprite dans la pile

	push 1		// on place l'offset de debut du bloc suivant (ici le premier bloc) dans la pile

/*********************************************************/

} dspr_debut: asm {
	
	lds si,spr			// ds:si pointe vers le debut du sprite
	pop dx				// recupere l'emplacement de debut du bloc suivant 
	add si,dx
	inc dx
						// si pointe vers la largeur du bloc a afficher
	lodsb				// al = largeur (restante) du sprite
	cmp al,0
	jne dpas_fini
	push 0				// on termine la fonction par deux pop
	jmp dspr_fin	

} dpas_fini: asm {

	cmp al,16
	jng dspr_deb2
	mov al,16
	
} dspr_deb2: asm {
	
	pop bx				// bx recupere la hauteur du sprite au sommet de la pile
	push bx				// et la replace dans la pile
	mov ch,bl			// ch contient lui aussi la hauteur
	shl bx,1
	cmp al,8
	jle dspr_deb1
	shl bx,1			// cx contient la taille du bloc (sans la largeur)
	
} dspr_deb1: asm {

	add dx,bx			// dx contient l'emplacement de debut du bloc suivant
	push dx				// on sauvegarde cette donnee pour le prochain saut vers spr_debut

	mov di,1023

	xor dh,dh
	mov dl,y
	shl dx,4			// y = 16*y
	sub di,dx

	mov bx,x
	mov cl,bl
	shr bx,3
	sub di,bx			// di pointe vers l'octet le plus a gauche pouvant contenir la ligne du sprite
	and cl,7			// cl contient le decalage droit

	cmp al,8
	jng dspr8_affich
	jmp dspr16_affich

/*********************************************************/

} dspr8_affich: asm{		// AFFICHAGE D'UN BLOC DE 1~8 PIXELS DE LARGE							

	lodsw				// ah = premier octet, al = second octet
	cmp ax,0
	je dspr8_affich_2	// rien à afficher
	mov bh,al
	xor al,al
	xor bl,bl			// ah = premier octet, bh = second octet
	shr ax,cl
	shr bx,cl			
	and ax,bx			// ax = couche 1, bx = couche 2
	jz dspr8_affich_1	// si la premiere couche est vide on n'a rien a afficher
	or es:[di-1],ax		// affiche l'octet sur le buffer 1

} dspr8_affich_1: asm {
	
	or es:[di+DRAW_JUMP-1],bx	// affiche l'octet sur le buffer 2

} dspr8_affich_2: asm {

	sub di,16			// passe a la ligne suivante dans la memoire video
	dec ch
	jnz dspr8_affich		// on doit afficher ch lignes

	jmp dspr_fin			// le sprite a été affiché entièrement, on saute a la fin

/*********************************************************/
					
} dspr16_affich: asm {			// AFFICHAGE D'UN BLOC DE 9~16 PIXELS DE LARGE					
		
	lodsw
	mov bx,ax
	lodsw					// bx = 1er morceau sur 2 octets, ax = 2eme morceau

	and bx,ax				// bx = premiere couche non décalée
	mov dh,bl
	xor dl,dl
	shr bx,cl
	shr dx,cl				// bx et dl = premiere couche décalée

	or es:[di-1],bx			// affiche le mot sur le buffer 1
	or es:[di-2],dl

	mov dh,al
	xor dl,dl
	shr ax,cl
	shr dx,cl				// ax et dl = deuxieme couche décalée

	or es:[di+DRAW_JUMP-1],ax		// affiche le mot sur le buffer 2
	or es:[di+DRAW_JUMP-2],dl

	sub di,16				// passe a la ligne suivante dans la memoire video

	dec ch
	jnz dspr16_affich		// on doit afficher ch lignes

	add x,16			// on avance x de 16 pixels puisque la suite ( s'il y en a une ) doit se trouver a coté
	jmp dspr_debut		// on retourne a la case depart

/*********************************************************/

} dspr_fin: asm {

	add sp,4			// oublie les donnees présentes dans la pile
	pop ds
	}
}
#else
void draw_noM_noC(int a,char b,void far* c) { ((uchar far*)c)+=a+b; }
#endif
