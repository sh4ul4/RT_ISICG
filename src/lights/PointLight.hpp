#pragma once
#include "defines.hpp"
#include "base_light.hpp"
#include "light_sample.hpp"

namespace RT_ISICG
{
	class PointLight : public BaseLight
	{
		Vec3f position;

	  public:
		PointLight() = delete;
		PointLight( const Vec3f & position, const Vec3f & color, const float power = 1.f )
			: position( position ), BaseLight( color, power )
		{
		}
		inline const Vec3f & getPosition() { return position; }
		LightSample			 sample( const Vec3f & p_point ) const;
	};
} // namespace RT_ISICG