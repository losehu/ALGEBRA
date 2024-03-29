#include <screen.h>
#include <dbpaint.h>
#include <calc.h>
#include <keyboard.h>

// les 2 images de 2 couches chacune (pour le gris)
unsigned char swfpict[4][1024]={{
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xC0,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xC0,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xC0,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xE0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xE0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xE0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xF0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xF9,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0xFF,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0xFF,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xC0,0xFF,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xC0,0xEF,0x3,0xE0,0x7F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xE0,0xE7,0x3,0xF8,0xFF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xFC,0xFF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xFE,0xFF,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x7F,0xFC,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xFC,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x80,0xFF,0xFF,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xC0,0xFF,0xFF,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xC0,0xFF,0xFF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xC0,0xFF,0xFF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xC0,0x7,0x7E,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xC0,0xF,0x7F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xC0,0xFF,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x80,0xFF,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xFF,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xFE,0x3,0xE0,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xE0,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x80,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x80,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xC0,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xC0,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xC0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xE0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xFC,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xFE,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xFF,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xFF,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF8,0x3,0x80,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF8,0x3,0x80,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF8,0x0,0xC0,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x3C,0x0,0xC0,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xC,0x0,0xE0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF8,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFF,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0},{
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFF,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0xFF,0x0,0xF0,0x7,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0x7F,0x0,0xF0,0x7,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0x7F,0x0,0xF8,0x3,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x1F,0x0,0xF8,0x3,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF8,0x3,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF8,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7E,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7E,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7E,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x3F,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x3F,0x80,0x1F,0xFC,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x1F,0xC0,0x1F,0xFE,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x1F,0xC0,0x1F,0xFF,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x1F,0xE0,0x1F,0xFF,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x9F,0xFF,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF8,0xDF,0xFF,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF8,0xFF,0xFF,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0xFF,0xFF,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7E,0xFF,0xFB,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3E,0xFF,0xF9,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3F,0xFF,0xF9,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x1F,0xFF,0xF8,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0xF,0x7F,0xF8,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0xF,0x7F,0xF8,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0x7,0x3F,0xF8,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0xFF,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0xFF,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0xFF,0x3,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFF,0xFF,0x3,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x7F,0xF8,0x3,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x3F,0xF8,0x3,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x3F,0xF8,0x3,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x7F,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0xFF,0x7,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0xFF,0x1F,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x3F,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFF,0x7F,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7F,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0xE3,0x7F,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x83,0x7F,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0xC7,0x3F,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0xFF,0x1F,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xFF,0xF,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xFF,0x3,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},{
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xFF,0xFF,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xF8,0x3F,0xE0,0x7F,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x80,0x3F,0xF0,0xFF,0xFF,0x7,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xE0,0x3,0xFF,0x1,0x80,0x1F,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x3F,0x0,0x0,0x1E,0xC0,0x1F,0x0,0xC0,0xFF,0x1,0x0,0x0,0x0,0x0,
0x0,0x80,0x3F,0x0,0x80,0x7,0xF0,0x0,0x0,0xFC,0xF0,0x7,0x0,0x0,0x0,0x0,
0x0,0x80,0x1F,0x0,0xF0,0x0,0x3F,0x0,0x80,0x7,0x3C,0x1E,0x0,0x0,0x0,0x0,
0x0,0xC0,0x1F,0x0,0x38,0x80,0xF,0x0,0xF8,0x0,0xE,0x7C,0x0,0x0,0x0,0x0,
0x0,0xC0,0x1F,0x0,0x6,0xE0,0x3,0x0,0x3C,0x80,0x3,0xF8,0x0,0x0,0x0,0x0,
0x0,0xC0,0x1F,0x80,0x3,0x70,0x0,0x0,0xF,0x80,0x7,0xD8,0x3,0x0,0x0,0x0,
0x0,0xE0,0xF,0xC0,0x0,0x3C,0x0,0xC0,0x7,0x80,0x7,0xD8,0xE,0x0,0x0,0x0,
0x0,0xE0,0xF,0x20,0x0,0xF,0x0,0xF0,0x1,0xC0,0xF,0x98,0x1C,0x0,0x0,0x0,
0x0,0xE0,0xF,0x18,0x80,0x3,0x0,0x78,0x0,0xE0,0xF,0x18,0x31,0x0,0x0,0x0,
0x0,0xF0,0xF,0xC,0xE0,0x1,0x0,0x1E,0x0,0xF0,0xF,0x18,0xE2,0x0,0x0,0x0,
0x0,0xF9,0x7,0x2,0x78,0x0,0x0,0xF,0x0,0xF8,0xF,0x18,0xC6,0x1,0x0,0x0,
0x80,0xFF,0x7,0x1,0x1C,0x0,0x80,0x3,0x0,0xFE,0x7,0x18,0x84,0x3,0x0,0x0,
0x80,0xFF,0xC7,0x0,0xE,0x0,0xC0,0x1,0xF0,0xFF,0x7,0x18,0x8C,0x7,0x0,0x0,
0xC0,0xFF,0x67,0x0,0x7,0x0,0xE0,0x0,0xE0,0xFF,0x7,0x8,0xC,0x1F,0x0,0x0,
0xC0,0xEF,0x33,0xE0,0x7F,0x0,0x70,0x80,0xFF,0xFF,0x7,0x8,0x18,0x36,0x0,0x0,
0xE0,0xE7,0x1B,0xF8,0xFF,0x0,0x38,0xC0,0xFF,0xFF,0x3,0x8,0x18,0x6C,0x0,0x0,
0x0,0x0,0xC,0xFC,0xFF,0x0,0x1C,0xF0,0xFF,0xFF,0x3,0x8,0x10,0xD8,0x0,0x0,
0x0,0x0,0x6,0xFE,0xFF,0x1,0xE,0xFE,0xFF,0xFF,0x3,0xC,0x30,0xB8,0x1,0x0,
0x0,0x0,0x3,0x7F,0xFC,0x1,0x7,0xFE,0xFF,0xFF,0x1,0xC,0x20,0x30,0x3,0x0,
0x0,0x80,0x1,0x8,0xFC,0x81,0x83,0xFF,0xFF,0xFF,0x0,0x4,0x20,0x70,0x7,0x0,
0x0,0x80,0x80,0xFF,0xFF,0xC1,0xF9,0xFF,0xFF,0xFF,0x0,0x6,0x20,0x60,0x6,0x0,
0x0,0x40,0xC0,0xFF,0xFF,0xC1,0xFF,0xFF,0xFF,0xFF,0x0,0x6,0x40,0xE0,0xC,0x0,
0x0,0x60,0xC0,0xFF,0xFF,0xE0,0xFF,0xFF,0xFF,0xFF,0x0,0x6,0x40,0xC0,0xC,0x0,
0x0,0x30,0xC0,0xFF,0xFF,0xF0,0xFF,0xFF,0xFF,0xFF,0x0,0x6,0x40,0xC0,0x19,0x0,
0x0,0x10,0xC0,0x7,0x7E,0xF8,0xFF,0xFF,0xFF,0x7F,0x0,0x2,0x40,0x80,0x31,0x0,
0x0,0x18,0xC0,0xF,0x7F,0xF8,0xFF,0xFF,0xFF,0x7F,0x0,0x3,0xC0,0x80,0x21,0x0,
0x0,0x8,0xC0,0xFF,0x3F,0xFC,0xFF,0xFF,0xFF,0x7F,0x0,0x3,0xC0,0x0,0x63,0x0,
0x0,0xC,0xA0,0xFF,0x1F,0xFE,0xFF,0xFF,0xFF,0x3F,0x0,0x3,0xC0,0x0,0x43,0x0,
0x0,0x6,0x30,0xFF,0xF,0xFF,0xFF,0xFF,0xFF,0x3F,0x0,0x1,0xC0,0x0,0xC2,0x0,
0x0,0x6,0x10,0xFE,0xE3,0xFF,0xFF,0xFF,0xFF,0x1F,0x80,0x1,0xC0,0x1,0x86,0x0,
0x0,0x3,0x8,0x0,0xF0,0xFF,0xFF,0xFF,0xFF,0x1F,0x80,0x1,0x80,0x1,0x86,0x1,
0x0,0x3,0xC,0x0,0xF8,0xFF,0xFF,0xFF,0xFF,0xF,0x80,0x1,0x80,0x1,0xC,0x1,
0x80,0x1,0x4,0x0,0xF8,0xFF,0xFF,0xFF,0xFF,0xF,0x80,0x1,0x80,0x1,0xC,0x3,
0x80,0x1,0x6,0x0,0xFC,0xFF,0xFF,0xFF,0xFF,0x7,0x80,0x0,0x80,0x1,0x8,0x3,
0x80,0x1,0x2,0x0,0xFC,0xFF,0xFF,0xFF,0xFF,0xF,0xC0,0x0,0x80,0x1,0x18,0x6,
0xC0,0x0,0x3,0x0,0xFC,0xFF,0xFF,0xFF,0xFF,0xF,0xC0,0x0,0x80,0x1,0x18,0x6,
0xC0,0x0,0x1,0x0,0xF8,0xFF,0xFF,0xFF,0xFF,0x1F,0xC0,0x0,0x80,0x1,0x10,0xC,
0xC0,0x7,0x1,0x0,0xF8,0xFF,0xFF,0xFF,0xFF,0x1F,0xC0,0x0,0x80,0x1,0x30,0xC,
0xE0,0x87,0x1,0x0,0xF8,0xFF,0xFF,0xFF,0xFF,0x3F,0x60,0x0,0x80,0x1,0x20,0x18,
0xE0,0x87,0x0,0x0,0xE8,0xFF,0xFF,0xFF,0xFF,0x7F,0x60,0x0,0x80,0x1,0x60,0x18,
0xE0,0xCF,0x0,0x0,0x8C,0xFF,0xFF,0xFF,0xFF,0xFF,0x70,0x0,0x80,0x1,0x60,0x18,
0xE0,0xCF,0x0,0x0,0x84,0xFF,0xFF,0xFF,0xFF,0xFF,0x30,0x0,0x80,0x1,0x60,0x10,
0xF0,0xE7,0x0,0x0,0x6,0xFF,0xFF,0xFF,0xFF,0x7F,0x30,0x0,0x80,0x1,0x60,0x10,
0xF0,0x67,0x0,0x0,0x2,0xFF,0xFF,0xFF,0xFF,0x7F,0x38,0x0,0x80,0x1,0x60,0x30,
0xF0,0x67,0x0,0x0,0x2,0xFF,0xFF,0xFF,0xFF,0xFF,0x18,0x0,0x80,0x1,0x60,0x30,
0xF0,0x7F,0x0,0x0,0x3,0xF0,0xFF,0xFF,0xFF,0xFF,0x1C,0x0,0x80,0x1,0x60,0x20,
0xF0,0x3F,0x0,0x0,0x1,0xF8,0xFF,0xFF,0xFF,0xFF,0x1C,0x0,0xC0,0x0,0x60,0x20,
0xF0,0x3F,0x0,0x80,0x1,0xF8,0xFF,0xFF,0xFF,0xFF,0xD,0x0,0xC0,0x0,0x60,0x20,
0xF8,0x37,0x0,0x80,0x0,0xF8,0xFF,0xFF,0xFF,0xFF,0xF,0x0,0xC0,0x0,0x60,0x20,
0xF8,0x3B,0x0,0x80,0x0,0xFC,0xFF,0xFF,0xFF,0xFF,0xF,0x0,0xC0,0x0,0x40,0x60,
0xF8,0x1B,0x0,0x40,0x0,0xFC,0xFF,0xFF,0xFF,0xFF,0x7,0x0,0xC0,0x0,0xC0,0x60,
0xF8,0x1B,0x0,0x40,0x0,0xE0,0xFF,0xFF,0xFF,0xFF,0x7,0x0,0x40,0x0,0xC0,0x40,
0xF8,0x19,0x0,0x40,0x0,0xC0,0xFF,0xFF,0xFF,0xFF,0x3,0x0,0x40,0x0,0xC0,0x40,
0xF8,0x1D,0x0,0x20,0x0,0xC0,0xFF,0xFF,0xFF,0xFF,0x3,0x0,0x60,0x0,0xC0,0x40,
0xFC,0xC,0x0,0x20,0x0,0x80,0xFF,0xFF,0xFF,0xFF,0x3,0x0,0x20,0x0,0xC0,0x40,
0x7C,0xC,0x0,0x30,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0x3,0x0,0x20,0x0,0x40,0x40,
0x3C,0xC,0x0,0x30,0x0,0x0,0xFC,0xFF,0xFF,0xFF,0x1,0x0,0x20,0x0,0x40,0x40,
0x1C,0xC,0x0,0x10,0x0,0x0,0xFC,0xFF,0xFF,0xFF,0x0,0x0,0x20,0x0,0x40,0x40},{
0xC,0xC,0x0,0x18,0x0,0x0,0xF6,0xFF,0xFF,0xFF,0x0,0x0,0x10,0x0,0x40,0x40,
0xC,0xC,0x0,0x18,0x0,0x0,0xF6,0xFF,0xFF,0x7F,0x0,0x0,0x10,0x0,0x40,0x40,
0xC,0xC,0x0,0x18,0x0,0x0,0xE2,0xFF,0xFF,0x7F,0x0,0x0,0x18,0x0,0x40,0x40,
0x8,0xC,0x0,0x18,0x0,0x0,0xC3,0xFF,0xFF,0x73,0x0,0x0,0x8,0x0,0x20,0x40,
0x8,0xC,0x0,0x8,0x0,0x0,0xC1,0xFF,0xFF,0xF3,0x7,0x0,0xC,0x0,0x20,0x40,
0x8,0xC,0x0,0xC,0x0,0x80,0xE1,0xFF,0xFF,0xF1,0x7,0x0,0xC,0x0,0x20,0x60,
0x8,0xC,0x0,0xC,0x0,0x80,0xE1,0xFF,0xFF,0xF9,0x3,0x0,0xC,0x0,0x20,0x60,
0x18,0xC,0x0,0xC,0x0,0xC0,0xF0,0xFF,0xFF,0xF9,0x3,0x0,0x6,0x0,0x20,0x60,
0x18,0xC,0x0,0xC,0x0,0xC0,0x0,0xF0,0xFF,0xFD,0x3,0x0,0x6,0x0,0x30,0x20,
0x10,0xC,0x0,0xC,0x0,0x40,0x0,0xE0,0xFF,0xFD,0x1,0x0,0x2,0x0,0x30,0x20,
0x10,0xC,0x0,0x4,0x0,0x60,0x0,0xC0,0xFF,0xFF,0x1,0x0,0x2,0x0,0x10,0x20,
0x10,0xC,0x0,0x6,0x0,0x60,0x0,0x80,0x1F,0xFF,0x0,0x0,0x3,0x0,0x18,0x20,
0x10,0xC,0x0,0x6,0x0,0x60,0x0,0x0,0x0,0xFF,0x0,0x0,0x1,0x0,0x18,0x30,
0x30,0xC,0x0,0x6,0x0,0x20,0x0,0x0,0x0,0xFF,0x0,0x80,0x1,0x0,0x18,0x30,
0x30,0x8,0x0,0x6,0x0,0x30,0x0,0x0,0x80,0xFF,0x0,0x80,0x1,0x0,0x1C,0x10,
0x20,0x18,0x0,0x6,0x0,0x30,0x0,0x0,0x80,0xFF,0x0,0xC0,0x0,0x0,0xC,0x10,
0x20,0x18,0x0,0x6,0x0,0x30,0x0,0x0,0xC0,0xFE,0x0,0xC0,0x0,0x0,0xC,0x10,
0x60,0x18,0x0,0x6,0x0,0x18,0x0,0x4,0xC0,0xFF,0x0,0x60,0x0,0x0,0x4,0x18,
0x60,0x18,0x0,0x6,0x0,0x18,0x0,0x18,0x60,0xFF,0x1,0x60,0x0,0x0,0x6,0x8,
0xC0,0x30,0x0,0x6,0x0,0xC,0x0,0x30,0x60,0xF8,0x1,0x30,0x0,0x0,0x2,0xC,
0xC0,0x30,0x0,0x6,0x0,0xC,0x0,0x0,0xA0,0xFF,0x3,0x30,0x0,0x0,0x3,0x4,
0x80,0x70,0x0,0x6,0x0,0xC,0x0,0x0,0xB1,0xFF,0x8F,0x1F,0xFC,0x0,0x1,0x4,
0x80,0x71,0x0,0x6,0x0,0xC,0x0,0x0,0xD6,0x9F,0xDF,0x1F,0xFE,0x80,0x1,0x2,
0x80,0x61,0x0,0x6,0x0,0x6,0x0,0x0,0xDE,0xDF,0xDF,0x1F,0xFF,0x80,0x0,0x2,
0x0,0x63,0x0,0x6,0x0,0x6,0x0,0x0,0xCC,0xDF,0xFF,0x1F,0xFF,0xC0,0x0,0x3,
0x0,0xC3,0x0,0x6,0x0,0x6,0x0,0x0,0x1C,0xE0,0xFF,0x9F,0xFF,0x40,0x0,0x1,
0x0,0xC3,0x0,0x6,0x0,0x6,0x0,0x0,0x3C,0xE0,0xFF,0xDF,0xFF,0x60,0x80,0x1,
0x0,0xC6,0x0,0x6,0x0,0x2,0x0,0x0,0xE6,0xF8,0xF8,0xFF,0xFF,0x30,0x80,0x1,
0x0,0x86,0x0,0x4,0x0,0x3,0x0,0x0,0x2,0x0,0xFC,0xFF,0xFF,0x31,0xC0,0x0,
0x0,0x8C,0x1,0x4,0x0,0x1,0x0,0x0,0xF,0x0,0xFE,0xFF,0xFB,0x19,0xC0,0x0,
0x0,0x8C,0x1,0x4,0x80,0x1,0x0,0x80,0x1D,0x0,0xFE,0xFF,0xF9,0x9,0x60,0x0,
0x0,0x18,0x3,0x4,0x80,0x1,0x0,0x80,0x1C,0x0,0xFF,0xFF,0xF9,0xD,0x30,0x0,
0x0,0x18,0x3,0xC,0x80,0x0,0x0,0xC0,0x1C,0x80,0xFF,0xFF,0xF8,0x5,0x30,0x0,
0x0,0x30,0x6,0xC,0xC0,0x0,0x0,0x40,0x1C,0xC0,0xFF,0x7F,0xF8,0x3,0x18,0x0,
0x0,0x60,0x4,0xC,0xC0,0x0,0x0,0x60,0x1C,0xC0,0xFF,0x7F,0xF8,0x1,0xC,0x0,
0x0,0x40,0xC,0xC,0xC0,0x0,0x0,0xB0,0x7F,0xE0,0xFF,0x3F,0xF8,0x1,0x4,0x0,
0x0,0xC0,0x18,0x8,0x40,0x0,0x0,0xF0,0xFF,0x1F,0xFE,0x5F,0xC0,0x0,0x6,0x0,
0x0,0x80,0x19,0x18,0x40,0x0,0x0,0xF8,0xFF,0xFF,0xFF,0x5F,0x60,0x0,0x3,0x0,
0x0,0x0,0x33,0x10,0x40,0x0,0x0,0xFC,0xFF,0xFF,0xFF,0x5F,0x30,0x80,0x7,0x0,
0x0,0x0,0x36,0x10,0x40,0x0,0x0,0xFC,0xFF,0xFF,0xFF,0x5F,0x18,0xF0,0xFF,0x0,
0x0,0x0,0x6C,0x30,0x40,0x0,0x0,0xFE,0xFF,0xFF,0xFF,0x6F,0x1C,0xFC,0xFF,0x1,
0x0,0x0,0xD8,0x20,0x60,0xCE,0xE1,0xFF,0xFF,0xFF,0xFF,0x2F,0xE,0xFE,0xFF,0x3,
0x0,0x0,0xF0,0x21,0x60,0x9C,0xFF,0xFF,0xFF,0xFF,0xFF,0x2F,0x7,0xFF,0xFF,0x3,
0x0,0x0,0xE0,0x63,0x60,0x8,0xFE,0xFF,0xFF,0xFF,0xFF,0xBF,0x83,0x7F,0xF8,0x3,
0x0,0x0,0xC0,0x47,0x60,0x0,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xC1,0x3F,0xF8,0x3,
0x0,0x0,0x80,0xC7,0x60,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x3F,0xF8,0x3,
0x0,0x0,0x0,0x8F,0x60,0xF0,0xFF,0xDF,0xFD,0xFF,0xFF,0x7F,0xC0,0x7F,0x0,0x0,
0x0,0x0,0x0,0x9E,0x61,0xE0,0xFF,0x3F,0xFE,0xFF,0xFF,0x3F,0xC0,0xFF,0x7,0x0,
0x0,0x0,0x0,0xBC,0x61,0xC0,0xFF,0x7F,0xFF,0xFF,0xFF,0xF,0xE0,0xFF,0x1F,0x0,
0x0,0x0,0x0,0x70,0xE3,0xC1,0xFF,0xFF,0xFE,0xFF,0xFF,0xF,0xF0,0xFF,0x3F,0x0,
0x0,0x0,0x0,0xE0,0xF6,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,0x3,0x18,0xFF,0x7F,0x0,
0x0,0x0,0x0,0x80,0xEF,0x3,0xFF,0xF9,0xFF,0xFF,0xFF,0x1,0xE,0xFC,0x7F,0x0,
0x0,0x0,0x0,0x0,0xEE,0x7,0xF9,0xFD,0xFF,0xFF,0x7F,0x80,0xF3,0xE3,0x7F,0x0,
0x0,0x0,0x0,0x0,0xF8,0xCF,0xF0,0xFC,0xFF,0xFF,0x7F,0xE0,0xF0,0x83,0x7F,0x0,
0x0,0x0,0x0,0x0,0xF0,0xFE,0xEE,0x87,0xFF,0xFF,0x3F,0x3C,0xF0,0xC7,0x3F,0x0,
0x0,0x0,0x0,0x0,0xC0,0x7F,0xFC,0x0,0xFF,0xFF,0x1F,0xF,0xF0,0xFF,0x1F,0x0,
0x0,0x0,0x0,0x0,0x0,0xFF,0x3C,0x8E,0xFF,0xFF,0xE7,0x3,0xE0,0xFF,0xF,0x0,
0x0,0x0,0x0,0x0,0x0,0xFC,0xE7,0xFF,0xFF,0xFF,0xFF,0x0,0xE0,0xFF,0x3,0x0,
0x0,0x0,0x0,0x0,0x0,0xC0,0x87,0xF3,0xFF,0xFF,0xF,0x0,0x80,0xFF,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x1F,0xFC,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF8,0x7F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

/*
swfpict[0] = Bas  : gris -> blanc
swfpict[1] = Haut : gris -> blanc
swfpict[2] = Bas  : gris -> noir
swfpict[3] = Haut : gris -> noir
*/


void main()
{ int yrep=0;
  desactint9(); // on desactive le contraste, MENU et l'eteinte de la caltos
  clear3(SEG1); // on efface les 3 premiers buffers
  mode(0xdb);   // on passe en mode db
  contrast(15); // on met le contraste optimal pour du gris
  disp_bmp(swfpict[1],SEG1); // on affiche le haut de l'image directement a l'ecran
  disp_bmp(swfpict[3],SEG3);
  disp_bmp(swfpict[0],SEG4); // puis on dispose les images dans la memoire video
  disp_bmp(swfpict[1],SEG5);
  disp_bmp(swfpict[2],SEG6);
  disp_bmp(swfpict[3],SEG7);

  do{
  if (testkey(4,7)) break;      // si ESC est presse, quitte la boucle et termine le programme
  if (testkey(3,9) && yrep<63)  // si BAS et qu'on est pas au bout du scrolling bas
  { downscroll(yrep,SEG4,SEG1); // alors scrolle vers le bas la premiere couche
    downscroll(yrep,SEG6,SEG3); // puis la deuxieme
    yrep++;}                    // decremente yrep
  if (testkey(2,9) && yrep>0)   // si HAUT et qu'on est pas au bout du scrolling haut
  { upscroll(yrep-1,SEG5,SEG1); // alors scrolle vers le haut la premiere couche
    upscroll(yrep-1,SEG7,SEG3); // puis la deuxieme (attention decalage de -1 par rapport au scrolling bas)
    yrep--;}                    // incremente yrep

  }while (1);    // quitte le programme :
  clear3(SEG1);  // efface les 3 premiers buffer
  contrast(8);   // remet le contraste pour mode c3
  mode(0xc3);    // retourne en mode c3
  actint9();}    // reactive le contraste, MENU et l'eteinte de la caltos

