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
 *           libg100 : comm.h
 *
 *           Original : GLink by dada66
 *
 *           (C) Copyright 2003-2004 Quaranta Software 
 *
**************/


#ifndef __COMM_H__
#define __COMM_H__

#include "defines.h"

/*! \brief Definition du Time_Out
 */
#define TIME_OUT	-1

/*! \brief Macros permettant de désactiver le clavier.
 * Désactivez le clavier lors de l'envoi de bcp de données en même temps.
 * Permet un gain de temps. (Vous pouvez aussi désactiver l'int9)
 */
#define DISABLEKEYBOARD	asm in al, 0x0A asm and al, 0xF7 asm out 0x0A, al

/*! \brief Macros permettant d'activer le clavier.
 * Désactivez le clavier lors de l'envoi de bcp de données en même temps.
 * Permet un gain de temps. (Vous pouvez aussi désactiver l'int9)
 */
#define ENABLEKEYBOARD	asm in al, 0x0A asm  or al, 0x08 asm out 0x0A, al
  
/*! \brief Allume le port de la G100
 *  \param vitesse La vitesse de communication
 *
 * Vitesse : 0 =>   9600 bps
 * Vitesse : 1 =>  14400 bps
 * Vitesse : 2 =>  19200 bps
 * Vitesse : 3 =>  28800 bps
 * Vitesse : 4 =>  38400 bps
 * Vitesse : 5 =>  57600 bps
 * Vitesse : 6 =>  76800 bps
 * Vitesse : 7 => 115200 bps
 */
void InitPort(int vitesse);

/*! \brief Ferme le port de communication
 *
 * Ferme le port de communication 
 */
void ClosePort();

/*! \brief Envoi des données
 *  \param buffer Pointeur vers le buffer
 *  \param nSize Taille des données a envoyer
 *
 * Envoi des données via le port comm
 */
void Send(const void* buffer, unsigned int nSize);

/*! \brief Envoi des données
 *  \param nSegment Segment du buffer
 *  \param off_set Offset du buffer
 *  \param nSize Taille des données a envoyer
 *
 * Envoi des données via le port comm en passant des paramêtre comme en assembleur
 */
void Send_long(unsigned int nSegment, unsigned int off_set, unsigned int nSize);

/*! \brief Reçois des données avec un time_out
 *  \param buffer Pointeur vers le buffer
 *  \param nSize Taille des données a recevoir
 *  \param time_out Valeur max d'attente d'un octet, si le delai est depasse
 *  \return 1 en cas de succes, sinon une valeur negative
 *
 * Retourne Pour le moment juste TIME_OUT
 * Time_out: Valeur max d'attente d'un octet, si le delai est depasse
 * Receive retourne TIME_OUT. La valeur 500 correspond à 1 seconde.
 */
int Receive(void* buffer, unsigned int nSize, unsigned int time_out);

/*! \brief Reçois des données avec un time_out
 *  \param nSegment Segment du buffer
 *  \param off_set Offset du buffer
 *  \param nSize Taille des données a recevoir
 *  \param time_out Valeur max d'attente d'un octet, si le delai est depasse
 *  \return 1 en cas de succes, sinon une valeur negative
 *
 * Retourne Pour le moment juste TIME_OUT
 * Time_out: Valeur max d'attente d'un octet, si le delai est depasse
 * Receive retourne TIME_OUT. La valeur 500 correspond à 1 seconde.
 */
int Receive_long(unsigned int nSegment, unsigned int off_set, unsigned int nSize, unsigned int time_out);

#endif /* __COMM_H__ */
