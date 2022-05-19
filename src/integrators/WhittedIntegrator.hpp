#pragma once

#include "base_integrator.hpp"
#include "lights/PointLight.hpp"
#include "PhotonCaster.hpp"

namespace RT_ISICG
{
	class WhittedIntegrator : public BaseIntegrator
	{
	  public:
		WhittedIntegrator() : BaseIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::WHITTED_LIGHTING; }

		Vec3f Li( const Scene & p_scene,
				  const Ray &	p_ray,
				  const float	p_tMin,
				  const float	p_tMax,
				  const float	p_nbLightSamples = 1 ) const;

		PhotonCaster * pc = nullptr;

	  private:
		Vec3f LiRec( const float   depth,
					 const bool inside,
					 const float   refractIdx,
					 const Scene & p_scene,
					 const Ray &   p_ray,
					 const float   p_tMin,
					 const float   p_tMax,
					 HitRecord& hitRecord,
					 const float   p_nbLightSamples = 1 ) const;

		Vec3f _directLighting( const Ray &		   ray,
							   const LightSample & ls,
							   const HitRecord &   hitRecord,
							   const float		   cosTheta ) const;

		Vec3f _indirectLighting( const Ray &		 ray,
								 const LightSample & ls,
								 const HitRecord &	 hitRecord,
								 const float		 cosTheta ) const;

		size_t _nbBounces = 5;

	  private:
		float fresnelEquation( const Vec3f & I, const Vec3f & N, const float ni, const float no, const float cosThetaIn ) const
		{
			const float cosThetaOut = glm::dot( -N, I );
			const float Rs			= ( no * cosThetaIn - ni * cosThetaOut ) / ( no * cosThetaIn + ni * cosThetaOut );
			const float Rp			= ( ni * cosThetaIn - no * cosThetaOut ) / ( ni * cosThetaIn + no * cosThetaOut );
			return ( Rs * Rs + Rp * Rp ) * 0.5f;
		}
	};
} // namespace RT_ISICG