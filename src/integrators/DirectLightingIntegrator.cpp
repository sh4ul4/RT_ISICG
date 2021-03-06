#pragma once

#include "DirectLightingIntegrator.hpp"

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax,
										const float	  p_nbLightSamples ) const
	{
		HitRecord hitRecord;
		Vec3f	  res( 0.f );
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			const float cosTheta = glm::dot( -p_ray.getDirection(), hitRecord._normal );
			for ( BaseLight * bl : p_scene.getLights() )
			{
				if ( bl->isSurface() )
				{
					Vec3f tmp( 0.f );
					for ( size_t i = 0; i < p_nbLightSamples; i++ )
					{
						const LightSample ls = bl->sample( hitRecord._point );
						Ray shadowRay( hitRecord._point, -ls._direction );
						shadowRay.offset( hitRecord._normal );
						if ( !p_scene.intersectAny( shadowRay, 0, ls._distance ) )
							tmp += _directLighting( p_ray, ls, hitRecord, cosTheta );
					}
					tmp /= p_nbLightSamples;
					res += tmp;
				}
				else
				{
					const LightSample ls = bl->sample( hitRecord._point );
					Ray shadowRay( hitRecord._point, -ls._direction );
					shadowRay.offset( hitRecord._normal );
					if ( !p_scene.intersectAny( shadowRay, 0, ls._distance ) )
						res += _directLighting( p_ray, ls, hitRecord, cosTheta );
				}
			}
			return res;
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f DirectLightingIntegrator::_directLighting( const Ray& ray,
													 const LightSample & ls,
													 const HitRecord &	 hitRecord,
													 const float		 cosTheta2 ) const
	{
		const float cosTheta = glm::dot( -ls._direction, hitRecord._normal );
		return hitRecord._object->getMaterial()->shade( ray, hitRecord, ls ) * ls._radiance * cosTheta;
	}
} // namespace RT_ISICG