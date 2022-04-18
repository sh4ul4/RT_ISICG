#include "aabb.hpp"

namespace RT_ISICG
{
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float			tmin	  = p_tMin;
		float			tmax	  = p_tMax;
		constexpr float epsilon	  = glm::epsilon<float>();
		const Vec3f &	origin	  = p_ray.getOrigin();
		const Vec3f &	direction = p_ray.getDirection();
		if ( std::abs( direction.x ) > epsilon )
		{
			const float t1 = ( _min.x - origin.x ) / direction.x;
			const float t2 = ( _max.x - origin.x ) / direction.x;
			tmin		   = std::max( tmin, std::min( t1, t2 ) );
			tmax		   = std::min( tmax, std::max( t1, t2 ) );
		}
		if ( std::abs( direction.y ) > epsilon )
		{
			const float t1 = ( _min.y - origin.y ) / direction.y;
			const float t2 = ( _max.y - origin.y ) / direction.y;
			tmin		   = std::max( tmin, std::min( t1, t2 ) );
			tmax		   = std::min( tmax, std::max( t1, t2 ) );
		}
		if ( std::abs( direction.z ) > epsilon )
		{
			const float t1 = ( _min.z - origin.z ) / direction.z;
			const float t2 = ( _max.z - origin.z ) / direction.z;
			tmin		   = std::max( tmin, std::min( t1, t2 ) );
			tmax		   = std::min( tmax, std::max( t1, t2 ) );
		}
		return tmax >= tmin;
	}
} // namespace RT_ISICG
