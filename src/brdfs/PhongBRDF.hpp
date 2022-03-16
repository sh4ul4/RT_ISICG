#pragma once
#include "defines.hpp"

//#define BLINNPHONG

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_ks, const float p_s ) : _ks( p_ks ), _s( p_s ) {};

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( const Ray &		   p_ray,
							   const HitRecord &   p_hitRecord,
							   const LightSample & p_lightSample ) const
		{
			const Vec3f wo = glm::normalize( -p_ray.getDirection() );
#ifndef BLINNPHONG
			const Vec3f wr
				= glm::normalize( glm::reflect( glm::normalize( -p_lightSample._direction ), p_hitRecord._normal ) );
			const float cosAlpha = glm::dot( wo, wr );
			return _ks * pow( cosAlpha, _s );
#else
			const float cosAlpha = glm::dot( glm::normalize(wo - p_lightSample._direction), p_hitRecord._normal );
			return _ks * pow( cosAlpha, _s );
#endif
		}

		inline const Vec3f & getKs() const { return _ks; }

	  private:
		float _s  = 1.f;
		Vec3f _ks = WHITE;
	};
} // namespace RT_ISICG
