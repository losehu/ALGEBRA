@ECHO OFF

REM Compilation automatisee par Mastermage version : 1-na modified for libg100


REM Verifie que les extensions soient bien activees
VERIFY OTHER 2>nul
SETLOCAL ENABLEEXTENSIONS
IF ERRORLEVEL 1 (
    REM si pas d'extensions alors on quitte avec le code d'erreur 1
    ECHO Impossible d'activer les extensions
    ECHO ON
    @EXIT /B 1
)

REM Les modifications d'environnement sont spécifiques au fichier
SETLOCAL

REM ---------------------------------------
REM -       Parametres du script          -
REM ---------------------------------------

REM ---- Chemins d'acces aux compilateurs
SET TC_PATH=C:\TC
SET DM_PATH=D:\DM

REM ---- Pour utiliser les header de libg100, par defaut libg100\ du rep courant
SET LIBG100_PATH=libg100

REM ---- Parametres specifiques aux compilateurs
SET TC_FLAGS=-1 -mt -I%LIBG100_PATH% -c
SET DM_FLAGS=-2 -ms -I%LIBG100_PATH% -Nc -c

SET RESPONSEFILE=build.arg
SET PATH=%DM_PATH%\BIN;%TC_PATH%\BIN;%PATH%

REM ---------------------------------------
REM -   Detection du type de compilateur  -
REM ---------------------------------------

SET CC=dmc
IF /I "%1" EQU "tc" set CC=tcc
IF /I "%1" EQU "tcc" set CC=tcc

REM ---------------------------------------
REM -    Nom de l'executable en sortie    -
REM ---------------------------------------

SHIFT
SET OUTPUT=%1
IF "%OUTPUT%" EQU "" SET OUTPUT=output
ECHO Fichier de sortie: %OUTPUT%.obj

REM ------------------------------------------
REM - Partie spécifique selon le compilateur -
REM ------------------------------------------

IF "%CC%" EQU "dmc" (

    REM ---------------------------------------
    REM -            Digital Mars             -
    REM ---------------------------------------
    
    ECHO Compilateur: Digital Mars
    
    SET OUTPUTFLAG=-o
    SET FLAGS=%DM_FLAGS%
	
) ELSE (

    REM ---------------------------------------
    REM -            Turbo C++                -
    REM ---------------------------------------
    
    ECHO Compilateur: Turbo C
    
    SET OUTPUTFLAG=-e
    SET FLAGS=%TC_FLAGS% -L%TC_PATH%\LIB -I%TC_PATH%\INCLUDE
)

REM ---------------------------------------
REM -    Enregistrement des parametres    -
REM ---------------------------------------

ECHO %FLAGS% %OUTPUTFLAG%%OUTPUT%.obj > %RESPONSEFILE%

REM ---------------------------------------
REM -       Fichiers a compiler           -
REM ---------------------------------------

SHIFT

IF "%1" EQU "" (

    REM si pas de parametres alors compiler *.c *cpp
    DIR /B *.c *.cpp >> %RESPONSEFILE%

) ELSE (

    REM sinon compiler les parametres
:top
	IF "%1" EQU "" GOTO suite
	ECHO Fichier[s] a traiter : %1
    DIR /B "%1" >> %RESPONSEFILE%
    SHIFT
    GOTO top

)

:suite

REM ---------------------------------------
REM -            Compilation              -
REM ---------------------------------------

%CC% @%RESPONSEFILE%

REM --------------------------------------------
REM - Suppression des fichiers intermediaires  -
REM --------------------------------------------

IF EXIST %RESPONSEFILE% DEL %RESPONSEFILE%
REM IF EXIST *.obj DEL *.obj


REM Retablissement des variables d'environnement d'origine
ENDLOCAL
REM Retablissement des extensions ou non
ENDLOCAL

ECHO ON
@REM Quitte avec le code d'erreur 0
@EXIT /B 0


