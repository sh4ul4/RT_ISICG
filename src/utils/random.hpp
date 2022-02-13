#ifndef __RT_ISICG_RANDOM__
#define __RT_ISICG_RANDOM__

#include <random>

namespace RT_ISICG
{
	// Return a pseudo random float between 0 and 1
	static inline float randomFloat()
	{
		static std::mt19937							 gen;
		static std::uniform_real_distribution<float> dis( 0.f, 1.f );
		return dis( gen );
	}

} // namespace RT_ISICG

#endif // __RT_ISICG_RANDOM__
