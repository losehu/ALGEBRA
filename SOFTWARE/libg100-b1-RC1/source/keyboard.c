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
 *           libcg100 : keyboard.c
 *
**************/

#include "keyboard.h"

unsigned char testkey   (unsigned char x, unsigned char y)
{
  asm{
       mov ax,0x1
       mov bl,al
       mov cl,y
       shl ax,cl
       mov cl,x
       inc cl
       ror bl,cl
       out 0x13,ax
       mov ch,0xA
     }
  loop:
  asm{
       dec ch
       jne loop
       in al,0x13
       and al,bl
     }
  if (_AL) return 1;
  return 0;
}

unsigned char testgroup (unsigned char group, unsigned int y)
{
  asm{
       mov ax,y
       mov dl,group
       out 0x13,ax
       mov cx,0xA
     }
  loop:
  asm{
       dec cx
       jne loop
       in al,0x13
       and al,dl
     }
  if(_AL) return 1;
  return 0;
}

void          emptykey  ()
{
  do
  {
    while (testgroup(0x7F,0x7FF)); // On teste toute les touches.
  } while (testgroup(0x7F,0x7FF)); // On refait le test pour eviter un bug bizarre.
}

