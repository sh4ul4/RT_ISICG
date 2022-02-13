#ifndef __RT_ISICG_SPHERE__
#define __RT_ISICG_SPHERE__

#include "base_object.hpp"
#include "geometry/sphere_geometry.hpp"

namespace RT_ISICG
{
	class Sphere : public BaseObject
	{
	  public:
		Sphere()		  = delete;
		virtual ~Sphere() = default;

		Sphere( const std::string & p_name, const Vec3f & p_center, const float p_radius )
			: BaseObject( p_name ), _geometry( p_center, p_radius )
		{
		}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

	  private:
		SphereGeometry _geometry;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE__
