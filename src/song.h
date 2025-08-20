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

#ifndef AE6AAFF8_C1A8_4722_9B86_D8D59D76438B
#define AE6AAFF8_C1A8_4722_9B86_D8D59D76438B

#include "types.h"

typedef struct sample {
	int8_t* data;
	uint32_t length;
	uint32_t loop_start;
	uint32_t loop_end;
} sample_t;

typedef struct row {
	uint8_t note;
	uint8_t sample;
	uint8_t volume;
} row_t;

#define MAX_ROWS     (64)
#define MAX_CHANNELS (4)

typedef struct pattern {
	row_t rows[MAX_ROWS][MAX_CHANNELS];
	uint8_t end_of_row;
	uint8_t speed;
	uint8_t jumppos;
} pattern_t;

typedef struct song {
	pattern_t* patterns;
	sample_t* samples;
	uint8_t* orders;
	uint8_t total_orders;
	uint8_t total_samples;
	uint8_t total_patterns;
	uint8_t speed;
} song_t;

#define align_bits   (16 - 1)
#define pad(n)	     ((((n) + align_bits) & ~align_bits) & 15)
#define total_pad(n) ((n) + pad(n))

#endif /* AE6AAFF8_C1A8_4722_9B86_D8D59D76438B */
