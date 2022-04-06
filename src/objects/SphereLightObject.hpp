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
						   const float		   p_power,
						   BaseLight *		   p_light )
			: BaseObject( p_name ), _geometry( p_center, p_radius ), _color( p_color ), _power( p_power ),
			  _light( p_light )
		{
		}

		virtual BaseLight * getLight() const override { return _light; }

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		SphereGeometry _geometry;
		Vec3f		   _color;
		float		   _power;
		BaseLight *	   _light;
	};

} // namespace RT_ISICG
