#pragma once

#include "base_object.hpp"
#include "geometry/sphere_geometry.hpp"
#include "lights/GlobeLight.hpp"

namespace RT_ISICG
{
	class SphereLightObject : public BaseObject
	{
	  public:
		SphereLightObject()			 = delete;
		virtual ~SphereLightObject() = default;

		SphereLightObject( const std::string & p_name,
						   const Vec3f &	   p_center,
						   const float		   p_radius,
						   const Vec3f &	   p_color,
						   const float		   p_power )
			: BaseObject( p_name ), _geometry( p_center, p_radius ), _color( p_color ), _power( p_power )
		{

		}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

		BaseLight * generateLight() const
		{
			return new GlobeLight( _geometry.getCenter(), _geometry.getRadius(), _color, _power );
		}

	  private:
		SphereGeometry _geometry;
		Vec3f		   _color;
		float		   _power;
	};

} // namespace RT_ISICG
