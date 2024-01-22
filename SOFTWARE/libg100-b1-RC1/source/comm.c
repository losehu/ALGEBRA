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
 *           libcg100 : comm.c
 *
 *           Original : GLink by dada66
 *
 *           (C) Copyright 2003-2004 Quaranta Software 
 *
**************/

#include "comm.h"

static unsigned int Ds_Seg;

char speeds[] = {0x7F, 0x5B, 0x41, 0x2B, 0x21, 0x17, 0x11, 0x0B};

void InitPort(int vitesse)
{ 
	
	asm{
		in al, 0x11;
		or al, 0x60;
		out 0x11, al;	//Port active
		xor al, al;
		out 0x44, al;	//Initialise le port
		mov si, OFFSET speeds;
 		add si, vitesse;
		mov al, ds:[si];
		out 0x47, al;	//defini la vitesse en bps max
		mov al, 0x16;
		out 0x46, al;	//Averti au recepteur son entree
		mov al, 0x41
		out 0x45, al;
		mov al, 0x7C
		out 0x44, al;	//Port configure a la vitesse choisie
	}
}

void ClosePort() //Desactive le port de communication
{
	asm{
		in al, 0x11;
		and al, 0x9F;
		out 0x11, al;
	}
}

void Send_long(unsigned int nSegment, unsigned int off_set, unsigned int nSize)
{
	asm mov es, nSegment;
	asm mov si, off_set;
	asm mov bx, nSize;
	
boucle: //Boucle tant que tous les octets ne sont pas envoyes
	asm mov al, es:[si];
	asm out 0x46, al;
attendre: //Attend l'envoi de l'octet
	asm in al, 0x45;
	asm and al, 1;
	asm jz attendre;
	asm inc si;
	asm dec bx;
	asm jne boucle;
}

void Send(const void* buffer, unsigned int nSize)
{
	asm mov ax, ds;
    asm mov Ds_Seg, ax;
	Send_long(Ds_Seg, (unsigned)buffer, nSize);
}

int Receive_long(unsigned int nSegment, unsigned int off_set, unsigned int nSize, unsigned int time_out)
{
	asm mov es, nSegment;
	asm mov si, off_set;
	asm mov bx, nSize;
	asm mov cx, time_out;
	asm mov di, 350;
boucle:	//Boucle tant que tous les octets n'ont pas ete recus
	asm mov dx, cx;

attendre: //Attend la presence d'un octet
	asm dec di;
	asm jne read;
	asm mov di, 350;
	asm dec dx;
	asm je err;
read:
	asm in al, 0x45;
	asm and al, 0x1A;
	asm cmp al, 0x2;
	asm jne attendre;	//Aucun octet n'a ete recu
	asm in al, 0x44;
	asm mov es:[si], al
	asm inc si;
	asm dec bx;
	asm jne boucle;
	return 1;
err:
	return TIME_OUT;
}

int Receive(void* buffer, unsigned int nSize, unsigned int time_out)
{     
	asm mov ax, ds;
	asm mov Ds_Seg, ds;
	return Receive_long(Ds_Seg, (unsigned)buffer, nSize, time_out);
}
