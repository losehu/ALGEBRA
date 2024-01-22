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
 *           libg100 : defines.h
 *
 *			 Original Author : swifter in Db-Lib, Memzones from 2072, code from various Authors
 *
**************/


#ifndef __DEFINES_H__
#define __DEFINES_H__

#define copyscreen(a,b) movedata(a,0,b,0,0x400)
#define waitkey(a,b) while (!testgroup(a,b))
#define SEG1 0x1A20
#define SEG2 0x1A60
#define SEG3 0x1AA0
#define SEG4 0x1AE0
#define SEG5 0x1B20
#define SEG6 0x1B60
#define SEG7 0x1BA0

// Memzones

#define ABS(a) (-(a) * ((a) < 0) + (a) * ((a) > 0))
#define SIGNUM(a) (-((a)<0)  + ((a)>0))

#define INT16MAX 0xFFFF

typedef unsigned char u_char;
typedef unsigned int  u_int;
typedef unsigned long  u_long;

#endif /* __DEFINES_H__ */
