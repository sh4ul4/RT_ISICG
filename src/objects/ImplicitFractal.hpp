#pragma once
#include "objects/implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitFractal : public ImplicitSurface
	{
	  public:
		ImplicitFractal()		   = delete;
		virtual ~ImplicitFractal() = default;

		ImplicitFractal( const std::string & p_name, const Vec3f & p_point, const Vec3f p_normal )
			: ImplicitSurface( p_name ), _point( p_point ), _normal( glm::normalize( p_normal ) )
		{
		}

	  private:
		Vec3f _point;
		Vec3f _normal;

		virtual float _sdf( const Vec3f & p_point ) const
		{
			Vec3f _point  = p_point - Vec3f( 0.f, 5.f, 6.f );
			float POWER	  = 2.f;
			Vec3f tmp_pos = _point;
			Vec3f cart_pos;
			float dr = 2.f;
			float r;
			float theta;
			float phi;
			float zr;
			for ( int tmp_iter = 0; tmp_iter < 10; tmp_iter++ )
			{
				r = glm::length( tmp_pos );
				if ( r > 2.f ) { break; }
				// approximate the distance differential
				dr = POWER * pow( r, POWER - 1.f ) * dr + 1.f;
				// calculate fractal surface
				// convert to polar coordinates
				theta = POWER * acos( tmp_pos.z / r );
				phi	  = POWER * atan2( tmp_pos.y, tmp_pos.x );
				zr	  = pow( r, POWER );
				// convert back to cartesian coordinated
				cart_pos.x = zr * sin( theta ) * cos( phi );
				cart_pos.y = zr * sin( theta ) * sin( phi );
				cart_pos.z = zr * cos( theta );
				tmp_pos	   = _point + cart_pos;
			}
			// distance estimator
			return 0.5f * log( r ) * r / dr;
		}
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			Vec3f _point = p_point - Vec3f( 3.f, 5.f, 6.f );
			Vec3f epsilon_x( glm::epsilon<float>(), 0, 0 );
			Vec3f epsilon_y( 0, glm::epsilon<float>(), 0 );
			Vec3f epsilon_z( 0, 0, glm::epsilon<float>() );
			Vec3f ray_perb_x1 = _point + epsilon_x;
			Vec3f ray_perb_y1 = _point + epsilon_y;
			Vec3f ray_perb_z1 = _point + epsilon_z;
			ray_perb_x1		  = _point + epsilon_x;
			ray_perb_y1		  = _point + epsilon_y;
			ray_perb_z1		  = _point + epsilon_z;
			Vec3f ray_perb_x2 = _point - epsilon_x;
			Vec3f ray_perb_y2 = _point - epsilon_y;
			Vec3f ray_perb_z2 = _point - epsilon_z;
			Vec3f surf_normal( _sdf( ray_perb_x1 ) - _sdf( ray_perb_x2 ),
							   _sdf( ray_perb_y1 ) - _sdf( ray_perb_y2 ),
							   _sdf( ray_perb_z1 ) - _sdf( ray_perb_z2 ) );
			return -glm::normalize( _normal );
		}
	};
} // namespace RT_ISICG