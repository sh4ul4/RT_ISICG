#ifndef __RT_ISICG_BRDF_LAMBERT__
#define __RT_ISICG_BRDF_LAMBERT__

#include "defines.hpp"

namespace RT_ISICG
{
	class LambertBRDF
	{
	  public:
		LambertBRDF( const Vec3f & p_kd ) : _kd( p_kd ) {};

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate() const { return _kd * INV_PIf; }

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_LAMBERT__
