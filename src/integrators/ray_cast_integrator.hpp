#ifndef __RT_ISICG_RAY_CAST_INTEGRATOR__
#define __RT_ISICG_RAY_CAST_INTEGRATOR__

#include "base_integrator.hpp"

namespace RT_ISICG
{
	class RayCastIntegrator : public BaseIntegrator
	{
	  public:
		RayCastIntegrator() : BaseIntegrator() {}
		virtual ~RayCastIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::RAY_CAST; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_RAY_CAST_INTEGRATOR__
