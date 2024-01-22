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
 *           libg100 : screen.h
 *
 *			 Original Author : swifter in Db-Lib, code from various Authors
 *
**************/


#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "defines.h"

/*! \brief Type d'images
 */
typedef unsigned char BMPTabType[1024];

/*! \brief Retourn e segment vidéo
 *  \return Segment vidéo
 *
 *  Et le place dans ES
 */
short getvideoseg(void);

/*! \brief Change le mode d'affichage
 *  \param modetype Code Hexa du mode
 *
 *   Modes : 0xC3, 0xD3, 0xCB, 0xDB
 */
void             mode                  (unsigned char modetype);

/*! \brief  Active le mode DB en modifiant le contraste
 */
unsigned char    db_on                 ();

/*! \brief Desactive le mode DB en modifiant le contraste
 */
void             db_off                ();


/*! \brief  Affiche une image fixe codée en format d'affichage courant
 *  \param BmpTab Pointeur vers l'image
 *  \param segm Segment d'affichage
 * Attention, l'image doit être dans le codage correspondat au mode d'affichage
 */
void             disp_bmp              (BMPTabType BmpTab, unsigned int segm);


/*! \brief  Efface une page de l'écran
 *  \param segm Segment d'affichage
 */
void             clear                 (unsigned int segm);

/*! \brief Efface 3 pages de l'écran
 *  \param segm Segment d'affichage
 *
 * Utile pour le mode gris
 */
void             clear3                (unsigned int segm);

/*! \brief Change la page d'affichage
 *  \param segm Segment d'affichage
 */
void             movevideo             (unsigned int segm);

/*! \brief Inverse la page binairement
 *  \param segm Segment d'affichage
 */
void             invscreen             (unsigned int segm);

/*! \brief Change le contraste
 *  \param niv Niveau de contraste
 */
void             contrast              (unsigned char niv);

/*! \brief Déplace le contraste
 *  \param sens Sens de déplacement
 *  \param niv Niveau de contraste
 */
void             contrast_chg          (unsigned char sens, unsigned char niv);


/*! \brief Déplace la page vers la droite
 *  \param x Offset de déplacement
 *  \param srcsegm Page source
 *  \param segm Segment d'affichage
 */
void             rightscroll           (unsigned int x, unsigned int srcsegm, unsigned int segm);

/*! \brief Déplace la page vers la gauche
 *  \param x Offset de déplacement
 *  \param srcsegm Page source
 *  \param segm Segment d'affichage
 */
void             leftscroll            (unsigned int x, unsigned int srcsegm, unsigned int segm);

/*! \brief Déplace la page vers le haut
 *  \param y Offset de déplacement
 *  \param srcsegm Page source
 *  \param segm Segment d'affichage
 */
void             upscroll              (unsigned int y, unsigned int srcsegm, unsigned int segm);

/*! \brief Déplace la page vers le bas
 *  \param y Offset de déplacement
 *  \param srcsegm Page source
 *  \param segm Segment d'affichage
 */
void             downscroll            (unsigned int y, unsigned int srcsegm, unsigned int segm);

#endif /* __SCREEN_H__ */
