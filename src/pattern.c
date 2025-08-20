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

#include "song.h"

void prepare_pattern(pattern_t* p) {
	unsigned row_index, channel_index;
	p->jumppos = 0xff;
	p->end_of_row = 63;
	for (row_index = 0; row_index < MAX_ROWS; ++row_index) {
		for (channel_index = 0; channel_index < MAX_CHANNELS; ++channel_index) {
			p->rows[row_index][channel_index].note = 255;
			p->rows[row_index][channel_index].sample = 0;
			p->rows[row_index][channel_index].volume = 65;
		}
	}
}
