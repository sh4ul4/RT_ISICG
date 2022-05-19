#include "PhotonCastIntegrator.hpp"
#include "glm/geometric.hpp"

namespace RT_ISICG
{
	void PhotonCastIntegrator::Li( const Scene &					p_scene,
								   const Ray &						p_ray,
								   const float						p_tMin,
								   const float						p_tMax,
								   std::vector<PhotonKd3::Photon> & photons,
								   const BaseLight *				bl ) const
	{
		LiRec( 0.f, false, 1.f, p_scene, p_ray, p_tMin + 0.01f, p_tMax, photons, bl );
	}

	void PhotonCastIntegrator::LiRec( const float					   depth,
									  const bool					   inside,
									  const float					   refractIdx,
									  const Scene &					   p_scene,
									  const Ray &					   p_ray,
									  const float					   p_tMin,
									  const float					   p_tMax,
									  std::vector<PhotonKd3::Photon> & photons,
									  const BaseLight *				   bl ) const
	{
		HitRecord hitRecord;
		Vec3f	  res( 0.f );
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if ( hitRecord._object->getLight() == bl )
			{
				LiRec( depth,
					   inside,
					   refractIdx,
					   p_scene,
					   Ray( hitRecord._point, p_ray.getDirection() ),
					   p_tMin + 0.001f,
					   p_tMax,
					   photons,
					   bl );
			}
			// mirror material
			if ( hitRecord._object->getMaterial()->isMirror() && depth <= _nbBounces )
			{
				LiRec( depth + 1.f,
					   inside,
					   refractIdx,
					   p_scene,
					   Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) ),
					   p_tMin,
					   p_tMax,
					   photons,
					   bl );
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
					LiRec( depth + 1.f,
						   inside,
						   refractIdx,
						   p_scene,
						   Ray( hitRecord._point, reflectDir ),
						   p_tMin,
						   p_tMax,
						   photons,
						   bl );
				}
				else
				{
					const Vec3f refractDir(
						glm::normalize( glm::refract( p_ray.getDirection(), hitRecord._normal, eta ) ) );
					const float R = fresnelEquation( refractDir, hitRecord._normal, ni, no, cosThetaIn );
					std::vector<PhotonKd3::Photon> tmp;
					LiRec( depth + 1.f,
						   inside,
						   refractIdx,
						   p_scene,
						   Ray( hitRecord._point, reflectDir ),
						   p_tMin,
						   p_tMax,
						   tmp,
						   bl );
					for ( const auto & p : tmp )
						photons.emplace_back( p.pos, p.pow * R );
					tmp.clear();
					LiRec( depth + 1.f,
						   !inside,
						   refractIdx,
						   p_scene,
						   Ray( hitRecord._point, refractDir ),
						   p_tMin,
						   p_tMax,
						   tmp,
						   bl );
					for ( const auto & p : tmp )
						photons.emplace_back( p.pos, p.pow * ( 1.f - R ) );
				}
			}
			else if ( depth > 0.f )
			{
				photons.emplace_back( hitRecord._point - p_ray.getDirection() * 0.001f, bl->getEmissionFlux() );
			}
		}
	}
} // namespace RT_ISICG