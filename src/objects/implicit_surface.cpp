#include "implicit_surface.hpp"

namespace RT_ISICG
{
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		Vec3f point = p_ray.getOrigin() + p_tMin * p_ray.getDirection();
		while ( glm::distance( point, p_ray.getOrigin() ) < p_tMax )
		{
			float dist = _sdf( point );
			if ( glm::abs( dist ) <= _minDistance )
			{
				p_hitRecord._point	= point;
				p_hitRecord._normal = _evaluateNormal( point );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._distance = glm::distance( point, p_ray.getOrigin() );
				p_hitRecord._object	  = this;
				return true;
			}
			else if ( dist > _minDistance )
				point += p_ray.getDirection() * dist;
			else point -= p_ray.getDirection() * dist;
		}
		return false;
	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		Vec3f point = p_ray.getOrigin() + p_tMin * p_ray.getDirection();
		while ( glm::distance( point, p_ray.getOrigin() ) < p_tMax )
		{
			float dist = _sdf( point );
			if ( glm::abs( dist ) <= _minDistance )
				return true;
			else if ( dist > _minDistance )
				point += p_ray.getDirection() * dist;
			else point -= p_ray.getDirection() * dist;
		}
		return false;
	}
} // namespace RT_ISICG
