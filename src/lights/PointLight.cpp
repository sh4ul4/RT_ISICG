#pragma once

#include "PointLight.hpp"

namespace RT_ISICG
{
	LightSample PointLight::sample( const Vec3f & p_point ) const
	{
		const float pdf		  = 1.f;
		const float distance  = glm::distance( position, p_point );
		const Vec3f direction = glm::normalize( position - p_point );
		const Vec3f radiance  = _color * _power * ( ( 1 / distance ) * ( 1 / distance ) );
		return LightSample( direction, distance, radiance, pdf );
	}
} // namespace RT_ISICG