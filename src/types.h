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

#ifndef E93AD7BC_8B50_4277_AB11_E479F3DC0395
#define E93AD7BC_8B50_4277_AB11_E479F3DC0395

#define C6682STM_WIN32                                                                         \
	(__WINDOWS__ != 0 || __WIN32__ != 0 || _WIN32 != 0 || _WIN64 != 0)                     \
	    || (_MSC_VER != 0 || _MSC_FULL_VER != 0 || _MSC_BUILD != 0) || (__MINGW32__ != 0)  \
	    || (__MINGW64__ != 0)
#define C6682STM_UNIX (_POSIX_SOURCE != 0)

#define C6682STM_ANSI (__cplusplus == 0) || (__STDC_VERSION__ == 0) || (__STDC__ != 0)

#if C6682STM_ANSI == 0
#include <stdbool.h>
#include <stdint.h>
#else
/* going by the guaranteed size */
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned long uint32_t;
typedef signed long int32_t;

typedef enum { false, true } bool;
#endif

#ifndef COUNT_OF
#define COUNT_OF(a) sizeof((a)) / sizeof((a)[0])
#endif

/* https://stackoverflow.com/a/58532788 */
#ifndef __GNU__
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#else
#ifndef MAX
#define MAX(a, b)                                                                              \
	({                                                                                     \
		__typeof__(a) _a = (a);                                                        \
		__typeof__(b) _b = (b);                                                        \
		_a > _b ? _a : _b;                                                             \
	})
#endif

#ifndef MIN
#define MIN(a, b)                                                                              \
	({                                                                                     \
		__typeof__(a) _a = (a);                                                        \
		__typeof__(b) _b = (b);                                                        \
		_a < _b ? _a : _b;                                                             \
	})
#endif
#endif

#ifndef CLAMP
#define CLAMP(n, l, h) (MAX((l), MIN((h), (n))))
#endif

#ifndef GETBIT
#define GETBIT(x, n) (((x) >> (n)) & 1)
#endif

#endif /* E93AD7BC_8B50_4277_AB11_E479F3DC0395 */
