#include "GlobeLight.hpp"
#include <glm/gtx/polar_coordinates.hpp>

namespace RT_ISICG
{
	GlobeLight::GlobeLight( const Vec3f & position, const float radius, const Vec3f & color, const float power )
		: BaseLight( color, power ), _position( position ), _radius( radius )
	{
		_isSurface = true;
		_area = (4 * PIf * _radius * _radius); // divide by 2 to keep area of hemisphere
	}

	bool GlobeLight::intersectGlobe(float& _t1, const Vec3f& D, const Vec3f& O) const {
		Vec3f C = _position;
		float _t2;
		float beta = 2.f * ( glm::dot( D, O - C ) );
		// float alpha = glm::dot(D, D);
		float gamma = glm::dot( O - C, O - C ) - _radius * _radius;
		float delta = beta * beta - 4.f /** alpha*/ * gamma;

		if ( delta >= 0 )
		{
			_t1 = ( -beta - sqrt( delta ) ) / 2;
			_t2 = ( -beta + sqrt( delta ) ) / 2;

			if ( _t1 > _t2 )
			{
				float tmp = _t1;
				_t1	  = _t2;
				_t2	  = tmp;
			}
			return true;
		}

		_t1 = -1.f;
		_t2 = -1.f;

		return false;
	}
#define VOLUMIC 1
	LightSample GlobeLight::sample( const Vec3f & p_point ) const
	{
#if VOLUMIC
		if ( glm::distance( p_point, _position ) <= _radius )
			return LightSample( glm::normalize( _position - p_point ), 0.f, _color * _power, 1.f );

		// this gives us the random position on the circle in 3D
		Vec3f randPos = glm::euclidean( Vec2f( randomFloat() * TWO_PIf, randomFloat() * TWO_PIf ) );
		randPos += _position;
		randPos = _position + ( randPos - _position ) * randomFloat() * _radius - 0.001f;

		// intersect the ray between the circle-point and p_point to get the corresponding point on the globe hemisphere
		float t1;
		Vec3f D = glm::normalize( randPos - p_point );
		if ( intersectGlobe( t1, D, p_point ) ) randPos = p_point + D * t1;
		const float distance  = glm::distance( randPos, p_point );
		const Vec3f direction = glm::normalize( randPos - p_point );
		const float pdf
			= ( 1.f / _area ) * ( distance * distance ) / glm::dot( glm::normalize( randPos - _position ), -direction );
		const Vec3f radiance = ( _color * _power ) / pdf;
		return LightSample( -direction, distance, radiance, pdf );
#else
		if ( glm::distance( p_point, _position ) <= _radius )
			return LightSample( glm::normalize( _position - p_point ), 0.f, _color * _power, 1.f );

		// find random vector in 2D circle
		float r		= _radius * sqrt( randomFloat() );
		float theta = randomFloat() * 2.f * PIf;
		Vec3f v( r * cos( theta ), r * sin( theta ), 0.f );

		// rotate back to 3D depending on the normal vector of the circle (2D representation of the visible hemisphere)
		glm::quat q( glm::normalize( p_point - _position ), Vec3f( 0.f, 0.f, 1.f ) );
		q			  = glm::normalize( q );
		Mat4f	  rot = glm::toMat4( q );
		glm::vec4 tmp = rot * glm::vec4( v, 1.f );

		// this gives us the random position on the circle in 3D
		Vec3f randPos = Vec3f( tmp.x, tmp.y, tmp.z );
		randPos += _position;

		// intersect the ray between the circle-point and p_point to get the corresponding point on the globe hemisphere
		float t1;
		Vec3f D = glm::normalize( p_point - randPos );
		if ( intersectGlobe( t1, p_point, randPos ) ) randPos = randPos + D * t1;

		const float distance  = glm::distance( randPos, p_point );
		const Vec3f direction = glm::normalize( randPos - p_point );
		const float pdf
			= ( 1.f / _area ) * ( distance * distance ) / glm::dot( glm::normalize( randPos - _position ), direction );
		const Vec3f radiance = ( _color * _power ) / pdf;
		return LightSample( -direction, distance, radiance, pdf );
#endif
	}
} // namespace RT_ISICG
