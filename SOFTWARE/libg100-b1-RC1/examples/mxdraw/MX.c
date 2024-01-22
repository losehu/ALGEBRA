#include <stdio.h>
#include <c3paint.h>
#include <screen.h>


unsigned char Sprite[][8] = {
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},	//Sprite 0
{0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA},	//Sprite 1
};

void main(void)
{
	short seg;
	seg = getvideoseg();
	clear(seg);
	
	Sprite8or_noclip( 0   , 0  , Sprite[0], seg);
	Sprite8or_noclip( 120 , 30 , Sprite[1], seg);
	Sprite8or_noclip( 83  , 0  , Sprite[0], seg);
	Sprite8or_noclip( 15  , 30 , Sprite[1], seg);


	getch();
	getch();
	
	clear(seg);
	
	asm {
		mov ax,4C00h
		int    21h
	}

}
