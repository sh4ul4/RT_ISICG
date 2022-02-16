#include "QuadLight.hpp"

namespace RT_ISICG
{
	QuadLight::QuadLight( const Vec3f & position,
						  const Vec3f & u,
						  const Vec3f & v,
						  const Vec3f & color,
						  const float	power )
		: BaseLight( color, power ), _position( position ), _u( u ), _v( v ),
		  _n( glm::normalize( glm::cross( _u, _v ) ) )
	{
		_isSurface = true;
	}

	LightSample QuadLight::sample( const Vec3f & p_point ) const {
		const Vec3f randPos( _position + _u * randomFloat() + _v * randomFloat() );
		const float area	  = glm::length( _u ) * glm::length( _v );
		const float distance  = glm::distance( randPos, p_point );
		const Vec3f direction = glm::normalize( randPos - p_point );
		const float pdf		  = (1.f/area) * (distance * distance) / glm::dot(_n, direction);
		const Vec3f radiance  = ( _color * _power ) / pdf;
		return LightSample( -direction, distance, radiance, pdf );
	}
} // namespace RT_ISICG