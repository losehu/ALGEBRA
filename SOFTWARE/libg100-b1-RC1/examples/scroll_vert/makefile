################################################################
#  Makefile for programs using libg100 library  		       #
#              uses compile.cmd & build.cmd by Mastermage	   #
#   libg100.sf.net		- distributed in LGPL				   #
################################################################

########################### Configuration a modifier ###################

#Nom du compilo avec lequel compiler (mettre # devant 1 des 2 compilos pour l'enlever)
#(le path des compilos doivent etre regles dans build.cmd et compile.cmd)
COMPILO = dm
#COMPILO = tc

##Liste des fichiers objets a compiler
PXL_OBJ 	 = vt.obj

##Liste des fichiers sur qui depends la compilation
PXL_DEP 	 = vt.c


######################### Configuration a ne pas modifier #############
# La lib doit etre au même endroit que les sources ou dans le rep lib\ du compilo
LIB_NAME = $(COMPILO)cg100.lib
#fichier compile.cmd
COMPILE = compile.cmd $(COMPILO) $* $*.c
#fichier build.cmd
BUILD = build.cmd $(COMPILO) $* $(LIB_NAME)

#################### Makefile a Modifier ########################

### Changer le nom de l'executable voulu
all: vt.exe

### Ici aussi avec les objets qui doivent être mis dans l'exe (les même sur les 2 lignes)
vt.exe	   : $(PXL_OBJ)
	$(BUILD) $(PXL_OBJ)

### Ici on indique comment on construit les objets, l'objet doit avoir le même nom que le fichier .c (obj a la place de c)
#### Ne pas toucher a la seconde ligne, copier autant de fois qu'il y a d'objet a compiler
#### ça correspond généralement au nombre de .c (ou .cpp) a compiler !
$(PXL_OBJ)  : $(PXL_DEP)
	 $(COMPILE)

