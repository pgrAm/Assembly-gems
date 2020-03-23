#ifndef TRANSPARENT_SPRITES_H_
#define TRANSPARENT_SPRITES_H_
extern void doTransparentBlitEven(uint16_t height, uint16_t screen_offset, const uint8_t * bitmapPtr, uint16_t len, uint16_t ldiff, uint16_t sdiff);
extern void doTransparentBlitOdd(uint16_t height, uint16_t screen_offset, const uint8_t * bitmapPtr, uint16_t len, uint16_t ldiff, uint16_t sdiff);
extern void doTransparentBlitOne(uint16_t height, uint16_t screen_offset, const uint8_t * bitmapPtr, uint16_t ldiff, uint16_t sdiff);

#define BLIT_LOOP 	"lodsw"\
					"test al, al"\
					"jnz storePixel1"\
				"skipPixel1:"\
					"inc di"\	
					"or al, ah"\
					"jnz storePixel2"\
				"skipPixel2:"\
					"inc di"\
					"dec cx"\
					"jz finish_line"\
				"loadPixel:"\
					"lodsw"\
					"test al, al"\
					"jz skipPixel1"\
				"storePixel1:" \
					"stosb"\
					"test ah, ah"\
					"jz skipPixel2"\
					"mov al, ah"\
				"storePixel2:" \
					"stosb"\
					"loop loadPixel"\
				"finish_line:"

#pragma aux doTransparentBlitEven = \
		"cld" \
		"push bp"\
		"xchg ax, bp" \
		INSTR(mov ax, VGA_SEGMENT) \
		"mov es, ax" \
		"shr cx, 1"\
	"rowLoop :"\
		"push cx"\
		BLIT_LOOP \
		"pop cx"\
		"add si, dx"\
		"add di, bp"\
		"dec bx"\
		"jnz rowLoop" \
		"pop bp" \
		parm[bx][di][ds si][cx][dx][ax] modify exact[ax bx cx di si es];

#pragma aux doTransparentBlitOdd = \
		"cld" \
		"push bp"\
		"xchg ax, bp" \
		INSTR(mov ax, VGA_SEGMENT) \
		"mov es, ax" \
		"shr cx, 1"\
	"rowLoop :"\
		"push cx"\
		BLIT_LOOP \
		"pop cx"\
		"lodsb"\
		"test al, al"\
		"jz skipLast"\
		"stosb"\
		"dec di"\
	"skipLast:"\
		"inc di"\
		"add si, dx"\
		"add di, bp"\
		"dec bx"\
		"jnz rowLoop" \
		"pop bp" \
		parm[bx][di][ds si][cx][dx][ax] modify exact[ax bx cx di si es];

#pragma aux doTransparentBlitOne = \
		"cld" \
		"cld" \
		INSTR(mov ax, VGA_SEGMENT) \
		"mov es, ax" \
		"lodsb"\
		"test al, al"\
		"jnz storePixel"\
	"skipPixel:"\
		"inc di"\
		"add si, dx"\
		"add di, bx"\
		"dec cx"\
		"jz end"\
	"loadPixel :"\
		"lodsb"\
		"test al, al"\
		"jz skipPixel"\
	"storePixel:"\
		"stosb"\
		"add si, dx"\
		"add di, bx"\
		"loop loadPixel"\
	"end:"\
		parm[cx][di][ds si][dx][bx] modify exact[ax cx di si es];
#endif
