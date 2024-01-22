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
//*                          <gxLib pour CASIO Graph100>                            *//
//                                                                                   //
//          Fichier d'entete a inclure pour utiliser les fonctions de la lib         //
//                                                                                   //
//*  Version 1.0                                           (c) Julien PATTE - 2004  *//
 /////////////////////////////////////////////////////////////////////////////////////

#ifndef GX_LIB
#define GX_LIB

// screen 1   1 10 01 10 0   (facteur 1)
// screen 2   1 11 10 00 0   (facteur 2)
// screen 3   1 01 01 01 0   (facteur 1)
//			  | \/ \/ \/ |
// intensite  4  3  2  1 0	 /4
// couleur    B DG MG LG W

// idealement la combinaison '100' devrait etre aussi utilisee pour le gris clair, mais elle
// servira plutot pour le masque dans les fonctions gxSpr_M_C et gxSpr_M_noC.

///////////////////////////////////////////////////////////////////////////////////////
// Compilateurs

typedef unsigned char uchar;
typedef unsigned int uint;

#ifdef __TURBOC__		// compilation avec TurboC 3.0
typedef uchar bool;
#define true 1
#define false 0
#define LBL(a) } a: asm {

#elif __DMC__			// compilation avec Digital Mars
#define interrupt _interrupt
#ifndef __cplusplus
typedef uchar bool;
#define true 1
#define false 0
#endif
#define LBL(a) } a: asm {

//#elif __WATCOMC__		// compilation avec Open Watcom
//typedef uchar bool;
//#define true 1
//#define false 0
//#define asm __asm
//#define LBL(a) a:
#endif

///////////////////////////////////////////////////////////////////////////////////////
// Constantes des couleurs   (ne pas modifier ces valeurs)

#define GX_WHITE				0
#define GX_LIGHTGRAY			1
#define GX_MIDGRAY				2
#define GX_DARKGRAY				3
#define GX_BLACK				4
#define GX_INVERSE				5

///////////////////////////////////////////////////////////////////////////////////////
// Mode d'affichage des sprites   (ne pas modifier ces valeurs)

#define GX_SPR_MASK_CLIP		0
#define GX_SPR_NOMASK_CLIP		1
#define GX_SPR_MASK_NOCLIP		2
#define GX_SPR_NOMASK_NOCLIP	3

///////////////////////////////////////////////////////////////////////////////////////
// Contraste par defaut et cadre de clipping

#define GX_CONT_G100			20
#define GX_CONT_G100PLUS		21

#define GX_CLIPG				0	// (0 <= x <= 127)
#define GX_CLIPD				128	// (1 <= x <= 128)
#define GX_CLIPH				0	// (0 <= x <= 63)
#define GX_CLIPB				64	// (1 <= x <= 64)

///////////////////////////////////////////////////////////////////////////////////////
// Choix des fonctions a compiler dans le projet

#define FUNC_GX_REFRESH
#define FUNC_GX_DRAWBMP

#define FUNC_GX_PIXEL
#define FUNC_GX_PIXW
#define FUNC_GX_PIXLG
#define FUNC_GX_PIXMG
#define FUNC_GX_PIXDG
#define FUNC_GX_PIXB
#define FUNC_GX_PIXINV
#define FUNC_GX_PIXTEST

#define FUNC_GX_VLINE
#define FUNC_GX_VLINEW
#define FUNC_GX_VLINELG
#define FUNC_GX_VLINEMG
#define FUNC_GX_VLINEDG
#define FUNC_GX_VLINEB
#define FUNC_GX_VLINEINV

#define FUNC_GX_HLINE
#define FUNC_GX_HLINEW
#define FUNC_GX_HLINELG
#define FUNC_GX_HLINEMG
#define FUNC_GX_HLINEDG
#define FUNC_GX_HLINEB
#define FUNC_GX_HLINEINV

#define FUNC_GX_FILL
#define FUNC_GX_FILLW
#define FUNC_GX_FILLLG
#define FUNC_GX_FILLMG
#define FUNC_GX_FILLDG
#define FUNC_GX_FILLB
#define FUNC_GX_INVERSE

#define FUNC_GX_SPRITE
#define FUNC_GX_SPR_M_C
#define FUNC_GX_SPR_NOM_C
#define FUNC_GX_SPR_M_NOC
#define FUNC_GX_SPR_NOM_NOC

///////////////////////////////////////////////////////////////////////////////////////
// Declarations

#ifdef __cplusplus
extern "C" {
#endif

void gxSetMode(bool enabled);
#define gxClearBuff gxFillW
void gxRefresh();
void gxDrawBmp(void far* bmp);

void gxPixel    (uint x, uint y, uchar coul);
void gxPixB     (uint x, uint y);
void gxPixW     (uint x, uint y);
void gxPixLG    (uint x, uint y);
void gxPixMG    (uint x, uint y);
void gxPixDG    (uint x, uint y);
void gxPixInv   (uint x,uint y);
uchar gxPixTest (uint x,uint y);

void gxVLine    (uint x, uint ymin, uint ymax, uchar coul);
void gxVLineB   (uint x, uint ymin, uint ymax);
void gxVLineW   (uint x, uint ymin, uint ymax);
void gxVLineLG  (uint x, uint ymin, uint ymax);
void gxVLineMG  (uint x, uint ymin, uint ymax);
void gxVLineDG  (uint x, uint ymin, uint ymax);
void gxVLineInv (uint x, uint ymin, uint ymax);

void gxHLine    (uint xmin, uint xmax, uint y, uchar coul);
void gxHLineB   (uint xmin, uint xmax, uint y);
void gxHLineW   (uint xmin, uint xmax, uint y);
void gxHLineLG  (uint xmin, uint xmax, uint y);
void gxHLineMG  (uint xmin, uint xmax, uint y);
void gxHLineDG  (uint xmin, uint xmax, uint y);
void gxHLineInv (uint xmin, uint xmax, uint y);

void gxFill     (uchar coul);
void gxFillW    ();
void gxFillLG   ();
void gxFillMG   ();
void gxFillDG   ();
void gxFillB    ();
void gxInverse  ();

void gxSprite      (int x, int y, void far* spr, uchar mode);
void gxSpr_M_C     (int x, int y, void far* spr);
void gxSpr_noM_C   (int x, int y, void far* spr);
void gxSpr_M_noC   (int x, int y, void far* spr);
void gxSpr_noM_noC (int x, int y, void far* spr);

#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////////////////
// Pointeurs vers les segments de l'ecran et du buffer

extern const void far* gxScreen1;
extern const void far* gxScreen2;
extern const void far* gxScreen3;
extern const void far* gxBuffer1;
extern const void far* gxBuffer2;
extern const void far* gxBuffer3;

///////////////////////////////////////////////////////////////////////////////////////

#endif
