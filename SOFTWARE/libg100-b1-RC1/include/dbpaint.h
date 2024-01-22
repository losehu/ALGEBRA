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
 *           libcg100 : dbpaint.h
 *
 *           Original Author : swifter in Db-Lib, code from various Authors
 *
**************/


#ifndef __DBPAINT_H__
#define __DBPAINT_H__

#include "defines.h"

void                 d8_or       (int x, int y, void* spr, unsigned int segm);
void                 d8_and      (int x, int y, void* spr, unsigned int segm);
void                 d8_xor      (int x, int y, void* spr, unsigned int segm);
void                 d8clip_or   (int x, int y, void* spr, unsigned int segm);
void                 d8clip_and  (int x, int y, void* spr, unsigned int segm);
void                 d8clip_xor  (int x, int y, void* spr, unsigned int segm);
void                 d16_or      (int x, int y, void* spr, unsigned int segm);
void                 d16_and     (int x, int y, void* spr, unsigned int segm);
void                 d16_xor     (int x, int y, void* spr, unsigned int segm);
void                 d16clip_or  (int x, int y, void* spr, unsigned int segm);
void                 d16clip_and (int x, int y, void* spr, unsigned int segm);
void                 d16clip_xor (int x, int y, void* spr, unsigned int segm);

void                 bvline      (unsigned int x, unsigned int ymin, unsigned int ymax, 
                                  unsigned int segm);
void                 wvline      (unsigned int x, unsigned int ymin, unsigned int ymax, 
                                  unsigned int segm);
void                 gvline      (unsigned int x, unsigned int ymin, unsigned int ymax, 
                                  unsigned int segm);
void                 invvline    (unsigned int x, unsigned int ymin, unsigned int ymax, 
                                  unsigned int segm);
void                 vline       (unsigned int x, unsigned int ymin, unsigned int ymax, 
                                  unsigned char couleur, unsigned int segm);
void                 bhline      (unsigned int xmin, unsigned int xmax, unsigned int y, 
                                  unsigned int segm);
void                 whline      (unsigned int xmin, unsigned int xmax, unsigned int y, 
                                  unsigned int segm);
void                 ghline      (unsigned int xmin, unsigned int xmax, unsigned int y, 
                                  unsigned int segm);
void                 invhline    (unsigned int xmin, unsigned int xmax, unsigned int y, 
                                  unsigned int segm);
void                 hline       (unsigned int xmin, unsigned int xmax, unsigned int y, 
                                  unsigned char couleur, unsigned int segm);

void                 bpxl        (unsigned int x, unsigned char y, unsigned int segm);
void                 wpxl        (unsigned int x, unsigned char y, unsigned int segm);
void                 gpxl        (unsigned int x, unsigned char y, unsigned int segm);
void                 invpxl      (unsigned int x, unsigned char y, unsigned int segm);
unsigned char        pxltest     (unsigned int x, unsigned char y, unsigned int segm);
void                 pxl         (unsigned int x, unsigned char y, unsigned char couleur, 
                                  unsigned int segm);

#endif /* __DBPAINT_H__ */
