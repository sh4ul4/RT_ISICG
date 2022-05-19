#pragma once

#include "PlaneGeometry.hpp"

#define BACKFACECULLING 1

namespace RT_ISICG
{
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t ) const {
		float d = glm::dot( normal, -p_ray.getDirection());
		if ( 
#if BACKFACECULLING
			d > 0.f 
#else
			d != 0.f 
#endif 
			)
		{
			Vec3f po = point - p_ray.getOrigin();
			p_t = glm::dot(po, normal) / glm::dot( p_ray.getDirection(), normal );
			return true;
		}
		return false;
	}
} // namespace RT_ISICG