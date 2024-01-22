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
 *           libcg100 : calc.c
 *
**************/

#include "calc.h"

void    shutcalc   ()
{
  asm{
       mov al,0x8
       out 0xC,al
     }
}

void    temps      (unsigned long j)
{
  unsigned long i;
  for(i=0;i<j*10;i++);
}

static int OldInt9Seg,OldInt9Off; // adresses de l'interruption 9

void    desactint9 ()
{
  asm{
       xor ax,ax
       mov es,ax
       mov ax,es:0x26     // Segment interruption 9
       mov OldInt9Seg,ax  // stocke dans variable
       mov ax,es:0x24     // Offset int9
       mov OldInt9Off,ax  // stocke dans variable
       mov ax,es:0x03FE   // segment int FF
       mov es:0x26,ax     // remplace seg 9 par seg FF
       mov ax,es:0x03FC   // offset int FF
       mov es:0x24,ax     // remplace offset 9 par offset FF
     }
}

void    actint9    ()
{
  asm{
       xor ax,ax
       mov es,ax
       mov ax,OldInt9Seg
       mov es:0x26,ax     // remet segment d'origine
       mov ax,OldInt9Off
       mov es:0x24,ax     // remet offset d'origine
     }
}
