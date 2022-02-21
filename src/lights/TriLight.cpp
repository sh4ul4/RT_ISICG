#include "TriLight.hpp"

namespace RT_ISICG
{
	TriLight::TriLight( const Vec3f & a, const Vec3f & b, const Vec3f & c, const Vec3f & color, const float power )
		: BaseLight( color, power ), _a( a ), _b( b ), _c( c ), _n( glm::normalize( glm::cross( _a - _b, _c - _b ) ) )
	{
		_isSurface = true;
		const float sinTheta
			= glm::sin( glm::acos( glm::dot( glm::normalize( _a - _b ), glm::normalize( _c - _b ) ) ) );
		_area = glm::distance( _a, _b ) * glm::distance( _c, _b ) * sinTheta / 2.f;
	}

	LightSample TriLight::sample( const Vec3f & p_point ) const
	{
		const Vec3f randPos( _b + ( _a - _b ) * randomFloat() + ( _c - _b ) * randomFloat() );
		const float distance  = glm::distance( randPos, p_point );
		const Vec3f direction = glm::normalize( randPos - p_point );
		const float pdf		  = ( 1.f / _area ) * ( distance * distance ) / glm::dot( _n, direction );
		const Vec3f radiance  = ( _color * _power ) / pdf;
		return LightSample( -direction, distance, radiance, pdf );
	}
} // namespace RT_ISICG