#ifndef __RT_ISICG_STB_INCLUDE__
#define __RT_ISICG_STB_INCLUDE__

// Disable all warnings for stb.
#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#endif

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // __RT_ISICG_STB_INCLUDE__
