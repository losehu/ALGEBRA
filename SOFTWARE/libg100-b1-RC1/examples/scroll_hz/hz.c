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
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xFC,0xF,0xFF,0x7,0xC0,0xFF,0x3,0xF8,0x7F,0x0,0x0,0xF0,0xFF,0x3,0x0,0x0,
0x6,0xC,0x1,0x2,0x40,0x0,0x2,0x8,0x20,0x0,0x80,0xF,0x0,0xC,0x0,0x0,
0x4,0x6,0x1,0x1,0x80,0x80,0x1,0x10,0x10,0x0,0x60,0x0,0x0,0x30,0x0,0x0,
0x4,0x2,0x81,0x0,0x80,0x80,0x0,0x10,0x8,0x0,0x10,0x0,0x0,0xC0,0x2,0x0,
0x4,0x2,0x81,0x0,0x80,0x80,0x0,0x10,0x8,0x0,0x8,0x0,0x0,0x80,0x1,0x0,
0x2,0x81,0x40,0x0,0x40,0x80,0x0,0x8,0x8,0x0,0x4,0x0,0x0,0x0,0x1,0x0,
0x2,0x81,0x40,0x0,0x20,0x80,0x0,0x4,0x8,0x0,0x3,0xE0,0x1F,0x80,0x0,0x0,
0x2,0x81,0x40,0x0,0x20,0x80,0x0,0x4,0x8,0x80,0x0,0x1C,0xE0,0x80,0x0,0x0,
0x1,0x81,0x40,0x0,0x10,0x80,0x0,0x2,0x8,0x80,0x0,0x2,0x0,0x41,0x0,0x0,
0x81,0x40,0x20,0x0,0x8,0x80,0x0,0x1,0x8,0x40,0x80,0x1,0x0,0x22,0x0,0x0,
0x81,0x40,0x20,0x0,0x8,0x80,0x0,0x1,0x10,0x40,0x40,0x0,0x0,0x24,0x0,0x0,
0x80,0x40,0x20,0x0,0x4,0x80,0x80,0x0,0x10,0x40,0x40,0x0,0x0,0x14,0x0,0x0,
0x40,0x20,0x10,0x0,0x2,0x81,0x40,0x20,0x10,0x40,0x40,0x0,0x0,0x14,0x0,0x0,
0x40,0x20,0x10,0x0,0x82,0x1,0x21,0x30,0x10,0x20,0x40,0x0,0x0,0x8,0x0,0x0,
0x40,0x20,0x10,0x0,0x81,0x1,0x21,0x30,0x10,0x20,0x40,0x0,0x0,0x0,0x0,0x0,
0x20,0x10,0x8,0x80,0x40,0x1,0x11,0x28,0x10,0x20,0x40,0x0,0x0,0x0,0x0,0x0,
0x20,0x10,0x8,0x80,0x40,0x1,0x9,0x24,0x10,0x20,0x80,0x3,0x0,0x0,0x0,0x0,
0x20,0x10,0x8,0x40,0x20,0x1,0x9,0x24,0x10,0x40,0x0,0xFC,0x1,0x0,0x0,0x0,
0x20,0x10,0x8,0x40,0x10,0x1,0x5,0x22,0x10,0x40,0x0,0x0,0x3E,0x0,0x0,0x0,
0x10,0x8,0x4,0x20,0x10,0x2,0x3,0x21,0x10,0x80,0x0,0x0,0xC0,0x0,0x0,0x0,
0x10,0x8,0x4,0x10,0x8,0x2,0x83,0x20,0x20,0x80,0x0,0x0,0x0,0x1,0x0,0x0,
0x10,0x8,0x4,0x10,0x4,0x2,0x81,0x20,0x20,0x0,0x1,0x0,0x0,0x1,0x0,0x0,
0x8,0x4,0x2,0x8,0x4,0x2,0x40,0x20,0x20,0x0,0x1E,0x0,0x0,0x2,0x0,0x0,
0x8,0x4,0x2,0x4,0x2,0x2,0x20,0x20,0x20,0x0,0xE0,0xF,0x0,0x2,0x0,0x0,
0x8,0x4,0x2,0x4,0x2,0x2,0x20,0x20,0x20,0x0,0x0,0xF0,0x0,0x2,0x0,0x0,
0x8,0x4,0x2,0x2,0x1,0x2,0x10,0x20,0x20,0x40,0x0,0x0,0x3,0x2,0x0,0x0,
0x4,0x2,0x1,0x81,0x0,0x2,0x8,0x20,0x20,0x60,0x0,0x0,0x2,0x2,0x0,0x0,
0x4,0x2,0x81,0x80,0x0,0x2,0x8,0x20,0x20,0x50,0x0,0x0,0x2,0x2,0x0,0x0,
0x4,0x3,0xC3,0x80,0x1,0x2,0x8,0x10,0x60,0x90,0x0,0x0,0x1,0x1,0x0,0x0,
0x8,0x1,0x22,0x0,0x81,0x1,0x10,0x8,0x80,0x8,0x1,0x80,0x0,0x1,0x0,0x0,
0x87,0xFF,0xF3,0xFF,0x81,0xFF,0xF,0xF8,0x7F,0x4,0x2,0x40,0x80,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0xC,0x30,0x80,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0xF0,0xF,0x40,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x20,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x3,0x0,0x0,0x10,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x4,0x0,0x0,0xC,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x78,0x0,0x0,0x3,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x3,0xE0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x1F,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},{
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x38,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x28,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xFE,0x7F,0xC0,0xFF,0xC8,0xFF,0xFF,0xFF,0x1,0xF0,0x3F,0x0,0x0,0x0,
0x0,0x0,0x3,0x20,0x60,0xC0,0x4,0x0,0x0,0x80,0x0,0x18,0x30,0x0,0x0,0x0,
0x0,0x0,0x4,0x10,0x40,0x60,0x4,0x0,0x0,0x40,0x0,0x10,0x18,0x0,0x0,0x0,
0x0,0x0,0x8,0x10,0x40,0x20,0x4,0x0,0x0,0x20,0x0,0x10,0x8,0x0,0x0,0x0,
0x0,0x0,0x8,0x10,0x40,0x20,0x4,0x0,0x0,0x20,0x0,0x10,0x8,0x0,0x0,0x0,
0x0,0x0,0x8,0x10,0x20,0x10,0x2,0x0,0x0,0x10,0x0,0x8,0x4,0x0,0x0,0x0,
0x0,0x0,0x8,0x20,0x20,0x10,0xFE,0xFF,0x7F,0x10,0x0,0x8,0x4,0x0,0x0,0x0,
0x0,0x0,0x10,0x20,0x20,0x10,0x6,0x0,0x40,0x10,0x0,0x8,0x4,0x0,0x0,0x0,
0x0,0x0,0x10,0x40,0x10,0x10,0x0,0x0,0x20,0x10,0x0,0x8,0x4,0x0,0x0,0x0,
0x0,0x0,0x20,0x40,0x10,0x8,0x0,0x0,0x20,0x8,0x0,0x4,0x2,0x0,0x0,0x0,
0x0,0x0,0x20,0x40,0x10,0x8,0x80,0x1,0x20,0x8,0x0,0x4,0x2,0x0,0xC,0x0,
0x0,0x0,0x40,0x80,0x8,0x8,0x80,0x1,0x10,0x8,0x0,0x4,0x2,0x0,0x1C,0x80,
0x0,0x0,0x70,0x80,0xF,0x4,0x40,0xFE,0x1F,0x8,0x0,0x2,0x1,0x0,0xE2,0xFF,
0x0,0x0,0xC,0x0,0x0,0x4,0x40,0x0,0x0,0x4,0x0,0x2,0x1,0x0,0x2,0x0,
0x0,0x0,0x2,0x0,0x0,0x4,0x40,0x0,0x0,0x4,0x0,0x2,0x1,0x0,0x2,0x0,
0x0,0x0,0x1,0x0,0x0,0x2,0x20,0x0,0x0,0x4,0x0,0x2,0x1,0x0,0x1,0x0,
0x0,0xC0,0x0,0x0,0x0,0x2,0x20,0x0,0x0,0x4,0x0,0x81,0x0,0x0,0x1,0x0,
0x0,0x20,0x0,0x0,0x0,0x2,0x20,0x0,0x0,0x2,0x0,0x81,0x0,0x0,0x1,0x0,
0x0,0x20,0xE0,0xFF,0x7,0x2,0xE0,0xFF,0x7,0x2,0x0,0x81,0x0,0x0,0xFD,0x7F,
0x0,0x20,0x10,0x0,0x4,0x1,0x50,0x0,0x4,0x2,0x80,0x40,0x0,0x80,0x2,0x40,
0x0,0x10,0x8,0x0,0x2,0x1,0x30,0x0,0x2,0x2,0x80,0x40,0x0,0x80,0x3,0x20,
0x0,0x10,0x8,0x0,0x2,0x1,0x0,0x0,0x2,0x1,0x80,0x40,0x0,0x0,0x0,0x20,
0x0,0x8,0x8,0x0,0x82,0x0,0x0,0x0,0x2,0x1,0x80,0x40,0x0,0x0,0x0,0x20,
0x0,0x8,0x8,0x0,0x81,0xE0,0x0,0x0,0x1,0x39,0x40,0x20,0xC0,0x7,0x0,0x10,
0x0,0x8,0xF0,0xFF,0x81,0xA0,0xFF,0xFF,0x1,0xE9,0x7F,0xE0,0x7F,0xFB,0xFF,0x1F,
0x0,0x8,0x0,0x0,0x80,0x20,0x0,0x0,0x80,0x4,0x0,0x0,0x0,0x1,0x0,0x0,
0x0,0x10,0x0,0x0,0x40,0x10,0x0,0x0,0x80,0x4,0x0,0x0,0x80,0x1,0x0,0x0,
0x0,0x10,0x0,0x0,0x40,0x10,0x0,0x0,0x80,0x4,0x0,0x0,0x80,0x1,0x0,0x0,
0x0,0x20,0x0,0x0,0x40,0x10,0x0,0x0,0xC0,0x2,0x0,0x0,0x80,0x0,0x0,0x0,
0x0,0x40,0x0,0x0,0x80,0x10,0x0,0x0,0x80,0x2,0x0,0x0,0x80,0x0,0x0,0x0,
0x0,0x80,0xFF,0xFF,0x7F,0xC8,0xFF,0xFF,0xFF,0xFA,0xFF,0xFF,0xCF,0xFE,0xFF,0xFF,
0x0,0x0,0x0,0x0,0x0,0x28,0x0,0x0,0x0,0xD,0x0,0x0,0x50,0x3,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x18,0x0,0x0,0x0,0x7,0x0,0x0,0xF0,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},{
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x4,0x20,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x2,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x2,0x0,0x10,0x40,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x1,0x0,0x0,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x1,0x0,0x0,0x4,0x10,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x81,0x1,0x1,0x1,0x3,0x4,0x8,0x0,0x1,0x40,0x0,0x0,0x0,0x0,0x0,0x0,
0x81,0x80,0x80,0x80,0x0,0x1,0x1,0x10,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,
0xC1,0x40,0x20,0x20,0x0,0x0,0x0,0x2,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xFD,0x2F,0xFF,0x1F,0xC8,0xFF,0x63,0xF8,0x7F,0x0,0x0,0xF0,0xFF,0x3,0x0,0x0,
0x7,0x1C,0x1,0x6,0x42,0x0,0xE,0x8,0x20,0x10,0x80,0xF,0x0,0xC,0x0,0x0,
0x5,0x1E,0x1,0x81,0xA0,0x80,0x1,0x14,0x90,0x0,0x60,0x0,0x0,0x30,0x0,0x0,
0x5,0xE,0x81,0x20,0x88,0x80,0x40,0x10,0x8,0x0,0x10,0x0,0x0,0xC0,0x2,0x0,
0x5,0xC6,0x81,0xC,0x81,0x80,0x4,0x10,0x8,0x0,0x8,0x0,0x0,0x80,0x1,0x0,
0x3,0xE3,0x40,0x61,0x70,0x80,0x0,0xB,0x18,0x0,0x4,0x0,0x0,0x0,0x1,0x0,
0x3,0x91,0x40,0x8,0x2E,0x80,0x1C,0x4,0x8,0x18,0x3,0xE0,0x1F,0x80,0x0,0x0,
0x3,0xCD,0x40,0xE1,0x21,0x80,0x1,0x5,0x38,0x80,0x0,0x1C,0xE0,0x80,0x0,0x0,
0x1,0xFF,0x40,0x3C,0x14,0x80,0x0,0x2,0x8,0x80,0x0,0x2,0x0,0x41,0x0,0x0,
0x81,0x7F,0xE0,0x7F,0x8,0x80,0xE0,0x1,0x8,0x41,0x80,0x1,0x20,0x22,0x0,0x0,
0x81,0x7F,0xE0,0xFF,0xF,0x80,0x3,0x1,0x10,0x40,0xC0,0x3,0x0,0x24,0x0,0x0,
0x80,0x7F,0xE0,0xFF,0x7,0x80,0x80,0x0,0x10,0x7F,0x40,0x0,0x0,0x14,0x0,0x0,
0xC0,0x3F,0xF0,0xFF,0x3,0x81,0x40,0x20,0x10,0x40,0x40,0x0,0x0,0x14,0x0,0x0,
0xC0,0x3F,0xF0,0xFF,0x83,0x1,0x3F,0x30,0x10,0x20,0x40,0x0,0x0,0x8,0x0,0x0,
0xC0,0x3F,0xF0,0xFF,0x81,0x1,0x23,0x30,0x10,0x20,0x40,0xF8,0x3,0x0,0x0,0x0,
0xE0,0x1F,0xF8,0xFF,0xC0,0x1,0x1F,0x38,0x10,0x20,0x40,0x0,0x0,0x0,0x0,0x0,
0xE0,0x1F,0xF8,0x80,0x40,0x1,0x9,0x24,0x10,0x20,0x80,0x3,0x0,0x0,0x0,0x0,
0xE0,0x1F,0xF8,0x7F,0xE0,0x1,0x9,0x24,0x10,0x40,0x0,0xFC,0x1,0x0,0x0,0x0,
0xE0,0x1F,0xF8,0x7F,0x10,0x1,0x7,0x3E,0x30,0x40,0x0,0x0,0x3E,0x0,0x0,0x0,
0xF0,0xF,0xFC,0x3F,0xF0,0x3,0x3,0x21,0x10,0x80,0x0,0x0,0xC0,0x0,0x0,0x0,
0xF0,0xF,0xFC,0x1F,0x8,0x2,0x83,0x3E,0x20,0x80,0x0,0x0,0x0,0x1,0x0,0x0,
0xF0,0xF,0xFC,0x1F,0xE4,0x3,0x81,0x20,0x20,0x80,0x1,0x0,0x0,0x1,0x0,0x0,
0xF8,0x7,0xFE,0xF,0x4,0x2,0x40,0x20,0x20,0x0,0x1E,0x0,0x0,0x2,0x0,0x0,
0xF8,0x7,0xFE,0x7,0xFA,0x2,0x20,0x3C,0x20,0x40,0xE0,0xF,0x0,0x2,0x0,0x0,
0xE8,0x7,0xFE,0x5,0xE,0x2,0x20,0x20,0xE0,0x1,0x0,0xF0,0x0,0x2,0x0,0x0,
0xC8,0x4,0xFE,0x2,0x83,0x2,0x10,0x20,0x20,0xC0,0x3,0x0,0x3,0x2,0x0,0x0,
0xCC,0x3,0x3F,0x81,0x38,0x2,0xC8,0x23,0x20,0x60,0x0,0xC,0x2,0x2,0x0,0x0,
0x9C,0x3,0xF3,0x80,0x86,0x3,0x8,0x30,0x20,0x50,0x0,0x0,0x2,0x2,0x0,0x0,
0x14,0x3,0xC7,0x80,0x21,0x2,0x28,0x10,0x60,0x90,0x4,0x0,0x1,0x1,0x0,0x0,
0x18,0x1,0x32,0x0,0x83,0x1,0x10,0xC,0x80,0xE,0x1,0x80,0x0,0x1,0x0,0x0,
0xB7,0xFF,0xF3,0xFF,0x89,0xFF,0x1F,0xF8,0x7F,0x4,0x2,0x40,0x80,0x0,0x0,0x0,
0x21,0x18,0x82,0xE0,0x40,0x80,0x80,0x0,0x8,0x2,0x2C,0x30,0x80,0x0,0x0,0x0,
0x21,0x10,0x4,0x82,0x3,0x2,0x4,0x8,0x0,0x2,0xF0,0xF,0x40,0x0,0x0,0x0,
0x41,0x30,0x8,0x8,0xC,0x8,0x0,0x80,0x0,0x1,0x0,0x0,0x20,0x0,0x0,0x0,
0x40,0x60,0x20,0x10,0x30,0x40,0x0,0x2,0x88,0x3,0x0,0x0,0x10,0x0,0x0,0x0,
0x80,0x40,0x40,0x40,0xC0,0x0,0x0,0x0,0x80,0x4,0x0,0x0,0xC,0x0,0x0,0x0,
0x80,0x80,0x80,0x0,0x0,0x3,0x8,0x0,0x0,0x78,0x0,0x0,0x3,0x0,0x0,0x0,
0x0,0x0,0x1,0x1,0x2,0xC,0x0,0x0,0x8,0x80,0x3,0xE0,0x0,0x0,0x0,0x0,
0x0,0x1,0x2,0x0,0x8,0x20,0x0,0x0,0x0,0x0,0xFC,0x1F,0x0,0x0,0x0,0x0,
0x0,0x1,0x0,0x0,0x0,0x80,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x2,0x8,0x20,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x10,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},{
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x10,0x40,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x8,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x0,0x0,0x80,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x0,0x0,0x8,0x0,0x40,0x80,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x0,0x80,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x0,0x0,0x0,0x80,0x0,0x1,0x1,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x0,0x1,0x20,0x40,0x0,0x1,0x2,0x2,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x78,0x0,0x8,0x0,0x0,0x1,0x4,0x4,0x6,0x2,
0x0,0x0,0x0,0x0,0x0,0x0,0x28,0x4,0x80,0x0,0x4,0x4,0x18,0x8,0x4,0x2,
0x0,0x0,0xFE,0x7F,0xC0,0xFF,0xC8,0xFF,0xFF,0xFF,0x21,0xF0,0x7F,0x20,0x8,0x4,
0x0,0x0,0x3,0x20,0x60,0xC0,0x4,0x0,0x0,0x80,0x80,0x18,0xF0,0x40,0x18,0x4,
0x0,0x0,0x4,0x10,0x40,0x60,0x4,0x0,0x0,0x40,0x3,0x14,0x18,0x83,0x30,0xC,
0x0,0x0,0x8,0x10,0x40,0x20,0x6,0x0,0x0,0x20,0x38,0x10,0x8,0xE,0x61,0x8,
0x0,0x0,0x8,0x10,0x40,0x20,0x4,0x0,0x0,0x60,0xC0,0x11,0x28,0x18,0xC6,0x8,
0x0,0x0,0x8,0x10,0x20,0x50,0x2,0x0,0x0,0x10,0x4,0xE,0x84,0x70,0xCC,0x99,
0x0,0x0,0x8,0x20,0x20,0x10,0xFE,0xFF,0x7F,0x70,0x40,0x8,0x34,0xC2,0x99,0x93,
0x0,0x0,0x10,0x20,0x20,0x10,0x6,0x0,0x40,0x10,0xC,0xC,0x84,0x18,0x3F,0xB3,
0x0,0x0,0x10,0x40,0x10,0x90,0x3,0x0,0x30,0xF0,0x81,0x9,0x7C,0x7E,0x7E,0xB6,
0x0,0x0,0x20,0x40,0x10,0x8,0x0,0xF,0x20,0x8,0xF8,0x4,0xFE,0xFF,0xFF,0xFF,
0x0,0x0,0x20,0x40,0x10,0xC8,0x87,0x1,0x3E,0x8,0x4,0x6,0xFE,0xFF,0xFF,0xFF,
0x0,0x0,0x40,0x80,0x8,0x8,0x80,0x7F,0x10,0xF8,0x1,0x4,0xFE,0xFF,0xFF,0xFF,
0x0,0x0,0x70,0x80,0xF,0x4,0x40,0xFE,0x1F,0x8,0xF8,0x3,0xFF,0xFF,0xE3,0xFF,
0x0,0x0,0xC,0x0,0x0,0x4,0x40,0x0,0x0,0x84,0xFF,0x2,0xFF,0xFF,0x3,0x0,
0x0,0x0,0x2,0x0,0x0,0x4,0x40,0x0,0x0,0x4,0x0,0x2,0xFF,0xFF,0x3,0x0,
0x0,0x0,0x1,0x0,0x0,0x2,0x20,0x0,0x0,0xFC,0xFF,0x3,0xFF,0xFF,0x1,0x0,
0x0,0xC0,0x0,0x0,0x0,0x2,0x20,0x0,0x0,0x4,0x0,0x81,0x0,0xFF,0x1,0x0,
0x0,0x20,0x0,0x0,0x0,0x2,0x20,0x0,0x0,0x2,0x0,0x81,0xFF,0xFF,0x1,0x0,
0x0,0x20,0xE0,0xFF,0x7,0x2,0xE0,0xFF,0x7,0xFE,0x3,0x81,0xFF,0xFF,0xFD,0x7F,
0x0,0x20,0x10,0x0,0x4,0x1,0x50,0x0,0x4,0xC2,0xFF,0x40,0xFE,0xFF,0xFE,0x7F,
0x0,0x10,0x8,0x0,0x2,0x1,0x30,0xF0,0x3,0x2,0xF8,0xC0,0xFF,0xFF,0xFF,0x3F,
0x0,0x10,0x8,0x0,0x2,0xFD,0x1,0x0,0x2,0x1,0xFC,0xC0,0xFF,0xFF,0xFF,0x3F,
0x0,0x8,0x8,0x0,0x82,0x0,0x40,0x0,0x2,0xFF,0x80,0xC0,0xFF,0xFF,0xFF,0x3F,
0x0,0x8,0x8,0x0,0x81,0xE0,0x0,0x80,0x1,0x39,0x42,0xE0,0xFF,0xFF,0xFF,0x1F,
0x0,0x8,0xF0,0xFF,0x81,0xA0,0xFF,0xFF,0x1,0xE9,0x7F,0xE0,0x7F,0xFB,0xFF,0x1F,
0x0,0x8,0x0,0x0,0x80,0x20,0x0,0x0,0x80,0x4,0x0,0x0,0x0,0x1,0x0,0x0,
0x0,0x10,0x0,0x0,0x40,0x10,0x0,0x0,0x80,0x7,0x0,0x0,0x80,0x1,0x0,0x0,
0x0,0x10,0x0,0x0,0x40,0x10,0x0,0x0,0x80,0x4,0x0,0x0,0x80,0x1,0x0,0x0,
0x0,0x20,0x0,0x0,0x40,0x10,0x0,0x0,0xC0,0x2,0x0,0x0,0x80,0x0,0x0,0x0,
0x0,0x40,0x0,0x0,0x80,0x10,0x0,0x0,0x80,0x2,0x0,0x0,0x80,0x0,0x0,0x0,
0x0,0x80,0xFF,0xFF,0x7F,0xC8,0xFF,0xFF,0xFF,0xFB,0xFF,0xFF,0xCF,0xFE,0xFF,0xFF,
0x0,0x0,0x0,0x0,0x0,0x2A,0x0,0x0,0x30,0xD,0x1,0x81,0x50,0x23,0x8,0x86,
0x0,0x0,0x0,0x0,0x0,0x18,0x0,0x0,0x3,0x17,0x40,0x20,0xF0,0x11,0x4,0x82,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30,0x0,0x2,0x8,0x8,0x18,0x8,0x2,0x83,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x4,0x2,0x1,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x0,0x4,0x40,0x40,0x0,0x1,0x81,0x80,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x10,0xC0,0x80,0x0,0x80,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x20,0x0,0x0,0x80,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x10,0x0,0x40,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x0,0x0,0x0,0x8,0x10,0x40,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x8,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x0,0x0,0x4,0x20,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

/*
swfpict[0] = Gauche : gris -> blanc
swfpict[1] = Droite : gris -> blanc
swfpict[2] = Gauche : gris -> noir
swfpict[3] = Droite : gris -> noir
*/

void main()
{ int xrep=0;
  desactint9(); // on desactive le contraste, MENU et l'�teinte de la caltos
  clear3(SEG1); // on efface les 3 premiers buffers
  mode(0xdb);   // on passe en mode db
  contrast(15); // on met le contraste optimal pour du gris
  disp_bmp(swfpict[0],SEG1); // on affiche le cote gauche directement a l'ecran 
  disp_bmp(swfpict[2],SEG3);
  disp_bmp(swfpict[0],SEG4); // puis en memoire video
  disp_bmp(swfpict[2],SEG5);
  disp_bmp(swfpict[1],SEG6); // finallement le cote droit en memoire video
  disp_bmp(swfpict[3],SEG7);

  do{
  if (testkey(4,7)) break;    // si ESC est presse, quitte la boucle et termine le programme
  if (testkey(1,9) && xrep>0)  // si GAUCHE et qu'on est pas au bout du scrolling gauche
  { leftscroll(xrep-1,SEG4,SEG1); // alors scrolle vers la gauche la premiere couche
    leftscroll(xrep-1,SEG5,SEG3); // puis la deuxieme (attention decalage de -1 par rapport au scrolling droit)
    xrep--;}                      // decremente xrep
  if (testkey(4,9) && xrep<127) // si DROITE et qu'on est pas au bout du scrolling droit
  { rightscroll(xrep,SEG6,SEG1); // alors scrolle vers la droite la premiere couche
    rightscroll(xrep,SEG7,SEG3); // puis la deuxieme
    xrep++;}                     // incremente xrep

  }while (1);    // quitte le programme :
  clear3(SEG1);  // efface les 3 premiers buffer
  contrast(8);   // remet le contraste pour mode c3
  mode(0xc3);    // retourne en mode c3
  actint9();}    // reactive le contraste, MENU et l'�teinte de la caltos
