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
D8_OBJ		 = d8.obj
D8_CLIP_OBJ	 = d8clip.obj
D16_OBJ		 = d16.obj
D16_CLIP_OBJ = d16clip.obj

##Liste des fichiers sur qui depends la compilation
D8_DEP		 = d8.c
D8_CLIP_DEP  = d8clip.c
D16_DEP		 = d16.c
D16_CLIP_DEP = d16clip.c


######################### Configuration a ne pas modifier #############
# La lib doit etre au même endroit que les sources ou dans le rep lib\ du compilo
LIB_NAME = $(COMPILO)cg100.lib
#fichier compile.cmd
COMPILE = compile.cmd $(COMPILO) $* $*.c
#fichier build.cmd
BUILD = build.cmd $(COMPILO) $* $(LIB_NAME)

#################### Makefile a Modifier ########################

### Changer le nom de l'executable voulu
all: d8.exe d8clip.exe d16.exe d16clip.exe

### Ici aussi avec les objets qui doivent être mis dans l'exe (les même sur les 2 lignes)
d8.exe	    : $(D8_OBJ)
	$(BUILD) $(D8_OBJ)
d8clip.exe	: $(D8_CLIP_OBJ)
	$(BUILD) $(D8_CLIP_OBJ)
d16.exe	    : $(D16_OBJ)
	$(BUILD) $(D16_OBJ)
d16clip.exe : $(D16_CLIP_OBJ)
	$(BUILD) $(D16_CLIP_OBJ)

### Ici on indique comment on construit les objets, l'objet doit avoir le même nom que le fichier .c (obj a la place de c)
#### Ne pas toucher a la seconde ligne, copier autant de fois qu'il y a d'objet a compiler
#### ça correspond généralement au nombre de .c (ou .cpp) a compiler !
$(D8_OBJ)  : $(D8_DEP)
	$(COMPILE)
$(D8_CLIP_OBJ)  : $(D8_CLIP_DEP)
	$(COMPILE)
$(D16_OBJ)  : $(D16_DEP)
	$(COMPILE)
$(D16_CLIP_OBJ)  : $(D16_CLIP_DEP)
	$(COMPILE)

