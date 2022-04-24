#pragma once
#include "defines.hpp"
#include "base_light.hpp"
#include "light_sample.hpp"
#include "utils/random.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace RT_ISICG
{
	class GlobeLight : public BaseLight
	{
		Vec3f _position;
		float _radius, _area;

		bool intersectGlobe( float & _t1, const Vec3f & _rayP, const Vec3f & _circleP ) const;

	  public:
		GlobeLight() = delete;
		GlobeLight( const Vec3f & position, const float radius, const Vec3f & color, const float power = 1.f );
		LightSample sample( const Vec3f & p_point ) const;
		Vec3f		getPos() const { return _position; }
		Vec3f		getEmissionFlux() const // not finished
		{
			const Vec3f le = _color * _power;
			return le;
		}
	};
} // namespace RT_ISICG
