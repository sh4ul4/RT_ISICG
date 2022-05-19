#pragma once

#include "WhittedIntegrator.hpp"
#include "glm/geometric.hpp"

#define SEMITRANSPARENCY 0
#define PHOTONCAST 0

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax,
								 const float   p_nbLightSamples ) const
	{
		HitRecord hitRecord;
		hitRecord._pixelConeRad = 0.005f;
		hitRecord._pixelConeAlpha = glm::radians(0.0005f);
		return LiRec( 0, false, 1.f, p_scene, p_ray, p_tMin + 0.01f, p_tMax, hitRecord, p_nbLightSamples );
	}

	Vec3f WhittedIntegrator::LiRec( const float	  depth,
									const bool	  inside,
									const float	  refractIdx,
									const Scene & p_scene,
									const Ray &	  p_ray,
									const float	  p_tMin,
									const float	  p_tMax,
									HitRecord& hitRecord,
									const float	  p_nbLightSamples ) const
	{
		Vec3f res( 0.f );
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			// mirror material
			if ( hitRecord._object->getMaterial()->isMirror() && depth <= _nbBounces )
			{
				res += LiRec( depth + 1.f,
							  inside,
							  refractIdx,
							  p_scene,
							  Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) ),
							  p_tMin,
							  p_tMax,
							  hitRecord,
							  p_nbLightSamples );
			}
			// transparent material
			else if ( hitRecord._object->getMaterial()->isTransparent() && depth <= _nbBounces )
			{
				const Vec3f reflectDir( glm::normalize( glm::reflect( p_ray.getDirection(), hitRecord._normal ) ) );

				const float ni	= inside ? hitRecord._object->getMaterial()->getIOR() : refractIdx;
				const float no	= inside ? refractIdx : hitRecord._object->getMaterial()->getIOR();
				const float eta = ni / no;

				const float cosThetaIn = glm::dot( hitRecord._normal, -p_ray.getDirection() );
				if ( ( glm::asin( 1.f / eta ) < glm::acos( cosThetaIn ) ) && ( ni > no ) )
				{
					res += LiRec( depth + 1.f,
								  inside,
								  refractIdx,
								  p_scene,
								  Ray( hitRecord._point, reflectDir ),
								  p_tMin,
								  p_tMax,
								  hitRecord,
								  p_nbLightSamples );
				}
				else
				{
					const Vec3f refractDir(
						glm::normalize( glm::refract( p_ray.getDirection(), hitRecord._normal, eta ) ) );
					const float R = fresnelEquation( refractDir, hitRecord._normal, ni, no, cosThetaIn );
					res += R
							   * LiRec( depth + 1.f,
										inside,
										refractIdx,
										p_scene,
										Ray( hitRecord._point, reflectDir ),
										p_tMin,
										p_tMax,
										hitRecord,
										p_nbLightSamples )
						   + ( 1.f - R )
								 * LiRec( depth + 1.f,
										  !inside,
										  refractIdx,
										  p_scene,
										  Ray( hitRecord._point, refractDir ),
										  p_tMin,
										  p_tMax,
										  hitRecord,
										  p_nbLightSamples );
#if SEMITRANSPARENCY
					const float maxDepth	 = hitRecord._object->getMaxDepth();
					const float transparency = hitRecord._object->getMaterial()->getTransparency();
					const float dist		 = glm::distance( hitRecord._point, p_ray.getOrigin() ) / maxDepth;
					const float factor		 = dist * dist * ( 1.f - ( transparency / 100.f ) );
					if ( inside )
					{
						res = ( ( 1.f - factor ) * res + factor * hitRecord._object->getMaterial()->getFlatColor() );
					}
#endif
				}
			}
			else
			{
				const float cosTheta = glm::dot( -p_ray.getDirection(), hitRecord._normal );
				for ( BaseLight * bl : p_scene.getLights() )
				{
					if ( bl->isSurface()
						 && hitRecord._object->getLight() != bl ) // make light objects ignore themselves
					{
						Vec3f tmp( 0.f );
						for ( size_t i = 0; i < p_nbLightSamples; i++ )
						{
							const LightSample ls = bl->sample( hitRecord._point );
							Ray				  shadowRay( hitRecord._point, -ls._direction );
							if ( !p_scene.intersectAny( shadowRay, 0.001f, ls._distance ) )
							{
								tmp += _directLighting( p_ray, ls, hitRecord, cosTheta );
							}
						}
						tmp /= p_nbLightSamples;
						res += tmp;
#if PHOTONCAST
						const LightSample ls = bl->sample( hitRecord._point );
						const Vec3f		  Lr = _indirectLighting( p_ray, ls, hitRecord, cosTheta );
						res += Lr;
#endif
					}
					else
					{
						const LightSample ls = bl->sample( hitRecord._point );
						Ray				  shadowRay( hitRecord._point, -ls._direction );
						if ( !p_scene.intersectAny( shadowRay, 0.001f, ls._distance ) )
						{
							res += _directLighting( p_ray, ls, hitRecord, cosTheta );
						}
#if PHOTONCAST
						const Vec3f Lr = _indirectLighting( p_ray, ls, hitRecord, cosTheta );
						res += Lr;
#endif
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

	Vec3f WhittedIntegrator::_indirectLighting( const Ray &			ray,
												const LightSample & ls,
												const HitRecord &	hitRecord,
												const float			cosTheta2 ) const
	{
		std::vector<PhotonKd3::Node *> knearest;
		// number of nearest-neighbours to get
		const int					   k = 20; // recommended values lay between 5 and 30
		// knn-search
		pc->kd3.knn( hitRecord._point, k, knearest );
		float r = glm::epsilon<float>(); // maximum radius to englobe all the nearest photons
		Vec3f powSum( 0.f );
		// add photon-effects together
		for ( auto node : knearest )
		{
			float d = glm::distance( node->p.pos, hitRecord._point );
			if ( d > r ) r = d;
			const float cosTheta = glm::dot( -ls._direction, hitRecord._normal );
			powSum += ( 1.f / cbrtf( d ) )
					  * ( hitRecord._object->getMaterial()->shade( ray, hitRecord, ls ) * node->p.pow );
		}
		// normalize power
		const Vec3f Lr = ( powSum / ( PIf * r * r ) );
		return abs( Lr );
	}
} // namespace RT_ISICG