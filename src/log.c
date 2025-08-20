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

#include <stdarg.h>
#include <stdio.h>

#if C6682STM_USE_STDOUT != 0
#define LOG_FILE stdout
#else
#define LOG_FILE stderr
#endif

int err_printf(const char* fmt, ...) {
	va_list args;
	int result = 0;
	va_start(args, fmt);
	fputs("ERROR: ", LOG_FILE);
	result = vfprintf(LOG_FILE, fmt, args);
	fputc('\n', LOG_FILE);
	va_end(args);
	return result;
}

int warn_printf(const char* fmt, ...) {
	va_list args;
	int result = 0;
	va_start(args, fmt);
	fputs("WARNING: ", LOG_FILE);
	result = vfprintf(LOG_FILE, fmt, args);
	fputc('\n', LOG_FILE);
	va_end(args);
	return result;
}

int verbose_printf(const char* fmt, ...) {
	va_list args;
	int result = 0;
	va_start(args, fmt);
#ifndef NDEBUG
	fputs("VERBOSE: ", LOG_FILE);
	result = vfprintf(LOG_FILE, fmt, args);
	fputc('\n', LOG_FILE);
#else
	(void)fmt;
#endif
	va_end(args);
	return result;
}

int err_puts(const char* str) {
	int result = 0;
	fputs("ERROR: ", LOG_FILE);
	result = fputs(str, LOG_FILE);
	fputc('\n', LOG_FILE);
	return result;
}

int warn_puts(const char* str) {
	int result = 0;
	fputs("WARNING: ", LOG_FILE);
	result = fputs(str, LOG_FILE);
	fputc('\n', LOG_FILE);
	return result;
}

int verbose_puts(const char* str) {
	int result = 0;
#ifndef NDEBUG
	fputs("VERBOSE: ", LOG_FILE);
	result = fputs(str, LOG_FILE);
	fputc('\n', LOG_FILE);
#else
	(void)str;
#endif
	return result;
}
