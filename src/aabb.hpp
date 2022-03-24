#ifndef __RT_ISICG_AABB__
#define __RT_ISICG_AABB__

#include "ray.hpp"

namespace RT_ISICG
{
	struct AABB
	{
	  public:
		AABB() = default;
		AABB( const Vec3f & p_min, const Vec3f & p_max ) : _min( p_min ), _max( p_max ) {}
		~AABB() = default;

		inline const Vec3f & getMin() const { return _min; }
		inline const Vec3f & getMax() const { return _max; }

		// Returns true if the AABB is degenerated.
		inline bool isValid() const { return ( ( _min.x <= _max.x ) && ( _min.y <= _max.y ) && ( _min.z <= _max.z ) ); }

		// Extends the AABB with a point
		inline void extend( const Vec3f & p_point )
		{
			_min.x = _min.x < p_point.x ? _min.x : p_point.x;
			_min.y = _min.y < p_point.y ? _min.y : p_point.y;
			_min.z = _min.z < p_point.z ? _min.z : p_point.z;
			_max.x = _max.x > p_point.x ? _max.x : p_point.x;
			_max.y = _max.y > p_point.y ? _max.y : p_point.y;
			_max.z = _max.z > p_point.z ? _max.z : p_point.z;
		}
		// Extends the AABB with another AABB
		inline void extend( const AABB & p_aabb )
		{
			_min.x = _min.x < p_aabb._min.x ? _min.x : p_aabb._min.x;
			_min.y = _min.y < p_aabb._min.y ? _min.y : p_aabb._min.y;
			_min.z = _min.z < p_aabb._min.z ? _min.z : p_aabb._min.z;
			_max.x = _max.x > p_aabb._max.x ? _max.x : p_aabb._max.x;
			_max.y = _max.y > p_aabb._max.y ? _max.y : p_aabb._max.y;
			_max.z = _max.z > p_aabb._max.z ? _max.z : p_aabb._max.z;
		}

		// Returns the AABB diagonal vector.
		inline Vec3f diagonal() const { return _max - _min; }

		// Returns the AABB centroid.
		inline Vec3f centroid() const { return ( _min + _max ) * 0.5f; }

		// Returns the largest axis, 0 -> x, 1 -> y, 2 -> z
		inline size_t largestAxis() const
		{
			const Vec3f d = diagonal();
			if ( d.x > d.y && d.x > d.z )
				return 0;
			else if ( d.y > d.z )
				return 1;
			else
				return 2;
		}

		bool intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const;

	  private:
		Vec3f _min = Vec3f( FLT_MAX );
		Vec3f _max = Vec3f( -FLT_MAX );
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_AABB__
