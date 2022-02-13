#ifndef __RT_ISICG_SPHERE_GEOMETRY__
#define __RT_ISICG_SPHERE_GEOMETRY__

#include "base_geometry.hpp"

namespace RT_ISICG
{
	class SphereGeometry : public BaseGeometry
	{
	  public:
		SphereGeometry()		  = delete;
		virtual ~SphereGeometry() = default;

		SphereGeometry( const Vec3f & p_center, const float p_radius ) : _center( p_center ), _radius( p_radius ) {}

		inline const Vec3f & getCenter() const { return _center; }
		inline const float	 getRadius() const { return _radius; }

		bool intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const;

		inline Vec3f computeNormal( const Vec3f & p_point ) const
		{
			/// TODO !
			return glm::normalize(p_point - _center);
		}

	  private:
		Vec3f _center = VEC3F_ZERO;
		float _radius = 1.f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE_GEOMETRY__
