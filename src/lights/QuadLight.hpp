#pragma once
#include "defines.hpp"
#include "base_light.hpp"
#include "light_sample.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
		Vec3f _position, _u, _v, _n;
		float _area;

	  public:
		QuadLight() = delete;
		QuadLight( const Vec3f & position,
				   const Vec3f & u,
				   const Vec3f & v,
				   const Vec3f & color,
				   const float	 power = 1.f );
		LightSample sample( const Vec3f & p_point ) const;
		Vec3f		getPos() const { return Vec3f( _position + _u * randomFloat() + _v * randomFloat() ); }
		Vec3f		getEmissionFlux() const
		{
			const Vec3f le = (_color / 255.f) * _power;
			return le;
		}
	};
} // namespace RT_ISICG