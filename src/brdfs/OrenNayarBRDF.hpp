#pragma once

#include "defines.hpp"
#include "glm/gtx/polar_coordinates.hpp"

namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, const float p_sigma ) : _kd( p_kd ), _sigma(p_sigma) {};

		inline Vec3f evaluate( const Vec3f & wi, const Vec3f & wo, const Vec3f& n ) const
		{
			const float cosPhiWi = glm::dot( wi, n );
			const float cosPhiWo = glm::dot( wo, n );

			const float phiWi = glm::acos( cosPhiWi );
			const float phiWo = glm::acos( cosPhiWo );

			const Vec3f projWi		 = wi - n * cosPhiWi;
			const Vec3f projWo		 = wo - n * cosPhiWo;

			const float sigma2 = _sigma * _sigma;

			const float A = 1.f - .5f * sigma2 / ( sigma2 + .33f );
			const float B = .45f * sigma2 / ( sigma2 + .09f );

			const float alpha = glm::max( phiWi, phiWo );
			const float beta  = glm::min( phiWi, phiWo );

			return _kd * INV_PIf
				   * ( A
					   + B * glm::max( glm::dot( glm::normalize( projWi ), glm::normalize( projWo ) ), 0.f )
							 * glm::sin( alpha ) * glm::tan( beta ) );
		}

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _sigma = 0.f;
	};
} // namespace RT_ISICG
