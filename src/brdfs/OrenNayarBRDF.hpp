#pragma once

#include "defines.hpp"
#include "glm/gtx/polar_coordinates.hpp"

namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, const float p_sigma ) : _kd( p_kd ), _sigma(p_sigma) {};

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( const Vec3f & wi, const Vec3f & wo ) const
		{
			const float sigmaPow2 = _sigma * _sigma;
			const float phii	  = wi.x;
			const float phio	  = wo.x;
			const float thetai	  = wi.y;
			const float thetao	  = wo.y;
			const float alpha	  = glm::max( thetai, thetao );
			const float beta	  = glm::min( thetai, thetao );
			const float A		  = 1.f - 0.5f * ( sigmaPow2 / ( sigmaPow2 + 0.33f ) );
			const float B		  = 0.45f * ( sigmaPow2 / ( sigmaPow2 + 0.09f ) );
			return _kd * INV_PIf * 1.f
				   / ( A + ( B * glm::max( 0.f, glm::cos( phii - phio ) ) * glm::sin( alpha ) ) * glm::sin( beta ) );
		}

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _sigma = 0.f;
	};
} // namespace RT_ISICG
