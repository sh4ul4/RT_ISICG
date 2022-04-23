#ifndef __RT_ISICG_RANDOM__
#define __RT_ISICG_RANDOM__

#include <random>

namespace RT_ISICG
{
	static size_t _currentRand = 0;
	class Rand
	{
	  public:
		static std::vector<float> diss;
		static const size_t		  _maxRand = 2000000;
	};
	static void generateRandoms()
	{
		static std::mt19937 gen;
		for ( size_t i = 0; i < Rand::_maxRand; i++ )
		{
			static std::uniform_real_distribution<float> dis( 0.f, 1.f );
			Rand::diss.emplace_back( dis( gen ) );
		}
	}
	// Return a pseudo random float between 0 and 1
	static float randomFloat()
	{
		/*static std::mt19937						 gen;
		static std::uniform_real_distribution<float> dis( 0.f, 1.f );
		return dis( gen );*/
		_currentRand = ( _currentRand + 1 ) % ( Rand::_maxRand - 1 );
		return Rand::diss[ _currentRand ];
	}
} // namespace RT_ISICG

#endif // __RT_ISICG_RANDOM__
