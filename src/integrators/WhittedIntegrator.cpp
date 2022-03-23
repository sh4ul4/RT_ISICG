#pragma once

#include "WhittedIntegrator.hpp"
#include "glm/geometric.hpp"

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax,
								 const float   p_nbLightSamples ) const
	{
		return LiRec( 0, false, 1.f, p_scene, p_ray, p_tMin, p_tMax, p_nbLightSamples );
	}

	Vec3f WhittedIntegrator::LiRec( const float	  depth,
		const bool inside,
									const float	  refractIdx,
									const Scene & p_scene,
									const Ray &	  p_ray,
									const float	  p_tMin,
									const float	  p_tMax,
									const float	  p_nbLightSamples ) const
	{
		HitRecord hitRecord;
		Vec3f	  res( 0.f );
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			// mirror material
			if ( hitRecord._object->getMaterial()->isMirror() && depth <= _nbBounces )
			{
				res += LiRec( depth + 1,
							inside,
							  refractIdx,
							  p_scene,
							  Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) ),
							  p_tMin + 0.01f,
							  p_tMax,
							  p_nbLightSamples );
			}
			// transparent material
			else if ( hitRecord._object->getMaterial()->isTransparent() && depth <= _nbBounces )
			{
				const float fresnel = fresnelEquation(
					p_ray.getDirection(), hitRecord._normal, hitRecord._object->getMaterial()->getIOR() );
				// reflect
				res += fresnel
					   * LiRec( depth + 1,
								inside,
								refractIdx,
								p_scene,
								Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) ),
								p_tMin + 0.01f,
								p_tMax,
								p_nbLightSamples );
				// transmit
				res += ( 1.f - fresnel )
					   * LiRec( depth + 1,
								!inside,
								hitRecord._object->getMaterial()->getIOR(),
								p_scene,
								Ray( hitRecord._point,
									 glm::refract( p_ray.getDirection(),
												   hitRecord._normal,
												   refractIdx / hitRecord._object->getMaterial()->getIOR() ) ),
								p_tMin + 0.01f,
								p_tMax,
								p_nbLightSamples );
			}
			else
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
							Ray				  shadowRay( hitRecord._point, -ls._direction );
							shadowRay.offset( hitRecord._normal );
							if ( !p_scene.intersectAny( shadowRay, 0.f, ls._distance ) )
							{
								tmp += _directLighting( p_ray, ls, hitRecord, cosTheta );
							}
						}
						tmp /= p_nbLightSamples;
						res += tmp;
					}
					else
					{
						const LightSample ls = bl->sample( hitRecord._point );
						Ray				  shadowRay( hitRecord._point, -ls._direction );
						shadowRay.offset( hitRecord._normal );
						if ( !p_scene.intersectAny( shadowRay, 0.f, ls._distance ) )
						{
							res += _directLighting( p_ray, ls, hitRecord, cosTheta );
						}
					}
				}
			}
			return res;
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f WhittedIntegrator::_directLighting( const Ray &		  ray,
											  const LightSample & ls,
											  const HitRecord &	  hitRecord,
											  const float		  cosTheta2 ) const
	{
		const float cosTheta = glm::dot( -ls._direction, hitRecord._normal );
		return hitRecord._object->getMaterial()->shade( ray, hitRecord, ls ) * ls._radiance * cosTheta;
	}
} // namespace RT_ISICG