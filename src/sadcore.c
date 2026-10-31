#include <stdarg.h>
#include <stdio.h>

#include "sadcore.h"

#if defined(SAD_CC_MSVC)
#include <intrin.h>
#elif defined(SAD_CC_GCC)

#elif defined(SAD_CC_CLANG)

#endif

/*
* Error handling
* ----------------------------------------------------------------------------------------------- */

void sad_throw(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	fflush(stdout);
	abort();
}

/*
* Special type functions
* ----------------------------------------------------------------------------------------------- */

sad_int sad_clz(sad_uint val)
{
#if defined(SAD_CC_MSVC)
	unsigned long index;
	if (_BitScanReverse(&index, val))
		return 31 - index;
	return 32;
	
#elif defined(SAD_CC_GCC) || defined(SAD_CC_CLANG)
	return __builtin_clz(val);
	
#else
	sad_int map[4] = { 2, 1, 0, 0 };
	sad_int result = 0;

	if ((val & 0xFFFF0000) == 0)
	{
		result += 16;
		val <<= 16;
	}

	if ((val & 0xFF000000) == 0)
	{
		result += 8;
		val <<= 8;
	}

	if ((val & 0xF0000000) == 0)
	{
		result += 4;
		val <<= 4;
	}

	if ((val & 0xC0000000) == 0)
	{
		result += 2;
		val <<= 2;
	}

	result += map[val >> 30];
	return result;

#endif
}

sad_int sad_ctz(sad_uint val)
{
#if defined(SAD_CC_MSVC)
	unsigned long index;
	if (_BitScanForward(&index, val))
		return index;
	return 32;

#elif defined(SAD_CC_GCC) || defined(SAD_CC_CLANG)
	return __builtin_ctz(val);

#else
	sad_int map[4] = { 2, 0, 1, 0 };
	sad_int result = 0;

	if ((val & 0xFFFF) == 0)
	{
		result += 16;
		val >>= 16;
	}

	if ((val & 0xFF) == 0)
	{
		result += 8;
		val >>= 8;
	}

	if ((val & 0xF) == 0)
	{
		result += 4;
		val >>= 4;
	}

	if ((val & 0x3) == 0)
	{
		result += 2;
		val >>= 2;
	}

	result += map[val & 3];
	return result;
#endif
}

size_t sad_align(size_t val, sad_int alignment)
{
	sad_int mask = alignment - 1;
	sad_int rest = (sad_int)val & mask;
	return val + ((alignment - rest) * (rest != 0));
}
