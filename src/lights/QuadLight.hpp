#pragma once
#include "defines.hpp"
#include "base_light.hpp"
#include "light_sample.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
		Vec3f _position, _u, _v, _n, _area;

	  public:
		QuadLight( const Vec3f & position,
				   const Vec3f & u,
				   const Vec3f & v,
				   const Vec3f & p_color,
				   const float	 p_power = 1.f );
		LightSample sample( const Vec3f & p_point ) const;
	};
} // namespace RT_ISICG