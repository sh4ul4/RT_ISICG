#pragma once
#include "defines.hpp"
#include "base_light.hpp"
#include "light_sample.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	class TriLight : public BaseLight
	{
		Vec3f _a, _b, _c, _n;
		float _area;

	  public:
		TriLight() = delete;
		TriLight( const Vec3f & a, const Vec3f & b, const Vec3f & c, const Vec3f & color, const float power = 1.f );
		LightSample sample( const Vec3f & p_point ) const;
		Vec3f		getPos() const { return _a; }
		Vec3f		getEmissionFlux() const
		{
			const Vec3f le = _color * _power;
			return le;
		}
	};
} // namespace RT_ISICG