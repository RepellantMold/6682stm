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

#ifndef B691DB19_C839_4C83_936A_32622C4FD91E
#define B691DB19_C839_4C83_936A_32622C4FD91E

#include <stdio.h>

#include "types.h"

uint8_t fread8(FILE* f);
uint16_t fread16(FILE* f);
uint32_t fread32(FILE* f);

void fwrite8(FILE* f, uint8_t v);
void fwrite16(FILE* f, uint16_t v);
void fwrite32(FILE* f, uint32_t v);

#define fokay(f)    (!(feof(f) || ferror(f)))
#define fskip(f, n) fseek(f, (n), SEEK_CUR)
#define fgoto(f, n) fseek(f, (n), SEEK_SET)

#endif /* B691DB19_C839_4C83_936A_32622C4FD91E */
