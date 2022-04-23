#pragma once

#include "base_integrator.hpp"
#include "lights/PointLight.hpp"
#include "PhotonKd3.hpp"

namespace RT_ISICG
{
	class PhotonCastIntegrator
	{
	  public:
		PhotonCastIntegrator() {}
		virtual ~PhotonCastIntegrator() = default;

		void Li( const Scene &					   p_scene,
				  const Ray &					   p_ray,
				  const float					   p_tMin,
				  const float					   p_tMax,
				  const float					   p_nbLightSamples,
				 std::vector<PhotonKd3::Photon> & photons,
				 const BaseLight *				  bl ) const;

	  private:
		void LiRec( const float   depth,
					 const bool	   inside,
					 const float   refractIdx,
					 const Scene & p_scene,
					 const Ray &   p_ray,
					 const float   p_tMin,
					 const float   p_tMax,
					 const float   p_nbLightSamples,
					std::vector<PhotonKd3::Photon> & photons,
					const BaseLight *				 bl ) const;

		size_t _nbBounces = 20;

	  private:
		float fresnelEquation( const Vec3f & I,
							   const Vec3f & N,
							   const float	 ni,
							   const float	 no,
							   const float	 cosThetaIn ) const
		{
			const float cosThetaOut = glm::dot( -N, I );
			const float Rs			= ( no * cosThetaIn - ni * cosThetaOut ) / ( no * cosThetaIn + ni * cosThetaOut );
			const float Rp			= ( ni * cosThetaIn - no * cosThetaOut ) / ( ni * cosThetaIn + no * cosThetaOut );
			return ( Rs * Rs + Rp * Rp ) * 0.5f;
		}
	};
} // namespace RT_ISICG