#pragma once

#include "PlaneGeometry.hpp"

namespace RT_ISICG
{
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t ) const {
		// On suppose les vecteurs normalises
		float d = glm::dot( normal, -p_ray.getDirection());
		// Si p et r ne sont pas parall�les
		if ( d > 0 )
		{ // Attention � la comparaison avec 0
			Vec3f po = point - p_ray.getOrigin();
			//p_t = glm::dot( po, normal ) / d;
			p_t = glm::dot(po, normal) / glm::dot( p_ray.getDirection(), normal );
			return true;
		}
		return false;

	}
} // namespace RT_ISICG