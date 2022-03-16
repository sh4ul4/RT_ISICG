#pragma once
#include "defines.hpp"

namespace RT_ISICG
{
	class CookTorranceBRDF
	{
		float G1( const float x, const float k ) const { return x / ( x * ( 1 - k ) + k ); }

	  public:
		CookTorranceBRDF( const Vec3f & p_ks, const float p_roughness, const Vec3f & p_F0 )
			: _ks( p_ks ), _theta( p_roughness ) {};

		inline Vec3f evaluate( const Vec3f & wi, const Vec3f & wo, const Vec3f& n ) const
		{
			const float alpha = _theta * _theta;
			const Vec3f h	  = ( wo + wi ) / glm::length( wo + wi );
			float		D
				= ( alpha * alpha ) / ( PIf * pow( powf( glm::dot( n, h ), 2 ) * ( alpha * alpha - 1.f ) + 1.f, 2 ) );

			float k	 = powf( _theta + 1.f, 2 ) / 8.f;
			float G = G1( glm::dot( n, wo ), k ) * G1( glm::dot( n, wi ), k );

			const Vec3f F = _F0 + (Vec3f)(( 1.f - _F0 ) * powf( 1.f - glm::dot( h, wo ), 5 ));

			const Vec3f specularRes = ( D * F * G ) / ( 4.f * glm::dot( wo, n ) * glm::dot( wi, n ) );
			return specularRes;
		}

		inline const Vec3f & getKs() const { return _ks; }

	  private:
		float _theta  = 8.f;
		Vec3f _F0	 = Vec3f( 0.f );
		Vec3f _ks	  = WHITE;
	};
} // namespace RT_ISICG
