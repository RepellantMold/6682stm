/*
 * 6682stm - written by RepellantMold
 * designed for converting "668" files from Tran's STM "player".
 * requires ANSI C or later.
 *
 * (C) 2025 Jade "RepellantMold" Andersen
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <stdio.h>

#include "types.h"

uint8_t fread8(FILE* f) { return (uint8_t)fgetc(f); }

uint16_t fread16(FILE* f) {
	uint16_t result = 0;
	result = fread8(f) << 0UL;
	result |= fread8(f) << 8UL;
	return result;
}

uint32_t fread32(FILE* f) {
	uint32_t result = 0;
	result = (uint32_t)fread8(f) << 0UL;
	result |= (uint32_t)fread8(f) << 8UL;
	result |= (uint32_t)fread8(f) << 16UL;
	result |= (uint32_t)fread8(f) << 24UL;
	return result;
}

void fwrite8(FILE* f, uint8_t v) { fputc(v, f); }

void fwrite16(FILE* f, uint16_t v) {
	fputc((v >> 0) & 0xFF, f);
	fputc((v >> 8) & 0xFF, f);
}

void fwrite32(FILE* f, uint32_t v) {
	fputc((v >> 0) & 0xFF, f);
	fputc((v >> 8) & 0xFF, f);
	fputc((v >> 16UL) & 0xFF, f);
	fputc((v >> 24UL) & 0xFF, f);
}
