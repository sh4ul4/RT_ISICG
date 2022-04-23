#pragma once

#include "WhittedIntegrator.hpp"
#include "glm/geometric.hpp"

#define SEMITRANSPARENCY 0
#define PHOTONCAST 1

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax,
								 const float   p_nbLightSamples ) const
	{
		return LiRec( 0, false, 1.f, p_scene, p_ray, p_tMin + 0.01f, p_tMax, p_nbLightSamples );
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
							  p_tMin,
							  p_tMax,
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
										p_nbLightSamples )
						   + ( 1.f - R )
								 * LiRec( depth + 1.f,
										  !inside,
										  refractIdx,
										  p_scene,
										  Ray( hitRecord._point, refractDir ),
										  p_tMin,
										  p_tMax,
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
					if ( bl->isSurface() && hitRecord._object->getLight() != bl ) // make light objects ignore themselves
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
#if PHOTONCAST
							tmp += _indirectLighting( p_ray, ls, hitRecord, cosTheta );
#endif
						}
						tmp /= p_nbLightSamples;
						res += tmp;
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
						res += _indirectLighting( p_ray, ls, hitRecord, cosTheta );
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

	Vec3f WhittedIntegrator::_indirectLighting( const Ray &		   ray,
												   const LightSample & ls,
												   const HitRecord &   hitRecord,
												   const float		   cosTheta2 ) const
	{
		std::vector<PhotonKd3::Node *> knearest;
		int							   k = 5;
		pc->kd3.knn( hitRecord._point, k, knearest );
		float r = 0.f;
		Vec3f s( 0.f );
		for ( auto node : knearest )
		{
			float d = glm::distance( node->p.pos, hitRecord._point );
			d		= ( d * d ) / 2000000.f;
			//if ( d < 0.01f )
			//{
				if ( d > r ) r = d;
				// s += node->p.pow*1000.f;
				s += ( node->p.pow / ( 0.00001f+ d ) );
			//}
			//else k--;
		}
		const Vec3f Lr = s / (float)k; // INV_PIf * r * r * ( s / (float)nclosest );
		//std::cout << Lr.r << " " << Lr.g << " " << Lr.b << std::endl;
		return glm::clamp( Lr, 0.f, 255.f );
	}
} // namespace RT_ISICG