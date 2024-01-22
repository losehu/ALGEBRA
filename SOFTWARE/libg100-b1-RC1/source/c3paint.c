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
 *           libg100 : c3paint.c
 *
 *			 Original Author : Casiom@x and Mastermage
 *
**************/

#include "c3paint.h"

void Sprite8(int x,int y,void* spr,int segm)
{
	asm{
	   push ax
	   push bx
	   push cx
	   push dx
	   push si
	   push di
	   push es
	
	   mov cx,x
	   mov dx,y
	   mov si,spr
	   mov ax,segm
	   mov es,ax
	
	   mov di,1023
	   mov bx,8
	
	   cmp dx,0
	   jge sp8clippedTop
	
	   add si,dx
	   add bx,dx
	   xor dx,dx
	   cmp bx,0
	   jg sp8clippedy
	   jmp sp8end
	}
	sp8clippedTop:
	asm{
	   cmp dx,56
	   jle sp8clippedy
	
	   mov ax,dx
	   sub ax,56
	   sub bx,ax
	   jg sp8clippedy
	   jmp sp8end
	}
	sp8clippedy:
	asm{
	   sub di,dx
	   cmp cx,0
	   jge sp8OKLeft
	   jmp Sprite8_SpecialLeft
	}
	sp8OKLeft:
	asm{
	   cmp cx,120
	   jl sp8OKRight
	   jmp Sprite8_SpecialRight
	}
	sp8OKRight:
	asm{
	   sub di,64
	   mov ax,cx
	   and ax,0xfff8
	   shl ax,3
	   sub di,ax
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8loopdisplay:
	asm{
	   xor al,al
	   or ah,0xff
	   shr ax,cl
	   and es:[di+bx],ah
	   and es:[di+bx+64],al
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   or es:[di+bx],al
	   or es:[di+bx+64],ah
	   dec bx
	   cmp bx,dx
	   jne sp8loopdisplay
	   jmp sp8end
	}
	
	Sprite8_SpecialRight:
	asm{
	   cmp cx,128
	   jl sp8srOKclipped
	   jmp sp8end
	}
	sp8srOKclipped:
	asm{
	   mov ax,cx
	   and ax,0xfff8
	   shl ax,3
	   sub di,ax
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8srloopdisplay:
	asm{
	   xor al,al
	   or ah,0xff
	   shr ax,cl
	   and es:[di+bx],al
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   xor es:[di+bx],ah
	   dec bx
	   cmp bx,dx
	   jne sp8srloopdisplay
	   jmp sp8end
	}
	
	Sprite8_SpecialLeft:
	asm{
	   cmp cx,-8
	   jg sp8slOKclipped
	   jmp sp8end
	}
	sp8slOKclipped:
	asm{
	   add cx,8
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8slloopdisplay:
	asm{
	   xor al,al
	   or ah,0xff
	   shr ax,cl
	   and es:[di+bx],ah
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   xor es:[di+bx],al
	   dec bx
	   cmp bx,dx
	   jne sp8slloopdisplay
	}
	sp8end:
	asm{
	   pop es
	   pop di
	   pop si
	   pop dx
	   pop cx
	   pop bx
	   pop ax
	}
	return;
}

void Sprite8or(int x,int y,void* spr,int segm)
{
	asm{
	   push ax
	   push bx
	   push cx
	   push dx
	   push si
	   push di
	   push es
	
	   mov cx,x
	   mov dx,y
	   mov si,spr
	   mov ax,segm
	   mov es,ax
	
	   mov di,1023
	   mov bx,8
	
	   cmp dx,0
	   jge sp8orclippedTop
	
	   add si,dx
	   add bx,dx
	   xor dx,dx
	   cmp bx,0
	   jg sp8orclippedy
	   jmp sp8orend
	}
	sp8orclippedTop:
	asm{
	   cmp dx,56
	   jle sp8orclippedy
	
	   mov ax,dx
	   sub ax,56
	   sub bx,ax
	   jg sp8orclippedy
	   jmp sp8orend
	}
	sp8orclippedy:
	asm{
	   sub di,dx
	   cmp cx,0
	   jge sp8orOKLeft
	   jmp Sprite8or_SpecialLeft
	}
	sp8orOKLeft:
	asm{
	   cmp cx,120
	   jl sp8orOKRight
	   jmp Sprite8or_SpecialRight
	}
	sp8orOKRight:
	asm{
	   sub di,64
	   mov ax,cx
	   and ax,0xfff8
	   shl ax,3
	   sub di,ax
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8orloopdisplay:
	asm{
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   or es:[di+bx],al
	   or es:[di+bx+64],ah
	   dec bx
	   cmp bx,dx
	   jne sp8orloopdisplay
	   jmp sp8orend
	}
	
	Sprite8or_SpecialRight:
	asm{
	   cmp cx,128
	   jl sp8orsrOKclipped
	   jmp sp8orend
	}
	sp8orsrOKclipped:
	asm{
	   mov ax,cx
	   and ax,0xfff8
	   shl ax,3
	   sub di,ax
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8orsrloopdisplay:
	asm{
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   or es:[di+bx],ah
	   dec bx
	   cmp bx,dx
	   jne sp8orsrloopdisplay
	   jmp sp8orend
	}
	
	Sprite8or_SpecialLeft:
	asm{
	   cmp cx,-8
	   jg sp8orslOKclipped
	   jmp sp8orend
	}
	sp8orslOKclipped:
	asm{
	   add cx,8
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8orslloopdisplay:
	asm{
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   or es:[di+bx],al
	   dec bx
	   cmp bx,dx
	   jne sp8orslloopdisplay
	}
	sp8orend:
	asm{
	   pop es
	   pop di
	   pop si
	   pop dx
	   pop cx
	   pop bx
	   pop ax
	}
	return;
}

void Sprite8xor(int x,int y,void* spr,int segm)
{
	asm{
	   push ax
	   push bx
	   push cx
	   push dx
	   push si
	   push di
	   push es
	
	   mov cx,x
	   mov dx,y
	   mov si,spr
	   mov ax,segm
	   mov es,ax
	
	   mov di,1023
	   mov bx,8
	
	   cmp dx,0
	   jge sp8xorclippedTop
	
	   add si,dx
	   add bx,dx
	   xor dx,dx
	   cmp bx,0
	   jg sp8xorclippedy
	   jmp sp8xorend
	}
	sp8xorclippedTop:
	asm{
	   cmp dx,56
	   jle sp8xorclippedy
	
	   mov ax,dx
	   sub ax,56
	   sub bx,ax
	   jg sp8xorclippedy
	   jmp sp8xorend
	}
	sp8xorclippedy:
	asm{
	   sub di,dx
	   cmp cx,0
	   jge sp8xorOKLeft
	   jmp Sprite8xor_SpecialLeft
	}
	sp8xorOKLeft:
	asm{
	   cmp cx,120
	   jl sp8xorOKRight
	   jmp Sprite8xor_SpecialRight
	}
	sp8xorOKRight:
	asm{
	   sub di,64
	   mov ax,cx
	   and ax,0xfff8
	   shl ax,3
	   sub di,ax
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8xorloopdisplay:
	asm{
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   xor es:[di+bx],al
	   xor es:[di+bx+64],ah
	   dec bx
	   cmp bx,dx
	   jne sp8xorloopdisplay
	   jmp sp8xorend
	}
	
	Sprite8xor_SpecialRight:
	asm{
	   cmp cx,128
	   jl sp8xorsrOKclipped
	   jmp sp8xorend
	}
	sp8xorsrOKclipped:
	asm{
	   mov ax,cx
	   and ax,0xfff8
	   shl ax,3
	   sub di,ax
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8xorsrloopdisplay:
	asm{
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   xor es:[di+bx],ah
	   dec bx
	   cmp bx,dx
	   jne sp8xorsrloopdisplay
	   jmp sp8xorend
	}
	
	Sprite8xor_SpecialLeft:
	asm{
	   cmp cx,-8
	   jg sp8xorslOKclipped
	   jmp sp8xorend
	}
	sp8xorslOKclipped:
	asm{
	   add cx,8
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8xorslloopdisplay:
	asm{
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   xor es:[di+bx],al
	   dec bx
	   cmp bx,dx
	   jne sp8xorslloopdisplay
	}
	sp8xorend:
	asm{
	   pop es
	   pop di
	   pop si
	   pop dx
	   pop cx
	   pop bx
	   pop ax
	}
	return;
}

void Sprite8_noclip(int x,int y,void* spr,int segm)
{
	asm{
	   push ax
	   push bx
	   push cx
	   push dx
	   push si
	   push di
	   push es
	
	   mov cx,x
	   mov dx,y
	   mov si,spr
	   mov ax,segm
	   mov es,ax
	
	   mov di,959 //1023-64
	   mov bx,8
	   sub di,dx
	   mov ax,cx
	   and ax,0xfff8
	   shl ax,3
	   sub di,ax
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8ncloopdisplay:
	asm{
	   xor al,al
	   or ah,0xff
	   shr ax,cl
	   and es:[di+bx],ah
	   and es:[di+bx+64],al
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   or es:[di+bx],al
	   or es:[di+bx+64],ah
	   dec bx
	   cmp bx,dx
	   jne sp8ncloopdisplay
	
	   pop es
	   pop di
	   pop si
	   pop dx
	   pop cx
	   pop bx
	   pop ax
	}
	return;
}

void Sprite8or_noclip(int x,int y,void* spr,int segm)
{
	asm{
	   push ax
	   push bx
	   push cx
	   push dx
	   push si
	   push di
	   push es
	
	   mov cx,x
	   mov dx,y
	   mov si,spr
	   mov ax,segm
	   mov es,ax
	
	   mov di,959 //1023-64
	   mov bx,8
	   sub di,dx
	   mov ax,cx
	   and ax,0xfff8
	   shl ax,3
	   sub di,ax
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8xorncloopdisplay:
	asm{
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   or es:[di+bx],al
	   or es:[di+bx+64],ah
	   dec bx
	   cmp bx,dx
	   jne sp8xorncloopdisplay
	
	   pop es
	   pop di
	   pop si
	   pop dx
	   pop cx
	   pop bx
	   pop ax
	}
	return;
}

void Sprite8xor_noclip(int x,int y,void* spr,int segm)
{
	asm{
	   push ax
	   push bx
	   push cx
	   push dx
	   push si
	   push di
	   push es
	
	   mov cx,x
	   mov dx,y
	   mov si,spr
	   mov ax,segm
	   mov es,ax
	
	   mov di,959 //1023-64
	   mov bx,8
	   sub di,dx
	   mov ax,cx
	   and ax,0xfff8
	   shl ax,3
	   sub di,ax
	   and cx,7
	   mov dx,0
	   sub dx,bx
	   xor bx,bx
	}
	sp8xorncloopdisplay:
	asm{
	   xor ah,ah
	   lodsb
	   xchg al,ah
	   shr ax,cl
	   xor es:[di+bx],al
	   xor es:[di+bx+64],ah
	   dec bx
	   cmp bx,dx
	   jne sp8xorncloopdisplay
	
	   pop es
	   pop di
	   pop si
	   pop dx
	   pop cx
	   pop bx
	   pop ax
	}
	return;
}
