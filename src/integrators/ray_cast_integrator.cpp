#include "ray_cast_integrator.hpp"

namespace RT_ISICG
{
	Vec3f RayCastIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			/// TODO ! cos theta...
			float cosTheta		= glm::dot( -p_ray.getDirection(), hitRecord._normal );
			float diffuseFactor = std::max( cosTheta, 0.f );
			return diffuseFactor * hitRecord._object->getMaterial()->getFlatColor();
		}
		else
		{
			return _backgroundColor;
		}
	}
} // namespace RT_ISICG
