#pragma once

#include "base_integrator.hpp"
#include "lights/PointLight.hpp"

namespace RT_ISICG
{
	class DirectLightingIntegrator : public BaseIntegrator
	{
	  public:
		DirectLightingIntegrator() : BaseIntegrator() {}
		virtual ~DirectLightingIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::DIRECT_LIGHTING; }

		Vec3f Li( const Scene & p_scene,
				  const Ray &	p_ray,
				  const float	p_tMin,
				  const float	p_tMax, const float p_nbLightSamples = 1 ) const;

	  private:
		Vec3f _directLighting( const LightSample & ls, const HitRecord & hitRecord, const float cosTheta ) const;
	};
} // namespace RT_ISICG