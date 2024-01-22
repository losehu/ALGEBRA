===========================================================
====== libG100 ============================================
==================== Complete Library for Afx2/Graph100 ===
===========================================================
==== Readme ===============================================
___________________________________________________________

	I. La librairie
	
  La libg100 est consitu�e des fonctions les plus utilis�es et les
  plus devellop�es dans le monde Casio !
  
  Toute les fonctions sont 100% fran�aises, mais ont des noms
  compatible avec une internationalisation !
  
  
    II. Compilation
    
  Cette librairie est faite pour compiler avec 2 compilateurs :
  		Borland Turbo C++ 3 (surement Borland C++ 4 ou 5)
  		et Digital Mars
  		
  Pour la compiler, il faudra correctement mettre les bons
  chemins d'acces aux compilateurs dans "makefile-tc" et 
  "makefile-dm" pour Turbo C et Digital Mars respectivement.
  
  Si vous ne voulez compiler pour qu'un compilateur, ouvrez
  make-project.bat, et mettez a la place de "make" :
  soit "make -f makefile-dm" soit "make -f makefile-tc"
  
  Ensuite lancez make-project.bat
  
  Les erreurs de compilation seront dans le fichier :
  "Resultat-Compilation.txt"
  
  
    III. Utilisation
    
  L'utilisation d'une librarie est tr�s simple, il suffit
  de passer le nom de la librairie en parametre en plus
  de votre fichier .c / .cpp a votre compilateur.
  
  Par exemple, pour compiler avec Turbo C :
  $ tcc -1 -mt truc.c tccg100.lib
  ou Digital Mars :
  $ dmc -2 -2 -ms -Nc truc.c dmcg100.lib
  
  Au niveau des headers (.h), il faut absolument que le compilateur
  puisse les trouver :
  1. Dans vos fichiers .c sous forme `#include "screen.h"`
  2. Dans le m�me r�pertoire que le fichier .c
  
  Il existe une m�thode plus simple, allez jeter un coup d'oeil
  dans le r�pertoire "examples" ou il y a des examples de compilation.
  
    IV. Distribution
    
  Cette librairie est distribu�e en LGPL, lisez LGPL.txt pour an savoir plus
  
  Selon la LGPL, un programme qui utilise la lig100 ou un d�riv� doit rester
  100% compilable avec la libg100, c'est a dire que ce programme devra
  fournir les modification faites a la libg100, soit en code source
  soit en fichier objet suppl�mentaire.
  
    V. Remerciements
    
  Nous, Communaut� Graph100, remercieons tout les devellopeurs qui
  ont particip� en autorisant la diffusion de leur code sous forme
  de librairie, Merci !!
  
  Site graph100 : http://www.graph100.com


=====================================================================

    Libg100 - The Complete Graph100 API - http://libg100.sf.net
    Copyright (C) 2004-2005  Graph100/Afx2 Devellopers

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
