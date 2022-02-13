#pragma once

#include "base_object.hpp"
#include "geometry/PlaneGeometry.hpp"

namespace RT_ISICG
{
	class Plane : public BaseObject
	{
	  public:
		Plane()		  = delete;
		virtual ~Plane() = default;

		Plane( const std::string & p_name, const Vec3f & p_center, const Vec3f & normal )
			: BaseObject( p_name ), _geometry( p_center, normal )
		{
		}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

	  private:
		PlaneGeometry _geometry;
	};
} // namespace RT_ISICG