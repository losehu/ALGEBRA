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
//				 Fichier d'entete a inclure dans les fichiers contenant				 //
//                des fonctions qui utilisent des fonctions de la lib                //
//											                                         //
//*  Version 1.4b										   (c) Julien PATTE - 2004  *//
 /////////////////////////////////////////////////////////////////////////////////////


#if !defined __DRAWLIB
#define __DRAWLIB

typedef unsigned char uchar;


/*! \brief Segment du premier buffer
 * def: 0x1A60
 */
#define DRAW_BUFFER1 0x1A60

/*! \brief Espace entre les débuts de chaque buffer
 * def: 2048 => segment 0x1AE0
 */
#define DRAW_JUMP    2048

/*! \brief Limites (en pixels) de clipping gauche
 * (0 <= x <= 127)
 */
#define DRAW_CLIPG   0

/*! \brief Limites (en pixels) de clipping droite
 * (1 <= x <= 128)
 */
#define DRAW_CLIPD   128

/*! \brief Limites (en pixels) de clipping haut
 * (0 <= x <= 63)
 */
#define DRAW_CLIPH   0

/*! \brief Limites (en pixels) de clipping bas
 * (1 <= x <= 64)
 */
#define DRAW_CLIPB   64

///////////////////////////////////////////////////////////////////////////////////////

/* Fonctions à compiler
 * NE SUPPRIMEZ PAS CES 5 MACROS,
 * mettez en commentaires les define correspondants aux fonctions dont vous n'avez pas besoin.
*/
#define FUNC_DRAWSPR
#define FUNC_SIZESPRITE
#define FUNC_DRAW_NOMASK_NOCLIP
#define FUNC_DRAW_MASK_NOCLIP
#define FUNC_DRAW_NOMASK_CLIP
#define FUNC_DRAW_MASK_CLIP

///////////////////////////////////////////////////////////////////////////////////////

/* Macros à passer comme 4eme argument de drawspr pour spécifier le type d'affichage  */
#define MODE_NOMASK_NOCLIP 0
#define MODE_MASK_NOCLIP   1
#define MODE_NOMASK_CLIP   2
#define MODE_MASK_CLIP     3

///////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Fonction principale de la lib (à utiliser en priorité)
 *  \param x Pos X
 *  \param y Pos Y
 *  \param spr Pointeur far du sprite
 *  \param mode Mode de dessin
 *
 * Affiche un sprite de taille variable avec un mode :
 * MODE_NOMASK_NOCLIP,
 * MODE_MASK_NOCLIP,
 * MODE_NOMASK_CLIP,
 * MODE_MASK_CLIP
 */
void drawspr(int x, char y, void far* spr, uchar mode);
	
/*! \brief Fonction NOMASK_NOCLIP
 *  \param x Pos X
 *  \param y Pos Y
 *  \param spr Pointeur far du sprite
 *
 * Affiche un sprite de taille variable avec un mode :
 * MODE_NOMASK_NOCLIP
 */
void draw_noM_noC (int x,char y,void far* spr);

/*! \brief Fonction MODE_MASK_NOCLIP
 *  \param x Pos X
 *  \param y Pos Y
 *  \param spr Pointeur far du sprite
 *
 * Affiche un sprite de taille variable avec un mode :
 * MODE_MASK_NOCLIP
 */
void draw_M_noC	  (int x,char y,void far* spr);

/*! \brief Fonction MODE_NOMASK_CLIP
 *  \param x Pos X
 *  \param y Pos Y
 *  \param spr Pointeur far du sprite
 *
 * Affiche un sprite de taille variable avec un mode :
 * MODE_NOMASK_CLIP
 */
void draw_noM_C   (int x,char y,void far* spr);

/*! \brief Fonction MODE_MASK_CLIP
 *  \param x Pos X
 *  \param y Pos Y
 *  \param spr Pointeur far du sprite
 *
 * Affiche un sprite de taille variable avec un mode :
 * MODE_MASK_CLIP
 */
void draw_M_C     (int x,char y,void far* spr);

/*! \brief Détermine le nombre d'octets necessaires pour coder un sprite
 *  \param Haut Hauteur du sprite
 *  \param Larg Largeur du sprite
 *  \return Nombre d'octets necessaires pour coder un sprite
 *
 * Avec une hauteur et largeur définie.
 */
int sizesprite(uchar Haut,uchar Larg);

/*! \brief Macro MODE_MASK_CLIP (Version précédente)
 *  \param a Pos X
 *  \param b Pos Y
 *  \param c Pointeur far du sprite
 *
 * Affiche un sprite de taille variable avec un mode :
 * MODE_MASK_CLIP
 * Compatibilité avec Version précédente
 */
#define drawsprite(a,b,c) draw_M_C(a,b,c)

#ifdef __cplusplus
}
#endif


///////////////////////////////////////////////////////////////////////////////////////

#endif
