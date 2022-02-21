#include "GlobeLight.hpp"

namespace RT_ISICG
{
	GlobeLight::GlobeLight( const Vec3f & position, const float radius, const Vec3f & color, const float power )
		: BaseLight( color, power ), _position( position ), _radius( radius )
	{
		_isSurface = true;
		_area = (4 * PIf * _radius * _radius)/2.f; // divide by 2 to keep area of hemisphere
	}

	bool GlobeLight::intersectGlobe(float& _t1, const Vec3f& _rayP, const Vec3f& _circleP) const {
		Vec3f D = glm::normalize(_rayP - _circleP);
		Vec3f O = _circleP;
		Vec3f C = _position;

		float beta = 2.f * ( glm::dot( D, O - C ) );
		// float alpha = glm::dot(D, D);
		float gamma = glm::dot( O - C, O - C ) - _radius * _radius;
		float delta = beta * beta - 4.f /** alpha*/ * gamma;

		if ( delta >= 0 )
		{
			_t1 = ( -beta - sqrt( delta ) ) / 2;
			float _t2 = ( -beta + sqrt( delta ) ) / 2;

			if ( _t1 > _t2 )
			{
				float tmp = _t1;
				_t1	  = _t2;
				_t2	  = tmp;
			}
			return true;
		}
		_t1 = -1.f;
		return false;
	}

	LightSample GlobeLight::sample( const Vec3f & p_point ) const
	{
		// find random vector in 2D circle
		float r		= _radius * sqrt( randomFloat() );
		float theta = randomFloat() * 2.f * PIf;
		Vec3f v( r * cos( theta ), r * sin( theta ), 0.f );

		// rotate back to 3D depending on the normal vectoor of the circle (2D representation of the visible hemisphere)
		glm::quat q( glm::normalize( p_point - _position ), Vec3f( 0.f, 0.f, 1.f ) );
		q		  = glm::normalize( q );
		Mat4f rot = glm::toMat4( q );
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
	}
} // namespace RT_ISICG
