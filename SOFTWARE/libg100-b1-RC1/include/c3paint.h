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
 *           libg100 : c3paint.h
 *
 *			 Original Author : Casiom@x and Mastermage
 *
**************/


#ifndef __C3PAINT_H__
#define __C3PAINT_H__

#include "defines.h"

/*! \brief Affiche tous les pixels d'un sprite
 * \param x Coordonnée X
 * \param y Coordonnée Y
 * \param spr Pointeur vers le sprite
 * \param segm Segment vidéo
 *
 * Clipping mais pas de transparence sur les pixels blancs
 */
void Sprite8(int x,int y,void* spr,int segm);

/*! \brief Affiche les pixels noirs d'un sprite
 * \param x Coordonnée X
 * \param y Coordonnée Y
 * \param spr Pointeur vers le sprite
 * \param segm Segment vidéo
 *
 * Clipping et transparence sur les pixels blancs
 */
void Sprite8or(int x,int y,void* spr,int segm);

/*! \brief Affiche un sprite par inversion de pixels
 * \param x Coordonnée X
 * \param y Coordonnée Y
 * \param spr Pointeur vers le sprite
 * \param segm Segment vidéo
 *
 * Clipping et inversion de pixels
 */
void Sprite8xor(int x,int y,void* spr,int segm);

/*! \brief Affiche tous les pixels d'un sprite sans Clipping
 * \param x Coordonnée X
 * \param y Coordonnée Y
 * \param spr Pointeur vers le sprite
 * \param segm Segment vidéo
 *
 * Sans Clipping mais pas de transparence sur les pixels blancs
 */
void Sprite8_noclip(int x,int y,void* spr,int segm);

/*! \brief Affiche les pixels noirs d'un sprite sans Clipping
 * \param x Coordonnée X
 * \param y Coordonnée Y
 * \param spr Pointeur vers le sprite
 * \param segm Segment vidéo
 *
 * Sans Clipping et transparence sur les pixels blancs
 */
void Sprite8or_noclip(int x,int y,void* spr,int segm);

/*! \brief Affiche un sprite par inversion de pixels sans Clipping
 * \param x Coordonnée X
 * \param y Coordonnée Y
 * \param spr Pointeur vers le sprite
 * \param segm Segment vidéo
 *
 * Sans Clipping et inversion de pixels
 */
void Sprite8xor_noclip(int x,int y,void* spr,int segm);


#endif /* __C3PAINT_H__ */
