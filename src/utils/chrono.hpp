#ifndef __RT_ISICG_CHRONO__
#define __RT_ISICG_CHRONO__

#ifdef _MSC_VER
#pragma once
#endif

#include <chrono>

namespace RT_ISICG
{
	class Chrono
	{
		using Clock	   = std::chrono::high_resolution_clock;
		using Duration = std::chrono::duration<float>;

	  public:
		void  start() { begin = Clock::now(); }
		void  stop() { end = Clock::now(); }
		float elapsedTime() const { return ( std::chrono::duration_cast<Duration>( end - begin ) ).count(); }

	  private:
		Clock::time_point begin;
		Clock::time_point end;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_CHRONO__
