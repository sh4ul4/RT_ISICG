#include "aabb.hpp"

namespace RT_ISICG
{
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		const Vec3f invDir = 1.f / p_ray.getDirection();
		const Vec3f rayO   = p_ray.getOrigin();

		const bool signDirX = invDir.x < 0.f;
		const bool signDirY = invDir.y < 0.f;
		const bool signDirZ = invDir.z < 0.f;

		Vec3f bbox		  = signDirX ? _max : _min;
		float tmin		  = ( bbox.x - rayO.x ) * invDir.x;
		bbox			  = signDirX ? _min : _max;
		float tmax		  = ( bbox.x - rayO.x ) * invDir.x;
		bbox			  = signDirY ? _max : _min;
		const float tymin = ( bbox.y - rayO.y ) * invDir.y;
		bbox			  = signDirY ? _min : _max;
		const float tymax = ( bbox.y - rayO.y ) * invDir.y;

		if ( ( tmin > tymax ) || ( tymin > tmax ) ) { return false; }
		if ( tymin > tmin ) { tmin = tymin; }
		if ( tymax < tmax ) { tmax = tymax; }

		bbox			  = signDirZ ? _max : _min;
		const float tzmin = ( bbox.z - rayO.z ) * invDir.z;
		bbox			  = signDirZ ? _min : _max;
		const float tzmax = ( bbox.z - rayO.z ) * invDir.z;

		if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) { return false; }
		if ( tzmin > tmin ) { tmin = tzmin; }
		if ( tzmax < tmax ) { tmax = tzmax; }
		if ( ( tmin < p_tMax ) && ( tmax > p_tMin ) ) { return true; }
		return false;
	}
} // namespace RT_ISICG
