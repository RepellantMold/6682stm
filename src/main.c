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

#include "options.h"

#include "types.h"

#if C6682STM_WIN32 != 0
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include "c89stringutils_string_extras.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "file.h"
#include "log.h"
#include "pattern.h"
#include "song.h"

int open_668(FILE* f, song_t* s);
int write_stm(FILE* f, song_t* s, const char* n);

#if C6682STM_UNIX != 0
#include <sys/stat.h>

/* https://en.cppreference.com/w/c/io/setvbuf.html */
int get_and_set_ideal_buffer_size(FILE* fp) {
	struct stat stats;
	if (!fp) {
		return EXIT_FAILURE;
	}
	if (fstat(fileno(fp), &stats) == -1) {
		perror("fstat");
		return EXIT_FAILURE;
	}

	if (setvbuf(fp, NULL, _IOFBF, stats.st_blksize) != 0) {
		perror("setvbuf");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
#else
#define get_and_set_ideal_buffer_size(fp) setvbuf(fp, NULL, _IOFBF, BUFSIZ)
#endif

int main(int argc, char** argv) {
	FILE *in = NULL, *out = NULL;
	char *in_name = NULL, *out_name = NULL;
	song_t* song = NULL;
	int result = -1;

#if C6682STM_GENERATE_RANDOM_FILENAME != 0
#if C6682STM_UNIX != 0 && _POSIX_VERSION == 200809L
	srand(668 ^ time(NULL) ^ getpid());
#else
	srand(668 ^ time(NULL));
#endif
#endif

	if (setvbuf(stdout, NULL, _IONBF, 0) != 0) {
		goto end;
	}
	if (setvbuf(stderr, NULL, _IONBF, 0) != 0) {
		goto end;
	}

	if (argc != 3) {
		printf("usage: %s input output.stm\n", argv[0]);
		goto end;
	}

	in_name = argv[1];
	out_name = argv[2];

	if (!strcasecmp(in_name, out_name)) {
		err_printf("cannot do the conversion");
		goto end;
	}

	in = fopen(in_name, "rb");
	if (!in) {
		err_printf("%s cannot be opened; %s", in_name, strerror(errno));
		goto end;
	}
	if (get_and_set_ideal_buffer_size(in) != 0) {
		goto end;
	}

	song = (song_t*)calloc(1, sizeof(song_t));
	if (!song) {
		err_puts("cant allocate space for song structure");
		goto end;
	}

	if (open_668(in, song) != 0) {
		err_printf("failed to open 668");
		goto end;
	}
	puts("successfully loaded");
	fclose(in);
	in = NULL;

	out = fopen(out_name, "wb");
	if (!out) {
		err_printf("%s cannot be opened; %s", out_name, strerror(errno));
		goto end;
	}
	if (get_and_set_ideal_buffer_size(out) != 0) {
		goto end;
	}

	{
		char* cool;
		bool has_backslash = strrchr(in_name, '\\') != NULL;
		bool has_forwardslash = strrchr(in_name, '/') != NULL;
		cool = has_backslash || has_forwardslash
			   ? strrchr(in_name, ('/' * has_forwardslash) + ('\\' * has_backslash))
				 + 1
			   : in_name;
		if (write_stm(out, song, cool) != 0) {
			err_puts("failed to write STM");
			goto end;
		}
	}

	puts("successfully converted\n"
	     "have a good day :)");

	result = 0;
end:
	if (song) {
		int index;
		if (song->patterns) {
			free(song->patterns);
		}
		for (index = 0; index < song->total_samples; ++index) {
			if (song->samples[index].data) {
				free(song->samples[index].data);
			}
		}
		if (song->samples) {
			free(song->samples);
		}
		if (song->orders) {
			free(song->orders);
		}
	}
	if (in) {
		fclose(in);
	}
	if (out) {
		fclose(out);
	}
	return result;
}

#ifdef C6682STM_UNITY_BUILD
#include "668.c"
#include "file.c"
#include "log.c"
#include "pattern.c"
#include "stm.c"
#endif
