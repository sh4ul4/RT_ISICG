#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{
		/// TODO !
		Vec3f D = p_ray.getDirection();
		Vec3f O = p_ray.getOrigin();
		Vec3f C = _center;

		float beta = 2.f * ( glm::dot( D, O - C ) );
		//float alpha = glm::dot(D, D);
		float gamma = glm::dot( O - C, O - C ) - _radius * _radius;
		float delta = beta * beta - 4.f /** alpha*/ * gamma;

		if ( delta >= 0 )
		{
			p_t1 = ( -beta - sqrt( delta ) ) / 2;
			p_t2 = ( -beta + sqrt( delta ) ) / 2;

			if ( p_t1 > p_t2 )
			{
				float tmp = p_t1;
				p_t1	  = p_t2;
				p_t2	  = tmp;
			}
			return true;
		}

		p_t1 = -1.f;
		p_t2 = -1.f;

		return false;
	}

} // namespace RT_ISICG
