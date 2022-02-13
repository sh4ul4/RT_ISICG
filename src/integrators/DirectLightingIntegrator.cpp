#pragma once

#include "DirectLightingIntegrator.hpp"

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax ) const
	{
		HitRecord	hitRecord;
		Vec3f		res( 0.f );
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			const float cosTheta = glm::dot( -p_ray.getDirection(), hitRecord._normal );
			for ( BaseLight * bl : p_scene.getLights() )
			{
				const LightSample ls = bl->sample( hitRecord._point );
				const Ray		  shadowRay( hitRecord._point, ls._direction );
				HitRecord		  tmp;
				//if ( !p_scene.intersect( shadowRay, 0, ls._distance, tmp ) )
					res += _directLighting( ls, hitRecord, cosTheta );
			}
			return res;
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f DirectLightingIntegrator::_directLighting( const LightSample& ls,
													 const HitRecord &		 hitRecord,
													 const float			 cosTheta ) const
	{
		const float cosTheta2 = glm::dot( -ls._direction, hitRecord._normal );
		return hitRecord._object->getMaterial()->getFlatColor() * ls._radiance * cosTheta2;
	}
} // namespace RT_ISICG