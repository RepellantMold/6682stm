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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "log.h"
#include "pattern.h"
#include "song.h"

static void read_sample_668(FILE* f, sample_t* s) {
	fread16(f);
	s->length = fread16(f);
	s->loop_start = fread16(f);
	s->loop_end = fread16(f);

	if (s->length == 0) {
		return;
	}
	s->data = (int8_t*)malloc(s->length);
	if (!s->data) {
		warn_puts("failed to allocate space for PCM data");
	}
}

static void load_pattern_668(FILE* f, pattern_t* p) {
	uint8_t row_index = 0;
	row_t* current_row;
	while (row_index < 64) {
		uint8_t b = fread8(f);
#if 0
		switch(b&15)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		{
			uint8_t d[2];
			b&=3;
			d[0]=fread8(f);
			d[1]=fread8(f);
			current_row=&p->rows[row_index][b&0xf];
			current_row->volume=(d[0]&0xf)<<2;
			current_row->sample=((d[0]>>4)|(GETBIT(d[1],7)<<4))+1;
			current_row->note=(d[1]&0x7f);

			verbose_printf(
				"row %02d channel %1d: n=%02d s=%02d v=%02d",
				row_index+1,b&0xf,
				current_row->note,current_row->sample,current_row->volume
				);
		}
		break;
		case 4:
		{
			uint8_t v;
			v=fread8(f);
			if(row_index+v>=64)
			{
				goto end;
			}
			row_index+=v;
			verbose_printf("row=%02d+%02d",
				       row_index,v);
		}
		break;
		case 5:
		{
			uint8_t v;
			v=fread8(f);
			p->jumppos=v;
		}
			goto end;
			break;
		case 6:
			p->end_of_row=row_index;
			goto end;
			break;
		default:
			warn_printf("unknown command (%02X)",b);
			break;
		}
#else
		if (b == 0x66) {
			continue;
		} else if (b == 4) {
			uint8_t v;
			v = fread8(f);
			if (row_index + v >= 64) {
				goto end;
			}
			row_index += v;
			verbose_printf("row=%02d+%02d", row_index, v);
		} else if (b == 5) {
			uint8_t v;
			v = fread8(f);
			p->jumppos = v;
		} else if (b == 6) {
			p->end_of_row = row_index;
			goto end;
			break;
		} else if (b < 4) {
			uint8_t d[2];
			d[0] = fread8(f);
			d[1] = fread8(f);
			current_row = &p->rows[row_index][b];
			current_row->volume = (d[0] & 0xf) << 2;
			current_row->sample = ((d[0] >> 4) | (GETBIT(d[1], 7) << 4)) + 1;
			current_row->note = (d[1] & 0x7f);

			verbose_printf("row %02d channel %1d: n=%02d s=%02d v=%02d",
				       row_index + 1, b, current_row->note, current_row->sample,
				       current_row->volume);
		} else {
			warn_printf("unknown command %02X", b);
		}
#endif
	}
end:
	verbose_printf("pattern is %d rows long", p->end_of_row + 1);
}

#define HANDLE_EOF_ERROR(f)                                                                    \
	do {                                                                                   \
		if (!fokay(f)) {                                                               \
			if (feof(f)) {                                                         \
				err_puts("unexpected EOF!");                                   \
			} else if (ferror(f)) {                                                \
				err_printf("%s", strerror(errno));                             \
			}                                                                      \
			goto end;                                                              \
		}                                                                              \
	} while (0)

#define HANDLE_EOF_WARNING(f)                                                                  \
	do {                                                                                   \
		if (!fokay(f)) {                                                               \
			if (feof(f)) {                                                         \
				warn_puts("unexpected EOF!");                                  \
			} else if (ferror(f)) {                                                \
				warn_printf("%s", strerror(errno));                            \
			}                                                                      \
		}                                                                              \
	} while (0)

int open_668(FILE* f, song_t* s) {
	int result = -1;
	long first_sample_data_location;
	static uint16_t pattern_locations[64];
	{
		int index = 0;
		s->speed = fread8(f);
		if (!s->speed) {
			goto end;
		}
		s->total_samples = fread8(f);
		if (!s->total_samples) {
			err_printf("invalid number of samples: %d", s->total_samples);
			goto end;
		}
		s->total_orders = fread8(f);
		if (!s->total_orders || s->total_orders > 128) {
			err_printf("invalid number of orders: %d", s->total_orders);
			goto end;
		}
		s->total_patterns = fread8(f);
		if (!s->total_patterns || s->total_patterns > 64) {
			err_printf("invalid number of patterns: %d", s->total_patterns);
			goto end;
		}
		verbose_printf("total orders:   %02d", s->total_orders);
		verbose_printf("total samples:  %02d", s->total_samples);
		verbose_printf("total patterns: %02d", s->total_patterns);
		fskip(f, 2 + s->total_orders);
		for (; index < s->total_patterns; ++index) {
			pattern_locations[index] = (6 + s->total_orders + (s->total_samples * 8)
						    + (s->total_patterns * 2))
						   + fread16(f);
			verbose_printf("pattern %-2d is at location %05d", index + 1,
				       pattern_locations[index]);
		}
	}

	s->samples = (sample_t*)calloc(s->total_samples, sizeof(sample_t));
	if (!s->samples) {
		err_printf("%s", strerror(errno));
		goto end;
	}

	s->patterns = (pattern_t*)calloc(s->total_patterns, sizeof(pattern_t));
	if (!s->patterns) {
		err_printf("%s", strerror(errno));
		goto end;
	}
	{
		int index;
		for (index = 0; index < s->total_patterns; ++index) {
			prepare_pattern(&s->patterns[index]);
		}
	}

	fgoto(f, 0x0004);
	first_sample_data_location = fread16(f);
	s->orders = (uint8_t*)malloc(s->total_orders);
	if (!s->orders) {
		err_printf("%s", strerror(errno));
		goto end;
	}
	fread(s->orders, 1, s->total_orders, f);
	HANDLE_EOF_ERROR(f);
	fskip(f, (s->total_patterns * 2));
	{
		int index;

		for (index = 0; index < s->total_samples; ++index) {
			sample_t* smp = &s->samples[index];
			smp->length = 0;
			smp->loop_start = 0;
			smp->loop_end = 0xFFFF;
			read_sample_668(f, smp);
			HANDLE_EOF_ERROR(f);
			verbose_printf("sample %02d: length=%05d loopstart=%05d loopend=%05d",
				       index + 1, smp->length, smp->loop_start, smp->loop_end);
		}

		for (index = 0; index < s->total_patterns; ++index) {
			uint16_t location = pattern_locations[index];
			fgoto(f, location);
			verbose_printf("reading pattern %02d", index + 1);
			load_pattern_668(f, &s->patterns[index]);
		}

		fgoto(f, first_sample_data_location);
		HANDLE_EOF_ERROR(f);
		for (index = 0; index < s->total_samples; ++index) {
			sample_t* smp = &s->samples[index];

			if (smp->data == NULL || smp->length == 0) {
				continue;
			}
			fread(smp->data, 1, smp->length, f);
			HANDLE_EOF_WARNING(f);
			while (ftell(f) & 15) {
				fskip(f, 1);
			}
		}
	}

	result = 0;
end:
	fflush(f);
	return result;
}
