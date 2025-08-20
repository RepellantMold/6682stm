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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "options.h"

#include "c89stringutils_string_extras.h"

#include "file.h"
#include "log.h"
#include "song.h"

#define STM_SAMPLE_HEADER_SIZE (32)
#define STM_MAX_INSTRUMENTS    (31)
#define STM_PATTERN_SIZE       ((MAX_ROWS * MAX_CHANNELS) * 4)

static void prepare_stm_pattern(pattern_t* p, uint8_t d[STM_PATTERN_SIZE]) {
	unsigned row_index, channel_index;
	bool place_jump_effect = false, need_pattern_break = false;
	place_jump_effect = p->jumppos < 64;
	need_pattern_break = place_jump_effect != false && p->end_of_row < 64;
	for (row_index = 0; row_index < MAX_ROWS; ++row_index) {
		for (channel_index = 0; channel_index < MAX_CHANNELS; ++channel_index) {
			uint8_t* data = &d[(row_index * 16) + (channel_index * 4)];
			if (row_index >= p->end_of_row) {
				data[0] = 0xFF;
				data[1] = 0x01;
				data[2] = 0x80;
				data[3] = 0x00;
			} else {
				bool at_end = row_index == (p->end_of_row - 1);
				row_t* r = &p->rows[row_index][channel_index];
				if (r->volume >> 2 >= 15) {
					r->volume = 65;
				}
				data[0] = r->note == 255
					      ? r->note
					      : ((r->note / 12) << 4) + (r->note % 12);
				data[1] = (r->volume & 0x07) | (r->sample << 3);
				data[2] = ((r->volume >> 3) << 4);
				if (place_jump_effect && at_end) {
					data[2] |= 2;
					data[3] = p->jumppos;
					place_jump_effect = false;
				} else {
					data[3] = 0;
				}

				if (need_pattern_break && !place_jump_effect) {
					data[2] |= 3;
					data[3] = 0;
					need_pattern_break = false;
				}
			}
		}
	}
}

int write_stm(FILE* f, song_t* s, const char* n) {
	int result = -1;
	int index = 0;
	static uint8_t sample_headers[STM_MAX_INSTRUMENTS][STM_SAMPLE_HEADER_SIZE];
	long sample_locations[STM_MAX_INSTRUMENTS] = {0};

	{
		static uint8_t song_header[0x30] = {0};
		double tempo_scale = 0.0;

		/* 12048 */
		tempo_scale = log(pow(12048.0, s->speed));
		memcpy(&song_header[0x0014], "6682STM!\x1A", 9);
		if (n != NULL) {
			strncpy((char*)&song_header[0x0000], n, 20);
		}
		song_header[0x001D] = 2;
		song_header[0x001E] = 2;
		song_header[0x001F] = 21;
		song_header[0x0020] = (s->speed + ((size_t)log(s->speed * 2))) << 4;
		song_header[0x0020] |= ((size_t)tempo_scale) & 15;
		song_header[0x0021] = s->total_patterns;
		song_header[0x0022] = 64;
		fwrite(song_header, 1, 48, f);
	}

	fgoto(f, 0x0410);

	{
		uint8_t order_list[128];
		memset(order_list, 99, 128);
		memcpy(order_list, s->orders, s->total_orders);
		fwrite(order_list, 1, 128, f);
	}

	{
		uint8_t* tmp = (uint8_t*)malloc(STM_PATTERN_SIZE);
		if (!tmp) {
			err_puts("cant allocate space for STM pattern");
			goto end;
		}
		for (index = 0; index < s->total_patterns; ++index) {
			prepare_stm_pattern(&s->patterns[index], tmp);
			fwrite(tmp, 4 * 4, 64, f);
		}
		free(tmp);
	}

	for (index = 0; index < STM_MAX_INSTRUMENTS; ++index) {
		const sample_t* smp = &s->samples[index];
		if (smp->length == 0 || smp->data == NULL) {
			continue;
		}
		{
			uint16_t s16 = 0;
			for (; s16 < smp->length; ++s16) {
				smp->data[s16] ^= 128;
			}
		}

		fskip(f, pad(ftell(f) & 15));
		sample_locations[index] = ftell(f);
		fwrite(smp->data, 1, smp->length, f);
	}
	for (index = 0; index < STM_MAX_INSTRUMENTS; ++index) {
		const sample_t* smp = &s->samples[index];
		uint8_t* header = &sample_headers[index][0];
		uint16_t loc = (uint16_t)(sample_locations[index] >> 4UL);
		bool pcm_present = smp->data != NULL && smp->length >= 4;
#if (C6682STM_GENERATE_RANDOM_FILENAME != 0) && (C6682STM_ADVERTISE == 0)
		if (pcm_present) {
			int index_name = 0;
			for (; index_name < 8; ++index_name) {
				header[index_name] = (rand() % 26) + 'A';
			}
			header[index_name++] = '.';
			header[index_name++] = (rand() % 26) + 'A';
			header[index_name++] = (rand() % 26) + 'A';
			header[index_name++] = (rand() % 26) + 'A';
		} else {
			memset(header, '\0', 0xB);
		}
#endif
		header[0x000C] = '\0';
		header[0x000E] = pcm_present ? ((loc >> 0) & 0xff) : 0;
		header[0x000F] = pcm_present ? ((loc >> 8) & 0xff) : 0;
		header[0x0010] = (smp->length >> 0) & 0xff;
		header[0x0011] = (smp->length >> 8) & 0xff;
		header[0x0012] = (smp->loop_start >> 0) & 0xff;
		header[0x0013] = (smp->loop_start >> 8) & 0xff;
		header[0x0014] = smp->loop_end == 0 ? 0xFF : ((smp->loop_end >> 0) & 0xff);
		header[0x0015] = smp->loop_end == 0 ? 0xFF : ((smp->loop_end >> 8) & 0xff);
		header[0x0016] = (pcm_present != false) ? 60 : 0;
		header[0x0018] = 0x00;
		header[0x0019] = 0x21;
	}
	if (C6682STM_ADVERTISE != 0) {
		const struct tm* tm;
		time_t t;

		t = time(NULL);
		tm = gmtime(&t);
		if (tm == NULL) {
#ifdef _POSIX_VERSION
			warn_printf("failed to get a timestamp! - %s", strerror(errno));
#else
			warn_puts("failed to get a timestamp!");
#endif
			goto blank_sample_names;
		}
		index = 0;
		strncpy((char*)&sample_headers[index++][0], "Converted on", 12);
		strftime((char*)&sample_headers[index++][0], 12, "%Y-%m-%d", tm);
		strftime((char*)&sample_headers[index++][0], 12, "T%H:%M:%S", tm);
#ifdef __STDC_VERSION__
		strftime((char*)&sample_headers[index++][0], 12, "%z", tm);
#else
		strftime((char*)&sample_headers[index++][0], 12, "%Z", tm);
#endif
		strncpy((char*)&sample_headers[index++][0], "with 6682STM", 12);
	} else {
	blank_sample_names:
		for (index = 0; index < STM_MAX_INSTRUMENTS; ++index) {
			memset((char*)&sample_headers[index][0], 0, 12);
		}
	}

	fgoto(f, 0x0030);

	fwrite(sample_headers, STM_SAMPLE_HEADER_SIZE, STM_MAX_INSTRUMENTS, f);

	result = 0;
end:
	fflush(f);
	return result;
}
