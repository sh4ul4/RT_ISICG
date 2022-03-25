#include "aabb.hpp"

namespace RT_ISICG
{
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float			tmin	= p_tMin;
		float			tmax	= p_tMax;
		constexpr float epsilon = glm::epsilon<float>();

		const Vec3f & o = p_ray.getOrigin();
		const Vec3f & d = p_ray.getDirection();

		if ( glm::abs( d.x ) > epsilon )
		{
			const float tx1 = ( _min.x - o.x ) / d.x;
			const float tx2 = ( _max.x - o.x ) / d.x;

			tmin = glm::max( tmin, glm::min( tx1, tx2 ) );
			tmax = glm::min( tmax, glm::max( tx1, tx2 ) );
		}

		if ( glm::abs( d.y ) > epsilon )
		{
			const float ty1 = ( _min.y - o.y ) / d.y;
			const float ty2 = ( _max.y - o.y ) / d.y;

			tmin = glm::max( tmin, glm::min( ty1, ty2 ) );
			tmax = glm::min( tmax, glm::max( ty1, ty2 ) );
		}

		if ( glm::abs( d.z ) > epsilon )
		{
			const float tz1 = ( _min.z - o.z ) / d.z;
			const float tz2 = ( _max.z - o.z ) / d.z;

			tmin = glm::max( tmin, glm::min( tz1, tz2 ) );
			tmax = glm::min( tmax, glm::max( tz1, tz2 ) );
		}

		return tmax >= tmin;
		/*
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
		return false;*/
	}
} // namespace RT_ISICG
