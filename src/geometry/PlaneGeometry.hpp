#pragma once

#include "base_geometry.hpp"

namespace RT_ISICG
{
	class PlaneGeometry : public BaseGeometry
	{
	  private:
		Vec3f point, normal;

	  public:
		PlaneGeometry( const Vec3f & point, const Vec3f & normal ) : point( point ), normal( normal ) {}
		PlaneGeometry() = delete;

		inline const Vec3f & getPoint() const { return point; }
		inline const Vec3f & getNormal() const { return normal; }

		bool intersect( const Ray & p_ray, float & p_t ) const;
	};
} // namespace RT_ISICG